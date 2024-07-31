#include <DHT.h>
#include <esp_now.h>
#include <WiFi.h>

#define DHTPIN 23
#define SOIL_MOISTURE_PIN 4

#define DHTTYPE DHT11

typedef struct struct_message {
  char node_id[4];
  char cluster_head[5];
  float temperature;
  float humidity;
  int soilMoisture;
} struct_message;

struct_message myData;

DHT dht(DHTPIN, DHTTYPE);

uint8_t broadcastAddress[] = {0x08, 0xA6, 0xF7, 0xBC, 0x3D, 0xDC}; // MAC Address Node 2

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  Serial.println("DHT11 and Soil Moisture Sensor Test");

  dht.begin();

  pinMode(SOIL_MOISTURE_PIN, INPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int soilMoistureValue = digitalRead(SOIL_MOISTURE_PIN);

  strcpy(myData.node_id, "001");
  strcpy(myData.cluster_head, "CHN1");
  myData.temperature = temperature;
  myData.humidity = humidity;
  myData.soilMoisture = soilMoistureValue;

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
    Serial.print("Node ID: "); Serial.println(myData.node_id);
    Serial.print("Cluster Head: "); Serial.println(myData.cluster_head);
    Serial.print("Temperature: "); Serial.println(myData.temperature);
    Serial.print("Humidity: "); Serial.println(myData.humidity);
    Serial.print("Soil Moisture: "); Serial.println(myData.soilMoisture);
  } else {
    Serial.println("Error sending the data");
  }
}

