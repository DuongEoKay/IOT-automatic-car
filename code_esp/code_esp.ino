#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#define LED 2   
#include "DHT.h"    


//dht
const int DHTPIN = 5;      
const int DHTTYPE = DHT11;  
DHT dht(DHTPIN, DHTTYPE);


//wifi
const char* ssid = "Văn Dương";
const char* password = "17112003";



//***Set server***
const char* mqttServer = "broker.hivemq.com"; 
int port = 1883;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);




void wifiConnect() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void mqttConnect() {
  while(!mqttClient.connected()) {
    Serial.println("Attemping MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if(mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");

      //***Subscribe all topic you need***
     mqttClient.subscribe("041072500/GET/Humid");
     mqttClient.subscribe("041072500/GET/Temp");
     mqttClient.subscribe("041072500/Humid");
     mqttClient.subscribe("041072500/Temp");
     mqttClient.subscribe("041072500/GET/Humid_cont");
     mqttClient.subscribe("041072500/GET/Temp_cont");
     mqttClient.subscribe("041072500/noti");
    }
    else {
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

//MQTT Receiver
void callback(char* topic, byte* message, unsigned int length) {
  Serial.println(topic);
  char buffer[3];
  itoa(std::rand(), buffer, 3);
  String strMsg;
  for(int i=0; i<length; i++) {
    strMsg += (char)message[i];
  }
  Serial.println(strMsg);

  //***Code here to process the received package***
  

  if (strMsg == "041072500/GET/Humid") 
  {
    int hInt = dht.readHumidity(); // Đọc giá trị độ ẩm là số nguyên 16-bit
    float h = static_cast<float>(hInt) / 1.0; 
    char buffer[20]; 
    sprintf(buffer, "%f", h);
    mqttClient.publish("041072500/Humid", buffer);

  }
  if (strMsg == "041072500/GET/Temp") 
  {
    int tInt = dht.readTemperature(); // Đọc giá trị nhiệt độ là số nguyên 16-bit
    float t = static_cast<float>(tInt) / 1.0; 
    char buffer[20]; 
    sprintf(buffer, "%f", t);
    mqttClient.publish("041072500/Temp", buffer);
    Serial.println(buffer);
  }
}





void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to WiFi");
  srand(time(NULL));
  wifiConnect();
  mqttClient.setServer(mqttServer, port);
  mqttClient.setCallback(callback);
  mqttClient.setKeepAlive( 90 );
  dht.begin();
  pinMode(LED,OUTPUT); 
}


void loop() {
  if(!mqttClient.connected()) {
    mqttConnect();
  }
  mqttClient.loop();

  int hInt = dht.readHumidity();      
  float h = static_cast<float>(hInt) / 1.0;   
  int tInt = dht.readTemperature(); // Đọc giá trị nhiệt độ là số nguyên 16-bit
  float t = static_cast<float>(tInt) / 1.0; 


  //gửi độ ẩm liên tục
  int hInt1 = dht.readHumidity();
  float h1 = static_cast<float>(hInt1) / 1.0; 
  char buffer1[20]; 
  sprintf(buffer1, "%f", h1);
  mqttClient.publish("041072500/Humid_cont", buffer1);
  if(h1>=70.00)
  {
    Serial.println("sent WARNING");
    mqttClient.publish("041072500/noti", "warning");
  }



  //gửi nhiệt độ liên tục
  int tInt2 = dht.readTemperature(); // Đọc giá trị nhiệt độ là số nguyên 16-bit
  float t2 = static_cast<float>(tInt2) / 1.0; 
  char buffer2[20];
  sprintf(buffer2, "%f", t2);
  mqttClient.publish("041072500/Temp_cont", buffer2);







  Serial.print("Nhiet do: ");
  Serial.println(t);               
  Serial.print("Do am: ");
  Serial.println(h);  
  Serial.println();               
  delay(1000);     

  //***Publish data to MQTT Server***







  //duong test code noi hai mach
if (Serial.available()) {
  String RxBuffer = "";
  while (Serial.available()) {
    char c = Serial.read();
    RxBuffer += c;
  }
  if(RxBuffer=="goal")
  {
    mqttClient.publish("041072500/noti", "noti");
  }
  
  Serial.print(RxBuffer);
  Serial.println();
}




//gui thong bao den dien thoai khi dung lai


  
}

