EMISORA

#include <PulseSensorPlayground.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h> // Asegúrate de tener instalada la librería ArduinoJson

const int PULSE_INPUT = 35; // IO para el sensor de pulso
const int DS18B20_PIN = 22; // Pin para DS18B20
const int THRESHOLD = 550;
PulseSensorPlayground pulseSensor;

// Configuración para DS18B20
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);

// Frecuencia LoRa para México
#define BAND 920500000 // Ajusta según tu región

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 1000; // cada 1 segundo

void setup() {
  Serial.begin(115200);
  
  // Inicializar sensor de pulso
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.setThreshold(THRESHOLD);

  if (!pulseSensor.begin()) {
    while (true) {
      Serial.println("¡Error iniciando PulseSensor!");
      delay(500);
    }
  }

  // Inicializar sensor de temperatura
  sensors.begin();

  // Inicializar LoRa
  SPI.begin(5, 19, 27, 18); // Ajusta según tus conexiones
  LoRa.setPins(18, 14, 26); // Ajusta según tus pines
  if (!LoRa.begin(BAND)) {
    Serial.println("¡Fallo de inicio LoRa!");
    while (true); // Detener si falla
  }
  Serial.println("¡LoRa iniciado!");
}

void loop() {
  // Leer temperatura
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  // Leer pulso
  int signal = analogRead(PULSE_INPUT);
  int BPM = pulseSensor.getBeatsPerMinute();
  int IBI = pulseSensor.getInterBeatIntervalMs();

  if (BPM >= 40 && BPM <= 180 && IBI > 300) {
    Serial.print("IBI: ");
    Serial.print(IBI);
    Serial.print(" Señal: ");
    Serial.print(signal);
    Serial.print(" BPM: ");
    Serial.print(BPM);
    Serial.print(" TempPuerco: ");
    Serial.print(temperature);
    Serial.println(" °C");

    // Envío de paquetes LoRa en formato JSON
    if (millis() - lastSendTime >= sendInterval) {
      lastSendTime = millis();

      // Crear el objeto JSON
      StaticJsonDocument<200> jsonDoc;
      jsonDoc["IBI"] = IBI;
      jsonDoc["BPM"] = BPM;
      jsonDoc["TempPuerco"] = temperature;

      // Serializar el JSON en un buffer de caracteres
      char jsonBuffer[200];
      serializeJson(jsonDoc, jsonBuffer);

      // Enviar el paquete LoRa
      LoRa.beginPacket();
      LoRa.print(jsonBuffer);
      LoRa.endPacket();

      // Imprimir el JSON en el Serial Monitor
      Serial.println("Datos enviados:");
      Serial.println(jsonBuffer);
    }
  } else {
    Serial.print("Esperando datos válidos... Señal: ");
    Serial.print(signal);
    Serial.print(" TempPuerco: ");
    Serial.print(temperature);
    Serial.println(" °C");
  }

  pulseSensor.sawNewSample();
  pulseSensor.sawStartOfBeat();
}