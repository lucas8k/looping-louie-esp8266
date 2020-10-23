#include <ESP8266WiFi.h>        // Include the Wi-Fi library

const char *ssid = "Looping Louie Access Point"; // The name of the Wi-Fi network that will be created
const char *password = "corona";   // The password required to connect to it, leave blank for an open network

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

uint8_t Pwm1 = D1; //Nodemcu PWM pin 
uint8_t Pwm2 = D2; //Nodemcu PWM pin

//Seven segment pins attachecd with nodemcu pins  
int a0 = 15;  //Gpio-15 of nodemcu esp8266  
int a1 = 13;  //Gpio-13 of nodemcu esp8266    
int a2 = 12;  //Gpio-12 of nodemcu esp8266   
int a3 = 14;  //Gpio-14 of nodemcu esp8266   

bool motorRunning = false;

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(a0, OUTPUT);     
  pinMode(a1, OUTPUT);     
  pinMode(a2, OUTPUT);
  pinMode(a3, OUTPUT);   
  Serial.println('\n');

  WiFi.softAP(ssid, password);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
  delay(1000);
    // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
    // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

 // Match the request for index
 if (request.indexOf("/index") != -1)  {  
    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML><html><head> <title>Looping Louie on Steroids</title> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <style>.slidecontainer{width: 100%;}.slider{-webkit-appearance: none; width: 100%; height: 15px; border-radius: 5px; background: #d3d3d3; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s;}.slider:hover{opacity: 1;}.slider::-webkit-slider-thumb{-webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 50%; background: #4CAF50; cursor: pointer;}.slider::-moz-range-thumb{width: 25px; height: 25px; border-radius: 50%; background: #4CAF50; cursor: pointer;}</style></head><h1 align=center>Looping Louie on Steroids</h1><h1 align=center>Remote Control Panel</h1><br><div> <h2>Current State:</h2> <p>Control Type: Manual</p><p>Motor Speed: <span id=\"demo\"></span>%</p><p>Direction: Forward</p></div><div class=\"slidecontainer\"> <input type=\"range\" min=\"50\" max=\"100\" value=\"100\" step=\"5\" class=\"slider\" id=\"myRange\"></div><div class=\"slidecontainer\"> <button type=\"button\" onclick=\"startMotor()\">Motor On</button> <button type=\"button\" onclick=\"stopMotor()\">Motor Off</button> <button type=\"button\" onclick=\"changeDirection()\">Change Direction</button></div><script>var slider=document.getElementById(\"myRange\"); var output=document.getElementById(\"demo\"); output.innerHTML=slider.value; slider.onchange=function (){output.innerHTML=this.value; if (window.XMLHttpRequest){xhttp=new XMLHttpRequest();}else{xhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");}xhttp.open(\"GET\", \"/change_speed?speed=\" + this.value, false); xhttp.send(); console.log(xhttp.responseText);}; function startMotor(){console.log(\"Start Motor\"); if (window.XMLHttpRequest){xhttp=new XMLHttpRequest();}else{xhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");}xhttp.open(\"GET\", \"/start_motor\", false); xhttp.send(); console.log(xhttp.responseText);}; function stopMotor(){if (window.XMLHttpRequest){xhttp=new XMLHttpRequest();}else{xhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");}xhttp.open(\"GET\", \"/stop_motor\", false); xhttp.send(); console.log(xhttp.responseText);}; function changeDirection(){if (window.XMLHttpRequest){xhttp=new XMLHttpRequest();}else{xhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");}xhttp.open(\"GET\", \"/change_direction\", false); xhttp.send(); console.log(xhttp.responseText);}; </script><body></body></html>");
    delay(1);
    Serial.println("Client disonnected");
    Serial.println("");
 }

  // Match the request for motor start
 if (request.indexOf("/start_motor") != -1)  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println(""); //  do not forget this one
    if (!motorRunning) {
      Serial.println("Starting Motor"); 
      digitalWrite(a0, HIGH); //Start  motor
      digitalWrite(a1, LOW);
    
      digitalWrite(a2, HIGH); //Start  motor
      digitalWrite(a3, LOW);
      motorRunning = true;
      client.println("{status: 1}");
    } else {
      Serial.println("Motor is already running"); 
      client.println("{status: 0}");
    }
 }

   // Match the request for motor stop
 if (request.indexOf("/stop_motor") != -1)  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println(""); //  do not forget this one 
    if (motorRunning) {
      Serial.println("Stopping Motor");
      digitalWrite(a0, LOW); //Stop  motor
      digitalWrite(a1, LOW);
      digitalWrite(a2, LOW); //Stop  motor
      digitalWrite(a3, LOW);
      motorRunning = false;
      client.println("{status: 1}");
    } else {
      Serial.println("Motor is already stopped"); 
      client.println("{status: 0}");
    }
 }

    // Match the request for motor stop
 if (request.indexOf("/stop_motor") != -1)  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println(""); //  do not forget this one 
    if (motorRunning) {
      Serial.println("Stopping Motor");
      digitalWrite(a0, LOW); //Stop  motor
      digitalWrite(a1, LOW);
      digitalWrite(a2, LOW); //Stop  motor
      digitalWrite(a3, LOW);
      motorRunning = false;
      client.println("{status: 1}");
    } else {
      Serial.println("Motor is already stopped"); 
      client.println("{status: 0}");
    }
    
 }

  if (request.indexOf("/change_direction") != -1)  {
    Serial.println("Change direction"); 
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println(""); //  do not forget this one 
    digitalWrite(a0, LOW);  //Change First motor rotation direction
    digitalWrite(a1, HIGH);
    
    digitalWrite(a2, LOW); //Change Second motor rotation direction
    digitalWrite(a3, HIGH);
    client.println("{status: 1}");
  } 

  if (request.indexOf("/change_speed?speed=") != -1)  {
    int speed = request.substring(24, request.indexOf("HTTP")).toInt();
    Serial.println("Change Speed to " + speed); 
    analogWrite(Pwm1, speed * 10);  //Pwm duty cycle
    analogWrite(Pwm2, speed * 10);  //Pwm duty cycle
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println(""); //  do not forget this one 
    
    client.println("{status: 1}");
  } 
  
}
