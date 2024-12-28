#include <Wire.h>
#include <MPU6050.h>
#include <smart_helm_inferencing.h>

MPU6050 mpu;

#define BUFFER_SIZE EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE
float imu_data_buffer[BUFFER_SIZE];
int buffer_index = 0;

void setup() {
    Serial.begin(115200);

    Serial.println("Initialize MPU6050");

    while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
        Serial.println("MPU6050 not found!");
        delay(500);
    }

    // Calibrate gyrometer
    mpu.calibrateGyro();
    mpu.setThreshold(3);

    Serial.println("MPU6050 ready.");
    delay(2000);

    Serial.println("Edge Impulse Inferencing with IMU data");
}

void loop() {
    // Read gyrometer and accelerometer data
    Vector rawGyro = mpu.readRawGyro();
    Vector rawAccel = mpu.readRawAccel();

    // Fill the buffer
    imu_data_buffer[buffer_index++] = rawGyro.XAxis;
    imu_data_buffer[buffer_index++] = rawGyro.YAxis;
    imu_data_buffer[buffer_index++] = rawGyro.ZAxis;
    imu_data_buffer[buffer_index++] = rawAccel.XAxis;
    imu_data_buffer[buffer_index++] = rawAccel.YAxis;
    imu_data_buffer[buffer_index++] = rawAccel.ZAxis;

    // Wrap around buffer index if it exceeds BUFFER_SIZE
    if (buffer_index >= BUFFER_SIZE) {
        buffer_index = 0;

        // Process the data using the Edge Impulse classifier
        run_inference();
    }

    delay(100); // Adjust sampling rate as needed
}

void run_inference() {
    // Set up the signal structure for Edge Impulse SDK
    signal_t features_signal;
    features_signal.total_length = BUFFER_SIZE;
    features_signal.get_data = &raw_feature_get_data;

    ei_impulse_result_t result = { 0 };

    // Run the classifier
    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);
    if (res != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", res);
        return;
    }

    // Print inference results
    print_inference_result(result);
}

// Fetch data from the IMU buffer
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, imu_data_buffer + offset, length * sizeof(float));
    return 0;
}

void print_inference_result(ei_impulse_result_t result) {
    ei_printf("Timing: DSP %d ms, inference %d ms, anomaly %d ms\r\n",
        result.timing.dsp,
        result.timing.classification,
        result.timing.anomaly);

#if EI_CLASSIFIER_OBJECT_DETECTION == 1
    ei_printf("Object detection bounding boxes:\r\n");
    for (uint32_t i = 0; i < result.bounding_boxes_count; i++) {
        ei_impulse_result_bounding_box_t bb = result.bounding_boxes[i];
        if (bb.value == 0) {
            continue;
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
    ei_printf("Predictions:\r\n");
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        ei_printf("  %s: ", ei_classifier_inferencing_categories[i]);
        ei_printf("%.5f\r\n", result.classification[i].value);
    }
#endif

#if EI_CLASSIFIER_HAS_ANOMALY
    ei_printf("Anomaly prediction: %.3f\r\n", result.anomaly);
#endif
}
