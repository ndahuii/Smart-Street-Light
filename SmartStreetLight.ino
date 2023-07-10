#define BLYNK_TEMPLATE_ID "TMPL6AWVgXhjf"
#define BLYNK_TEMPLATE_NAME "ESP32 AUTOMATION"
#define BLYNK_AUTH_TOKEN "wMK63dcerreIatpK0PjLvmXNJvJZ6ZG_"

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Tnovriza";
char pass[] = "Radela25";

bool fetch_blynk_state = true;  //true or false

//Update the HEX code of IR Remote buttons 0x<HEX CODE>

//#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <AceButton.h>
using namespace ace_button;

// define the GPIO connected with Relays and switches
#define RelayPin1 13  //D13

#define SwitchPin1 14  //D14

#define wifiLed   2   //D2

//Change the virtual pins according the rooms
#define VPIN_BUTTON_1    V1 

#define VPIN_BUTTON_C    V5


// Relay State
bool toggleState_1 = LOW; //Define integer to remember the toggle state for relay

int wifiFlag = 0;

char auth[] = BLYNK_AUTH_TOKEN;

ButtonConfig config1;
AceButton button1(&config1);

void handleEvent1(AceButton*, uint8_t, uint8_t);

BlynkTimer timer;

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, !toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_C) {
  all_SwitchOff();
}

void all_SwitchOff(){
  toggleState_1 = 0; digitalWrite(RelayPin1, HIGH); Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); delay(100);
}

void checkBlynkStatus() { // called every 3 seconds by SimpleTimer

  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, LOW);
  }
  if (isconnected == true) {
    wifiFlag = 0;
    if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    }
    digitalWrite(wifiLed, HIGH);
    //Serial.println("Blynk Connected");
  }
}

BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state){
    Blynk.syncVirtual(VPIN_BUTTON_1);
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(RelayPin1, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, !toggleState_1);

  digitalWrite(wifiLed, LOW);

  config1.setEventHandler(button1Handler);
  
  button1.init(SwitchPin1);

  //Blynk.begin(auth, ssid, pass);
  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
  
  if (!fetch_blynk_state){
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
  }
}

void loop()
{ 
  
  Blynk.run();
  timer.run(); // Initiates SimpleTimer

  button1.check();
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventReleased:
      digitalWrite(RelayPin1, toggleState_1);
      toggleState_1 = !toggleState_1;
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
      break;
  }
}