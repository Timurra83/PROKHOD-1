#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <driver/ledc.h>

#define TX_PIN 16
#define PWM_CHANNEL LEDC_CHANNEL_0
#define PWM_RESOLUTION LEDC_TIMER_3_BIT
#define PWM_FREQ 7000000
#define PWM_DUTY 4
#define BEACON_INTERVAL 60000

Adafruit_BMP280 bmp;
#define BMP280_ADDRESS 0x76
const char* morseTable[] = {
  "-----", ".----", "..---", "...--", "....-",  // 0-4
  ".....", "-....", "--...", "---..", "----.",  // 5-9
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",      // A-G
  "....", "..", ".---", "-.-", ".-..", "--", "-.",      // H-N
  "---", ".--.", "--.-", ".-.", "...", "-", "..-",      // O-U
  "...-", ".--", "-..-", "-.--", "--.."                 // V-Z
};

void setup() {
  Serial.begin(115200);
  setupPWM();
  setupSensors();
  setupPower();
}

void setupPWM() {
  ledc_timer_config_t timer_conf = {
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .timer_num = LEDC_TIMER_0,
    .duty_resolution = PWM_RESOLUTION,
    .freq_hz = PWM_FREQ,
    .clk_cfg = LEDC_AUTO_CLK
  };
  ledc_timer_config(&timer_conf);
  
  ledc_channel_config_t channel_conf = {
    .gpio_num = TX_PIN,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .channel = PWM_CHANNEL,
    .intr_type = LEDC_INTR_DISABLE,
    .timer_sel = LEDC_TIMER_0,
    .duty = 0,
    .hpoint = 0
  };
  ledc_channel_config(&channel_conf);
}

void setupSensors() {
  if (!bmp.begin(BMP280_ADDRESS)) {
    Serial.println("BMP280 ERROR");
  } else {
    bmp.setSampling(
      Adafruit_BMP280::MODE_NORMAL,
      Adafruit_BMP280::SAMPLING_X2,
      Adafruit_BMP280::SAMPLING_X16,
      Adafruit_BMP280::FILTER_X16,
      Adafruit_BMP280::STANDBY_MS_500
    );
  }
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
}

void setupPower() {
  esp_sleep_enable_timer_wakeup(BEACON_INTERVAL * 1000);
}

void enableCarrier(bool enable) {
  if (enable) {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL, PWM_DUTY);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL);
  } else {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL, 0);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, PWM_CHANNEL);
  }
}

void sendMorseChar(char c) {
  if (c >= '0' && c <= '9') {
    const char* morse = morseTable[c - '0'];
    for (int i = 0; morse[i] != '\0'; i++) {
      if (morse[i] == '.') {
        enableCarrier(true);
        delay(200);  // Точка
        enableCarrier(false);
      } else if (morse[i] == '-') {
        enableCarrier(true);
        delay(600);  // Тире
        enableCarrier(false);
      }
      delay(200);  
    }
  } else if (c >= 'A' && c <= 'Z') {
    const char* morse = morseTable[c - 'A' + 10];
    for (int i = 0; morse[i] != '\0'; i++) {
      if (morse[i] == '.') {
        enableCarrier(true);
        delay(200);
        enableCarrier(false);
      } else if (morse[i] == '-') {
        enableCarrier(true);
        delay(600);
        enableCarrier(false);
      }
      delay(200);
    }
  }
  delay(600);  
}


void sendMorseString(const char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    sendMorseChar(str[i]);
  }
}

void readAndSendTelemetry() {
  // Чтение датчиков
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;
  uint16_t adcValue = adc1_get_raw(ADC1_CHANNEL_0);
  float voltage = (adcValue * 3.3 * 2 / 4095.0);
  
  char message[50];
  snprintf(message, sizeof(message), "ESP32 T%.1f P%.0f V%.1f", 
           temperature, pressure, voltage);
  
  for (int i = 0; message[i]; i++) {
    if (message[i] >= 'a' && message[i] <= 'z') {
      message[i] = message[i] - 32;
    }
  }
  
  Serial.print("TX: ");
  Serial.println(message);
  
  // Передача Морзе
  sendMorseString(message);
  
  
  enableCarrier(false);
  delay(1400);
}

void loop() {
  readAndSendTelemetry();
  enableCarrier(false);
  delay(10);
  esp_deep_sleep_start();
}
