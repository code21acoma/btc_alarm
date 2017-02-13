#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`  https://github.com/squix78/esp8266-oled-ssd1306
#include "images.h"
#include "oeESP.h"

WiFiClient client;

const char* ssid     = "Your SSID";  
const char* password = "password";
const int httpPort = 80;
const char* host  = "yourwebsite.cz"; // Your domain  
String pathLed       = "/api/bitstamp-witty.php";  

String wittyGET;
String dolni;
String horni;
String btcLast;

float Dolni;
float Horni;
float BtcLast;

int alarm = 0;
String alarmActive = "0";


// Initialize the OLED display using brzo_i2c
// D3 -> SDA
// D5 -> SCL

SSD1306  display(0x3c, D3, D5);

const int buzzer = 5; //buzzer to arduino pin 5


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void json()
{

  Serial.println(" ");
  Serial.println("------------ connecting to PHP.JSON ");
  Serial.println(host);
  WiFiClient client;
  
  if (!client.connect(host, httpPort)) { Serial.println("connection failed");  return; }

  client.print(String("GET ") + pathLed + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");

  delay(500); // wait for server to respond

  // read response
  String section="header";
  while(client.available()){
    String line = client.readStringUntil('\r');
    // Serial.print(line);
    // we�ll parse the HTML body here
    if (section=="header") { // headers..
      Serial.print(".");
      if (line=="\n") { section="json"; } // skips the empty space at the beginning
    }
    else if (section=="json") {  // print the good stuff
      section="ignore";
      String result = line.substring(1);

      // Parse JSON
      Serial.println("parseObject: ");
      int size = result.length() + 1;
      char json[size];
      result.toCharArray(json, size);
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json_parsed = jsonBuffer.parseObject(json);
      if (!json_parsed.success()) { Serial.println("parseObject() failed");   return; }
      
      //Serial.println(json_parsed["datetime"]);
      //https://github.com/bblanchon/ArduinoJson/wiki/Decoding-JSON
      const char* last    = json_parsed["last"];
      Serial.println(last);

      //const char* timestamp    = json_parsed["timestamp"]; //short message
      //Serial.println(timestamp);
      
      // Make the decision to turn off or on the LED
      if (strcmp(json_parsed["light"], "on") == 0) {
        Serial.println("LED ON");
        //digitalWrite(pin, HIGH);
        //blinkGre(1000,1000);       
      }
      else {
        //digitalWrite(pin, LOW);
        Serial.println("led off");
        //blinkRed(3000,1000);
       }
    }
  }

  
}


void btclast() {

    Serial.println(" ");
    Serial.println("------------ resp() -------------");
    //blinkBlu();
    String URL = "yourwebsite.cz";
    //Serial.print("Requesting URL: ");
    //Serial.println(URL);
    
   if (client.connect(host, 80)) {
    client.println("GET /api/bitstamp-witty.php HTTP/1.1"); 
    client.println("Host: " + URL);
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("Connection failed to server");
    blinkRed(500,1000);
    return;
  }
  delay(500);

  while(client.available()){
    wittyGET = client.readStringUntil('\r');    
   }   

    btcLast = getValue(wittyGET, '#', 0);
    dolni = getValue(wittyGET, '#', 1);
    horni = getValue(wittyGET, '#', 2);
    alarmActive = getValue(wittyGET, '#', 3);
    BtcLast = btcLast.toFloat();
    Dolni = dolni.toFloat();
    Horni = horni.toFloat();
      
   display.clear();
   display.setFont(ArialMT_Plain_16);
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   display.drawString(0, 0, "BTC/USD");   
   display.drawString(0, 25, btcLast);   
   display.setFont(ArialMT_Plain_10);
   display.drawString(0, 50, dolni);   
   display.drawString(64, 50, horni); 
   if (alarmActive == "1")   display.drawString(80, 0, "alarm ON");   
   else display.drawString(80, 0, "alarm OFF");
   display.display();
   blinkBlu(200,1000);
   
}


void controlAlarm() {

  alarm = 0;

  if (Dolni > BtcLast) {
    
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(500);        // ...for 500 msec
    noTone(buzzer);     // Stop sound...
    delay(500);        // ...for 500 msec    
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(500);        // ...for 500 msec
    noTone(buzzer);     // Stop sound...
    delay(500);        // ...for 500 msec    
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(500);        // ...for 500 msec
    noTone(buzzer);     // Stop sound...           
    blinkRed(200,1000);
    blinkRed(200,1000);
    blinkRed(200,1000);
    alarm = 1;
  }

  if (Horni < BtcLast) {
    
    tone(buzzer, 200); // Send 1KHz sound signal...
    delay(200);        
    noTone(buzzer);    
    delay(100);      
    tone(buzzer, 400); // Send 1KHz sound signal...
    delay(500);        
    noTone(buzzer);    
    delay(500);      
    tone(buzzer, 200); // Send 1KHz sound signal...
    delay(200);        
    noTone(buzzer);    
    delay(100);      
    tone(buzzer, 400); // Send 1KHz sound signal...
    delay(500);        
    noTone(buzzer);    
    delay(500);      
    tone(buzzer, 200); // Send 1KHz sound signal...
    delay(200);        
    noTone(buzzer);    
    delay(100);      
    tone(buzzer, 400); // Send 1KHz sound signal...
    delay(500);        
    noTone(buzzer);        
    blinkGre(200,1000);
    blinkGre(200,1000);
    blinkGre(200,1000);
    alarm = 2;
  } 
  
}


void setup() {

  pinMode(buzzer, OUTPUT); // Set buzzer - pin 5 as an output
  
  Serial.begin(115200);
  //Serial.setDebugOutput(true);

  display.init();
  display.flipScreenVertically();  
  
  WiFi.mode(WIFI_STA);
  //WiFi.begin("ParalelniPolis", "vejdiven");
  //ledRGB(300);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  //String ip = WiFi.localIP();

  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(20, 30, "BitCoin Alarm");
  display.display();

  delay(5000);
  
  display.clear();
  display.drawXbm(34, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);  
  display.setFont(ArialMT_Plain_10);
  display.drawString(30, 40, WiFi.localIP().toString());
  display.display();

  delay(4000);

  
}

void loop() {

  btclast();
  if (alarmActive == "1") controlAlarm();
  delay(10000);
  
}
