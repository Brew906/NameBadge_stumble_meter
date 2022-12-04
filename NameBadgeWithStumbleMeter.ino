#include <ETH.h>
#include <M5StickCPlus.h>
#include <WiFi.h>
#include "secrets.h"
#include<HTTPClient.h>

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

const char* resource = "/trigger/Button_Press/with/key/f7es5dadMw0EoH0c0WuLun0BzBXdzlU-2UUK4uXjTTb";
const char* server = "maker.ifttt.com";

int datapin = 26; //blue
int clockpin = 25; //green
int latchpin = 0; //yellow
 
// We'll also declare a global variable for the data we're
// sending to the shift register:
 
byte data = 0;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

int number = 0;
//int counter = 0;

const int LED = 10;
#define LED_BUILTIN 10

void setup() {
  Serial.begin(115200);  //Initialize serial
 M5.begin();
  M5.IMU.Init();
   delay(1000);
   M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(80, 15);
  M5.Lcd.println("IMU TEST");
  M5.Lcd.setCursor(30, 30);
  M5.Lcd.println("  X       Y       Z");
  M5.Lcd.setCursor(30, 70);
  M5.Lcd.println("  Pitch   Roll    Yaw");

  // Set the three SPI pins to be outputs:
  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);  
  pinMode(latchpin, OUTPUT);
   pinMode(LED, OUTPUT);

   WiFi.begin(SECRET_SSID,SECRET_PASS);
 Serial.print("Connecting to ");
 Serial.println(SECRET_SSID);
 while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
 }
 Serial.println();
 Serial.print("Connecting to ");
 Serial.print(SECRET_SSID);

   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

   WiFi.mode(WIFI_STA);  

  

}

void loop()
{

  ///gyro 
M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
M5.IMU.getAhrsData(&pitch,&roll,&yaw);

 M5.Lcd.setCursor(30, 40);
  M5.Lcd.printf("%6.2f  %6.2f  %6.2f      ", gyroX, gyroY, gyroZ);

  M5.Lcd.setCursor(30, 80);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", pitch, roll, yaw);
  delay(100);

  if(gyroX >= 35 || gyroX <= -35 ){
    if(gyroY >= 35 || gyroY <= -35 ){
      if(gyroY >= 35 || gyroY <= -35 ){
        if(pitch >= 10 || pitch <= -10 || roll >= 100 || roll <= 65 ){
      digitalWrite(LED, LOW); // turn the LED on 
      delay(1000);
      digitalWrite(LED, HIGH); // turn the LED off
      
      number++;
        if(number >= 10){
          number = 0;
          }// all five ending brackets end the series of if statements for gyro axis, pitch, roll, and yaw conditions
         }
        }
       }
      }
      else{
      digitalWrite(LED, HIGH); // turn the LED off
            }// end of else statement      
/// end of gyro 

  
 /// name badge 
  int index;
  int delayTime = 50; // time (milliseconds) to pause between LEDs
                       // make this smaller for faster switching
 
  // step through the LEDs, from 0 to 7
 
  for(index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH);    // turn LED on
    delay(delayTime);       // pause to slow down the sequence
    shiftWrite(index, LOW); // turn LED off
  }
 
  // step through the LEDs, from 7 to 0
 
  for(index = 7; index >= 0; index--)
  {
    shiftWrite(index, HIGH);    // turn LED on
    delay(delayTime);       // pause to slow down the sequence
    shiftWrite(index, LOW); // turn LED off
  }
  /////////////end of larson scan name badge

  M5.update();
  if(number >=9){
    makeIFTTTRequest2();  
    M5.Lcd.print("number is at 5");
  }///// end of IFTTT

  

}//end of void loop




////IFTTT functions
void makeIFTTTRequest2(){
  Serial.println("Connecting to " + String(server));
  HTTPClient http;
  http.begin(String("https://")+server+resource);

  int response = http.GET();
  if(response > 0){
    Serial.println("Response: " + String(response));
    Serial.println("Response body: " + http.getString());
  }
  else{
    Serial.println("Request failed");
  }
  http.end();
}//////end of IFTTT functions


 //// larson scan name badge functions
void shiftWrite(int desiredPin, boolean desiredState)
 
{
  // First we'll alter the global variable "data," changing the
  // desired bit to 1 or 0:
 
  bitWrite(data,desiredPin,desiredState);
 
  // Now we'll actually send that data to the shift register.
  // The shiftOut() function does all the hard work of
  // manipulating the data and clock pins to move the data
  // into the shift register:
 
  shiftOut(datapin, clockpin, MSBFIRST, data);
 
  // Once the data is in the shift register, we still need to
  // make it appear at the outputs. We'll toggle the state of
  // the latchPin, which will signal the shift register to "latch"
  // the data to the outputs. (Latch activates on the high-to
  // -low transition).
 
  digitalWrite(latchpin, HIGH);
  digitalWrite(latchpin, LOW);
}//// end of name badge functions
