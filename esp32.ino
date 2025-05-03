#include <driver/dac.h>
#include <math.h>

#define PI 3.14159265
#define DAC_CHANNEL DAC_CHANNEL_1  // GPIO25

const float f1 = 4100; // Hz
const float f2 = 4700; // Hz
const float A = 1.0;
const int sampleRate = 50000; // Hz
const int bufferSize = 200;   // Number of samples in buffer

uint8_t wave[bufferSize];

void setup() {
  dac_output_enable(DAC_CHANNEL);

  // Precompute wave samples
  for (int i = 0; i < bufferSize; i++) {
    float t = (float)i / sampleRate;
    float value = A * (cos(2 * PI * f1 * t) + cos(2 * PI * f2 * t));
    value = (value + 2.0) / 4.0; // Normalize to 0-1 range
    wave[i] = (uint8_t)(value * 255); // Scale to 8-bit DAC output
  }
}

void loop() {
  for (int i = 0; i < bufferSize; i++) {
    dac_output_voltage(DAC_CHANNEL, wave[i]);
    delayMicroseconds(1e6 / sampleRate);
  }
}
