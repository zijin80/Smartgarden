//Include the library files
#define BLYNK_TEMPLATE_ID "TMPL6UaNEOqf8"
#define BLYNK_TEMPLATE_NAME "Smart garden"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "Ao09ubtWzonWo60WKDAl2O2_W-KGApeI ";
char ssid[] = "CACA";
char pass[] = "safa2024";

BlynkTimer timer;
bool Relay = 0;

//Define component pins
#define sensor A0
#define waterPump D3
#define DHTPIN D5 
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);



void setup() {
  Serial.begin(115200);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);
  

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);

  
  dht.begin();
  timer.setInterval(2500L, sendSensor);

  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
}

//Get the button value
BLYNK_WRITE(V3) {
  Relay = param.asInt();

  if (Relay == 1) {
    digitalWrite(waterPump, LOW);
    
  } else {
    digitalWrite(waterPump, HIGH);
  }
}
//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);

}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}
void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V2, t);
  Blynk.virtualWrite(V1, h);
  
  Serial.print("Suhu : ");
  Serial.print(t);
  Serial.print("|| Kelembapan : ");
  Serial.println(h);


  if(t > 30){
    Blynk.logEvent("notifikasi","Suhu diatas 30 Derajat celcius");
  }
}
