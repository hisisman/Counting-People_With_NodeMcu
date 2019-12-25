#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#include <Wire.h>
#include <Adafruit_AMG88xx.h>


#define WLAN_SSID       "wlan-ssid"
#define WLAN_PASS       "12345678"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME  "adafruit-username"
#define AIO_KEY       "adafruif-aio-key"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish myFirstValue = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/otobusDoluluk");


void MQTT_connect();
Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

void setup() {
    Serial.begin(9600);

    bool status;
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
    
    status = amg.begin();
    if (!status) {
        while (1);
    }
    delay(100); // let sensor boot up
}

uint8_t kisi=0;

void loop() { 
  MQTT_connect();
    amg.readPixels(pixels);
    
    for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
      Serial.print(pixels[i-1]);
      Serial.print(" "); 
    }

    char c=Serial.read();
    if(c=='1'){
      myFirstValue.publish(kisi);  
      kisi++;    
    }

    delay(1000); 
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }


  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
}
