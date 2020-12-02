#include <DHT.h>

#include <ESP8266WiFi.h>
 
String apiKey = "API dari thingspeak";     //  Tulis API key kamu dari ThingSpeak
 
const char *ssid =  "Nama Wifi";     // Ganti dengan wifi dan password kamu
const char *pass =  "Password Wifi";
const char* server = "api.thingspeak.com";
 
#define DHTPIN 0          // Pin dimana dht11 terhubung
 
DHT dht(DHTPIN, DHT11);
 
WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Menghubungkan ke ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi terhubung");
 
}
 
void loop() 
{
  
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Gagal membaca dari sensor DHT!");
                      return;
                 }
 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Suhu: ");
                             Serial.print(t);
                             Serial.print(" derajat Celcius, Kelembaban: ");
                             Serial.print(h);
                             Serial.println("%. Kirim ke Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Menunggu ...");
  
  // Thingspeak membutuhkan delay minimal 15 detik untuk update data
  delay(1000);
}
