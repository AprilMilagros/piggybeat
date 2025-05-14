#include <WiFi.h>
#include <SPI.h>
#include <LoRa.h>
#include <esp_now.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BAND 920500000
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

typedef struct struct_message {
  char json[250];
} struct_message;

struct_message myData;

uint8_t tsimAddress[] = {0x30, 0xC9, 0x22, 0xF0, 0x95, 0x2C};

void setup() {
  Serial.begin(115200);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ No se pudo iniciar el display OLED");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Iniciando...");
  display.display();

  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(18, 14, 26);
  if (!LoRa.begin(BAND)) {
    Serial.println("❌ Fallo al iniciar LoRa");
    display.println("LoRa error!");
    display.display();
    while (true);
  }
  Serial.println("✅ LoRa iniciado");

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Error al iniciar ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, tsimAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Error al agregar peer T-SIM");
    return;
  }

  Serial.println("✅ Receptora lista");
  display.println("Listo!");
  display.display();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0) {
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    Serial.println("📡 Recibido desde LoRa:");
    Serial.println(incoming);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Datos LoRa:");
    display.println(incoming);
    display.display();

    StaticJsonDocument<256> docLoRa;
    DeserializationError error = deserializeJson(docLoRa, incoming);

    if (error) {
      Serial.print("❌ Error al parsear JSON LoRa: ");
      Serial.println(error.c_str());
      return;
    }

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("⚠ Error al leer DHT11");
      return;
    }

    StaticJsonDocument<256> finalDoc;
    finalDoc["BPM"] = docLoRa["BPM"];
    finalDoc["TempPuerco"] = docLoRa["TempPuerco"];
    finalDoc["TempAmbiente"] = t;
    finalDoc["Humedad"] = h;

    serializeJson(finalDoc, myData.json);
    esp_err_t result = esp_now_send(tsimAddress, (uint8_t *)&myData, sizeof(myData));

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("BPM: " + String(docLoRa["BPM"].as<int>()));
    display.println("TempP: " + String(docLoRa["TempPuerco"].as<float>()));
    display.println("Amb: " + String(t));
    display.println("Hum: " + String(h));
    display.display();

    if (result == ESP_OK) {
      Serial.println("✅ Enviado por ESP-NOW:");
      Serial.println(myData.json);
    } else {
      Serial.println("❌ Error al enviar por ESP-NOW");
    }
  }
}