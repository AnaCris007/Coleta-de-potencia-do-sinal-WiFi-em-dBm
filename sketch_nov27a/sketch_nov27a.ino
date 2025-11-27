#include <WiFi.h>
#include <PubSubClient.h>

// =================== CONFIGURAÇÕES ===================
const char* WIFI_SSID = "Inteli.Iot";
const char* WIFI_PASSWORD = "%(Yk(sxGMtvFEs.3";

// Ubidots
#define UBIDOTS_TOKEN "BBUS-zyaga0Ee7GQ2Pdppt1S6vb8jcYTCkm"
#define MQTT_SERVER "industrial.api.ubidots.com"
#define MQTT_PORT 1883

// tópico: /v1.6/devices/esp32/rssi
const char* MQTT_TOPIC = "/v1.6/devices/esp32/rssi";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// =================== WIFI ===================
void connectWiFi() {
  Serial.println("\nReconectando ao WiFi...");
  WiFi.disconnect(true);     // limpa conexão antiga
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int tentativas = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tentativas++;

    if (tentativas > 20) {  // tenta por 10 segundos
      Serial.println("\nFalha ao conectar WiFi!");
      Serial.print("Status: ");
      Serial.println(WiFi.status());
      return;
    }
  }

  Serial.println("\nWiFi reconectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}


// =================== MQTT ===================
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao Ubidots MQTT... ");

    // Cliente MQTT único
    String clientId = "esp32-" + String(random(0xffff), HEX);

    /*
      Ubidots usa:
      username = TOKEN
      password = vazio
    */
    if (client.connect(clientId.c_str(), UBIDOTS_TOKEN, "")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falhou, rc=");
      Serial.println(client.state());
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  connectWiFi();

  client.setServer(MQTT_SERVER, MQTT_PORT);
  connectMQTT();
}

void loop() {
  // --- Verifica WiFi ---
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi perdido! Tentando reconectar...");
    connectWiFi();  // tenta novamente
    return;         // evita tentar MQTT sem rede
  }

  // --- Verifica MQTT ---
  if (!client.connected()) {
    Serial.println("MQTT desconectado! Tentando reconectar...");
    connectMQTT();
  }

  client.loop();

  // Medir a força do WiFi
  long rssi = WiFi.RSSI();

  Serial.print("RSSI: ");
  Serial.print(rssi);
  Serial.println(" dBm");

  // JSON do Ubidots
  char payload[50];
  sprintf(payload, "{\"value\": %ld}", rssi);

  // Enviar ao Ubidots
  client.publish(MQTT_TOPIC, payload);

  Serial.println("Enviado ao Ubidots!\n");

  delay(5000);
}

