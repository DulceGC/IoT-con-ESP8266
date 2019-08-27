//curso de IoT 2019

#include <ESP8266WiFi.h>
#include <aREST.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

//def
#define DHTPIN D2
#define DHTTYPE DHT22
#define MQ A0

//init
DHT dht(DHTPIN, DHTTYPE);

//aREST
aREST rest = aREST();

//WiFi
const char* ssid = "snapoffice_01"; //modem de wifi
const char* password = "SnapAguascalientes"; //contraseña de tu modem

//port 
#define LISTEN_PORT 80

//Create instance of server
WiFiServer server(LISTEN_PORT);

//variables API
float mq,humidity,temperature;

void setup(){
  Serial.begin(115200);
  dht.begin();

//init variables API
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);
  rest.variable("pollution",&mq);
  
 //Name ID
  rest.set_id("1");
  rest.set_name("sensor_NodeMCU");

  //conect to wifi
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");

  //start server
  server.begin();
  Serial.println("Server started");

  //IP
  Serial.println(WiFi.localIP());
}
void loop(){
    //wait 1s
    delay(1000);
    mq = analogRead(MQ); //MQ135
    humidity = dht.readHumidity(); //RH %0 - 100 (Punto de rocio)
    temperature = dht.readTemperature(); //0 - 100 *c

    //REST calls
    WiFiClient client = server.available();
    if (!client) {
      return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
}


 
