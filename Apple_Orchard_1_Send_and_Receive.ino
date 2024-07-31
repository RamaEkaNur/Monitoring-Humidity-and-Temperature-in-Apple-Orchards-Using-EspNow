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

struct_message receivedData;
struct_message myData;

DHT dht(DHTPIN, DHTTYPE);

uint8_t receiverAddress[] = {0x2c, 0xbc, 0xbb, 0x0c, 0x08, 0x94}; // MAC Address of Node 3

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  Serial.println("OnDataRecv called");
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  Serial.println("Data Received:");
  Serial.print("Node ID: "); Serial.println(receivedData.node_id);
  Serial.print("Cluster Head: "); Serial.println(receivedData.cluster_head);
  Serial.print("Temperature: "); Serial.println(receivedData.temperature);
  Serial.print("Humidity: "); Serial.println(receivedData.humidity);
  Serial.print("Soil Moisture: "); Serial.println(receivedData.soilMoisture == HIGH ? "Dry" : "Wet");

  // Send received data to Node 3
  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *) &receivedData, sizeof(receivedData));
  if (result == ESP_OK) {
    Serial.println("Forwarded received data with success");
  } else {
    Serial.print("Error forwarding received data: ");
    Serial.println(esp_err_to_name(result));
  }


  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int soilMoistureValue = digitalRead(SOIL_MOISTURE_PIN);

  strcpy(myData.node_id, "002");
  strcpy(myData.cluster_head, "CHN2");
  myData.temperature = temperature;
  myData.humidity = humidity;
  myData.soilMoisture = soilMoistureValue;

  result = esp_now_send(receiverAddress, (uint8_t *) &myData, sizeof(myData));
  if (result == ESP_OK) {
    Serial.println("Own data sent with success");
  } else {
    Serial.print("Error sending own data: ");
    Serial.println(esp_err_to_name(result));
  }
}

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

  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  // Remove all existing peers to avoid conflicts
  esp_now_del_peer(receiverAddress);

  // Add Node 3 as a peer
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add receiver peer");
    return;
  }
}

void loop() {
  // Loop does not need to do anything, everything is handled in callback
}



