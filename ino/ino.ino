#define NEOPIXEL_PIN    4
#define NEOPIXEL_COUNT  16

#define LED_ESP_BUILTIN 2
#define LED_ESP_ON LOW
#define LED_ESP_OFF HIGH

void setup() {
  Serial.begin(115200);
  pinMode(LED_ESP_BUILTIN, OUTPUT);
  delay(100);
  digitalWrite(LED_ESP_BUILTIN, LED_ESP_OFF);
  wlan_setup();
  rest_setup();
}

void loop() {
  //blink();
  rest_handleClient();
}
