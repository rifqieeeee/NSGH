#include <Wire.h>
#include <MPU6050.h>
#include <smart_helm_inferencing.h>

MPU6050 mpu;

// LED pins
const int ledPins[] = {0, 1, 2, 21, 16, 17};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

// LED blinking variables
unsigned long ledTimers[numLeds];
const unsigned long blinkInterval = 500;
int currentLed = 0;

// MPU6050 and classification variables
#define BUFFER_SIZE EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE
float imu_data_buffer[BUFFER_SIZE];
int buffer_index = 0;

void setup() {
    // Initialize LEDs
    for (int i = 0; i < numLeds; i++) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
        ledTimers[i] = millis();
    }

    // Initialize MPU6050
    Serial.begin(115200);
    Serial.println("Initialize MPU6050");

    while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
        Serial.println("MPU6050 not found!");
        delay(500);
    }

    mpu.calibrateGyro();
    mpu.setThreshold(3);

    Serial.println("MPU6050 ready.");
    delay(2000);

    Serial.println("Edge Impulse Inferencing with IMU data");
}

void loop() {
    handleLeds();
    handleClassifier();
}

void handleLeds() {
    unsigned long currentTime = millis();

    // Update the current LED state
    for (int i = 0; i < numLeds; i++) {
        if (currentTime - ledTimers[i] >= blinkInterval) {
            digitalWrite(ledPins[i], HIGH);
            delay(50); // Small ON delay
            digitalWrite(ledPins[i], LOW);
            ledTimers[i] = currentTime;
        }
    }
}

void handleClassifier() {
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

    delay(10); // Adjust sampling rate as needed
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
        Serial.printf("ERR: Failed to run classifier (%d)\n", res);
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
    Serial.printf("Timing: DSP %d ms, inference %d ms, anomaly %d ms\n",
        result.timing.dsp,
        result.timing.classification,
        result.timing.anomaly);

#if EI_CLASSIFIER_OBJECT_DETECTION == 1
    Serial.println("Object detection bounding boxes:");
    for (uint32_t i = 0; i < result.bounding_boxes_count; i++) {
        ei_impulse_result_bounding_box_t bb = result.bounding_boxes[i];
        if (bb.value == 0) {
            continue;
        }
        Serial.printf("  %s (%f) [ x: %u, y: %u, width: %u, height: %u ]\n",
            bb.label,
            bb.value,
            bb.x,
            bb.y,
            bb.width,
            bb.height);
    }
#else
    Serial.println("Predictions:");
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        Serial.printf("  %s: %.5f\n", ei_classifier_inferencing_categories[i],
            result.classification[i].value);
    }
#endif

#if EI_CLASSIFIER_HAS_ANOMALY
    Serial.printf("Anomaly prediction: %.3f\n", result.anomaly);
#endif
}
