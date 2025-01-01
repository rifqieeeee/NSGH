#include <Wire.h>
#include <MPU6050.h>
#include <smart_helm_inferencing.h>

// MPU6050 object for IMU sensor communication
MPU6050 mpu;

// Define buffer size based on Edge Impulse input frame size
#define BUFFER_SIZE EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE
float imu_data_buffer[BUFFER_SIZE]; // Buffer to store IMU data
int buffer_index = 0; // Index to keep track of buffer position

void setup() {
    Serial.begin(115200);

    Serial.println("Initialize MPU6050");

    // Initialize MPU6050 and verify connection
    while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
        Serial.println("MPU6050 not found!"); // Error message if sensor is not detected
        delay(500);
    }

    // Calibrate the gyrometer for precise measurements
    mpu.calibrateGyro();
    mpu.setThreshold(3); // Set threshold to filter small noise

    Serial.println("MPU6050 ready.");
    delay(2000); // Allow time for sensor stabilization

    Serial.println("Edge Impulse Inferencing with IMU data");
}

void loop() {
    // Read raw gyroscope and accelerometer data
    Vector rawGyro = mpu.readRawGyro();
    Vector rawAccel = mpu.readRawAccel();

    // Store IMU data into buffer in sequence (X, Y, Z for both gyro and accel)
    imu_data_buffer[buffer_index++] = rawGyro.XAxis;
    imu_data_buffer[buffer_index++] = rawGyro.YAxis;
    imu_data_buffer[buffer_index++] = rawGyro.ZAxis;
    imu_data_buffer[buffer_index++] = rawAccel.XAxis;
    imu_data_buffer[buffer_index++] = rawAccel.YAxis;
    imu_data_buffer[buffer_index++] = rawAccel.ZAxis;

    // Reset the buffer index and process data when buffer is full
    if (buffer_index >= BUFFER_SIZE) {
        buffer_index = 0; // Reset for circular buffering

        // Call function to process data using Edge Impulse classifier
        run_inference();
    }

    delay(100); // Sampling rate delay (adjust as needed)
}

void run_inference() {
    // Setup Edge Impulse signal structure for IMU data
    signal_t features_signal;
    features_signal.total_length = BUFFER_SIZE;
    features_signal.get_data = &raw_feature_get_data;

    ei_impulse_result_t result = { 0 };

    // Run the Edge Impulse classifier
    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);
    if (res != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", res);
        return; // Exit on error
    }

    // Print the results of inference
    print_inference_result(result);
}

// Function to fetch data from IMU buffer for inference
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, imu_data_buffer + offset, length * sizeof(float)); // Copy requested data
    return 0; // Success
}

void print_inference_result(ei_impulse_result_t result) {
    // Print timing details of each inference step
    ei_printf("Timing: DSP %d ms, inference %d ms, anomaly %d ms\r\n",
        result.timing.dsp,
        result.timing.classification,
        result.timing.anomaly);

#if EI_CLASSIFIER_OBJECT_DETECTION == 1
    // Print object detection bounding boxes if applicable
    ei_printf("Object detection bounding boxes:\r\n");
    for (uint32_t i = 0; i < result.bounding_boxes_count; i++) {
        ei_impulse_result_bounding_box_t bb = result.bounding_boxes[i];
        if (bb.value == 0) {
            continue; // Skip boxes with no detections
        }
        ei_printf("  %s (%f) [ x: %u, y: %u, width: %u, height: %u ]\r\n",
            bb.label,
            bb.value,
            bb.x,
            bb.y,
            bb.width,
            bb.height);
    }
#else
    // Print classification results
    ei_printf("Predictions:\r\n");
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        ei_printf("  %s: ", ei_classifier_inferencing_categories[i]);
        ei_printf("%.5f\r\n", result.classification[i].value);
    }
#endif

#if EI_CLASSIFIER_HAS_ANOMALY
    // Print anomaly detection result if supported
    ei_printf("Anomaly prediction: %.3f\r\n", result.anomaly);
#endif
}
