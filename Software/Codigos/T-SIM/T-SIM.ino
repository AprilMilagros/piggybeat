T-SIM

#define TINY_GSM_MODEM_SIM7000

#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <esp_now.h>

#define SerialMon Serial
#define SerialAT Serial1

#define MODEM_PWRKEY 4
#define MODEM_TX 27
#define MODEM_RX 26
#define MODEM_BAUD 9600

TinyGsm modem(SerialAT);
TinyGsmClient espClient(modem);
PubSubClient client(espClient);

// Parámetros del broker MQTT (actualizados)
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_user = "";
const char* mqtt_pass = "";

// Estructura para recibir datos de ESP-NOW
typedef struct struct_message {
  char data[250]; // Cambiado para recibir el mensaje completo
} struct_message;

struct_message receivedData;

// Variable para verificar si hay datos nuevos
bool newData = false;

// Callback para datos recibidos por ESP-NOW
void OnDataRecv(const esp_now_recv_info *recvInfo, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  SerialMon.print("Datos recibidos: ");
  SerialMon.println(receivedData.data);
  newData = true;
}

void powerOnModem() {
  pinMode(MODEM_PWRKEY, OUTPUT);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, HIGH);
}

void setup() {
  SerialMon.begin(115200);
  delay(3000);
  powerOnModem();

  SerialAT.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  SerialMon.println("🔄 Verificando comunicación con el módem...");
  while (!modem.testAT()) {
    SerialMon.print(".");
    delay(1000);
  }
  SerialMon.println("\n✅ Módem detectado");

  SerialMon.print("📶 Verificando SIM... ");
  if (modem.getSimStatus() == SIM_READY) {
    SerialMon.println("✅ SIM detectado");
  } else {
    SerialMon.println("❌ SIM NO detectado");
    return;
  }

  // Inicializar ESP-NOW
  WiFi.mode(WIFI_STA);
  delay(1000);
  SerialMon.print("Dirección MAC de SIM7000G: ");
  SerialMon.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) {
    SerialMon.println("Error al inicializar ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  // Conectar a GPRS
  modem.gprsConnect("internet.itelcel.com", "", "");
  if (modem.isGprsConnected()) {
    SerialMon.println("✅ Conexión GPRS establecida");
    SerialMon.print("Dirección IP: ");
    SerialMon.println(modem.getLocalIP());
  } else {
    SerialMon.println("❌ No se pudo conectar a GPRS");
    return;
  }

  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    String clientId = "SIM7000G_Client_" + String(random(1000));
    SerialMon.print("Conectando a MQTT...");
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      SerialMon.println("✅ Conectado a MQTT");
    } else {
      SerialMon.print(".");
      delay(10000);
    }
  }
}

void loop() {
  client.loop();

  if (newData) {
    // Publicar los datos recibidos en MQTT
    client.publish("nicamex/porcino", receivedData.data);

    SerialMon.print("Publicado en MQTT: ");
    SerialMon.println(receivedData.data);

    newData = false;
  }

  delay(1000);
}