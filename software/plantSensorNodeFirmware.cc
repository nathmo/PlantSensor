#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
#include <json.hpp>
#include <map>   
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <Ticker.h>
#include <DHTesp.h>
 
const char* ssid = "SSID";
const char* password =  "PASSWORD";
 
const char * host =  "plantSensor00";
IPAddress serverIp;
const int measurementIntervalMs = 20000;

const int sensorPin = 17;
DHTesp dht;


AsyncWebServer server(80);
ESPDash dashboard(&server); 
 
std::map<std::string, Card> sensorCards;
 
void connectWiFi(){
 
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to WiFi network");
}
 
void startMdns(){
   
  while(!MDNS.begin(host)) {
     Serial.println("Starting mDNS...");
     delay(1000);
  }
 
  Serial.println("MDNS started");
}
 
void resolveHostAddress(){
   
  while(mdns_init()!= ESP_OK){
    delay(1000);
    Serial.println("Starting MDNS...");
  }
 
  Serial.println("MDNS started");
 
  while (serverIp.toString() == "0.0.0.0") {
    Serial.println("Resolving host...");
    delay(250);
    serverIp = MDNS.queryHost(host);
  }
 
  Serial.println("Gateway address resolved");
}

void addCardIfNotExists(std::string id){
   
  if (sensorCards.find(id) == sensorCards.end()) {
 
    sensorCards.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(id), 
      std::forward_as_tuple(&dashboard, TEMPERATURE_CARD, id.c_str(), "°C"));
  }
}

void postMeasurement() {
   
  float temp = dht.getTemperature();
 
  nlohmann::json measurement;  
  measurement["id"] = WiFi.macAddress().c_str();
  measurement["val"] = temp;
 
  std::string serializedObject = measurement.dump();
 
  HTTPClient http;   
  
  http.begin("http://" + serverIp.toString() + "/measurement");
  http.addHeader("Content-Type", "application/json");            
  
  int httpResponseCode = http.POST(serializedObject.c_str());
  
  Serial.println(httpResponseCode);
 
  http.end();
}
 
void updateCard(std::string id, float val){
  sensorCards.at(id).update(val);
  dashboard.sendUpdates();
}
 
void setup() {
  Serial.begin(115200);
  
  dht.setup(sensorPin, DHTesp::DHT22);
   
  connectWiFi();
  startMdns(); 
  resolveHostAddress();
  
  server.on(
    "/measurement",
    HTTP_POST,
    [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
 
      nlohmann::json obj = nlohmann::json::parse(data, data+len);
 
      auto id = obj["id"].get<std::string>();
      auto val = obj["val"].get<float>();
 
      addCardIfNotExists(id);
      updateCard(id, val);
       
      request->send(200);          
  });
 
  server.begin();
  
  periodicTicker.attach_ms(measurementIntervalMs, postMeasurement);
}
  
void loop() {}
