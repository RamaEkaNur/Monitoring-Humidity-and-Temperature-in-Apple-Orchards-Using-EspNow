#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  char node_id[4];
  char cluster_head[5];
  float temperature;
  float humidity;
  int soilMoisture;
} struct_message;

struct_message myData;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println("Data Received:");
  Serial.print("Node ID: ");
  Serial.println(myData.node_id);
  Serial.print("Cluster Head: ");
  Serial.println(myData.cluster_head);
  Serial.print("Temperature: ");
  Serial.print(myData.temperature);
  Serial.print(" *C, Humidity: ");
  Serial.print(myData.humidity);
  Serial.print(" %, Soil Moisture: ");
  Serial.println(myData.soilMoisture == HIGH ? "Dry" : "Wet");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Nothing to do here, everything is handled by the callback
}

///////////////////////////////////////////////////////////


// #include <esp_now.h>
// #include <WiFi.h>
// #include <FS.h>
// #include <SPIFFS.h>
// #include <ArduinoJson.h>

// typedef struct struct_message {
//   char node_id[4];
//   char cluster_head[5];
//   float temperature;
//   float humidity;
//   int soilMoisture;
// } struct_message;

// struct_message myData;

// void writeDataToFile(struct_message data) {
//   // Create a JSON object
//   StaticJsonDocument<200> jsonDoc;
//   jsonDoc["node_id"] = data.node_id;
//   jsonDoc["cluster_head"] = data.cluster_head;
//   jsonDoc["temperature"] = data.temperature;
//   jsonDoc["humidity"] = data.humidity;
//   jsonDoc["soil_moisture"] = (data.soilMoisture == HIGH) ? "Dry" : "Wet";

//   // Convert JSON object to string
//   String jsonString;
//   serializeJson(jsonDoc, jsonString);

//   // Create a file path based on node_id
//   String filePath = String("/") + data.node_id + ".json";

//   // Open file for writing
//   File file = SPIFFS.open(filePath, FILE_WRITE);
//   if (!file) {
//     Serial.println("Failed to open file for writing");
//     return;
//   }

//   // Write JSON string to file
//   file.print(jsonString);
//   file.close();

//   Serial.print("Data written to file: ");
//   Serial.println(filePath);
// }

// void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
//   memcpy(&myData, incomingData, sizeof(myData));
//   Serial.println("Data Received:");
//   Serial.print("Node ID: ");
//   Serial.println(myData.node_id);
//   Serial.print("Cluster Head: ");
//   Serial.println(myData.cluster_head);
//   Serial.print("Temperature: ");
//   Serial.print(myData.temperature);
//   Serial.print(" *C, Humidity: ");
//   Serial.print(myData.humidity);
//   Serial.print(" %, Soil Moisture: ");
//   Serial.println(myData.soilMoisture == HIGH ? "Dry" : "Wet");

//   // Write the received data to a JSON file
//   writeDataToFile(myData);
// }

// void setup() {
//   Serial.begin(115200);
//   WiFi.mode(WIFI_STA);

//   // Initialize SPIFFS
//   if (!SPIFFS.begin(true)) {
//     Serial.println("An Error has occurred while mounting SPIFFS");
//     return;
//   }

//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }

//   esp_now_register_recv_cb(OnDataRecv);
// }

// void loop() {
//   // Nothing to do here, everything is handled by the callback
// }




//////////////////////////////////////////





// #include <esp_now.h>
// #include <WiFi.h>
// #include <FS.h>
// #include <SPIFFS.h>
// #include <ArduinoJson.h>

// typedef struct struct_message {
//   char node_id[4];
//   char cluster_head[5];
//   float temperature;
//   float humidity;
//   int soilMoisture;
// } struct_message;

// struct_message myData;

// void writeDataToFile(struct_message data) {
//   // Create a JSON object
//   StaticJsonDocument<200> jsonDoc;
//   jsonDoc["node_id"] = data.node_id;
//   jsonDoc["cluster_head"] = data.cluster_head;
//   jsonDoc["temperature"] = data.temperature;
//   jsonDoc["humidity"] = data.humidity;
//   jsonDoc["soil_moisture"] = (data.soilMoisture == HIGH) ? "Dry" : "Wet";

//   // Convert JSON object to string
//   String jsonString;
//   serializeJson(jsonDoc, jsonString);

//   // Create a file path based on node_id
//   String filePath = String("/") + data.node_id + ".json";

//   // Open file for writing
//   File file = SPIFFS.open(filePath, FILE_WRITE);
//   if (!file) {
//     Serial.println("Failed to open file for writing");
//     return;
//   }

//   // Write JSON string to file
//   file.print(jsonString);
//   file.close();

//   Serial.print("Data written to file: ");
//   Serial.println(filePath);
// }

// void readFileFromSPIFFS(const char *path) {
//   File file = SPIFFS.open(path);
//   if (!file) {
//     Serial.println("Failed to open file for reading");
//     return;
//   }

//   Serial.println("File Content:");
//   while (file.available()) {
//     Serial.write(file.read());
//   }
//   file.close();
// }

// void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
//   memcpy(&myData, incomingData, sizeof(myData));
//   Serial.println("Data Received:");
//   Serial.print("Node ID: ");
//   Serial.println(myData.node_id);
//   Serial.print("Cluster Head: ");
//   Serial.println(myData.cluster_head);
//   Serial.print("Temperature: ");
//   Serial.print(myData.temperature);
//   Serial.print(" *C, Humidity: ");
//   Serial.print(myData.humidity);
//   Serial.print(" %, Soil Moisture: ");
//   Serial.println(myData.soilMoisture == HIGH ? "Dry" : "Wet");

//   // Write the received data to a JSON file
//   writeDataToFile(myData);

//   // Read the data back from the file and display it
//   String filePath = String("/") + myData.node_id + ".json";
//   readFileFromSPIFFS(filePath.c_str());
// }

// void setup() {
//   Serial.begin(115200);
//   WiFi.mode(WIFI_STA);

//   // Initialize SPIFFS
//   if (!SPIFFS.begin(true)) {
//     Serial.println("An Error has occurred while mounting SPIFFS");
//     return;
//   }

//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }

//   esp_now_register_recv_cb(OnDataRecv);
// }

// void loop() {
//   // Nothing to do here, everything is handled by the callback
// }
