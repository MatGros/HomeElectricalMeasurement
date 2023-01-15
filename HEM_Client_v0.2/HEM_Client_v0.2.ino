//************************************************************
// this is a simple example that uses the painlessMesh library to 
// setup a node that logs to a central logging node
// The logServer example shows how to configure the central logging nodes
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

const int D1_LED1 = 5;
const int D2_LED2 = 4;

// Déclaration des variables globales
int ClientAlive=0;

// Déclaration des variables reçus du serveur
bool ServerAlive=0;
int LDRValue = 0;
int ConsoInstVA = 0;
int maxConsoInstVA = 0;
int WhPer15Minute = 0;
int WhPerHour = 0;

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;

// Prototype
void receivedCallback( uint32_t from, String &msg );

size_t logServerId = 0;

// LED blinking function
void DigitalBlink(int GPIO){
  if (digitalRead(GPIO)==1) {      
  digitalWrite(GPIO,0);
  }
  else  {
  digitalWrite(GPIO,1); 
  }    
}

// Send message to the logServer every 10 seconds 
Task myLoggingTask(2000, TASK_FOREVER, []() {

//DigitalBlink(LED_BUILTIN);
      
#if ARDUINOJSON_VERSION_MAJOR==6
        DynamicJsonDocument jsonBuffer(1024);
        JsonObject msg = jsonBuffer.to<JsonObject>();
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& msg = jsonBuffer.createObject();
#endif
    msg["topic"] = "logClient";
    ClientAlive = not ClientAlive;
    msg["ClientAlive"] = ClientAlive;
    msg["nodeId"] = mesh.getNodeId();
    
    String str;
#if ARDUINOJSON_VERSION_MAJOR==6
    serializeJson(msg, str);
#else
    msg.printTo(str);
#endif
    if (logServerId == 0) // If we don't know the logServer yet
        mesh.sendBroadcast(str);
    else
        mesh.sendSingle(logServerId, str);

    // log to serial
#if ARDUINOJSON_VERSION_MAJOR==6
    serializeJson(msg, Serial);
#else
    msg.printTo(Serial);
#endif
    Serial.printf("\n");
});

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize GPIO2 pin as an output
  pinMode(D1_LED1, OUTPUT);    
  pinMode(D2_LED2, OUTPUT); 
  
      
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages
  // mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 11 ); // dernier chiffre est le canal wifi
  mesh.onReceive(&receivedCallback);

  // Add the task to the your scheduler
  userScheduler.addTask(myLoggingTask);
  myLoggingTask.enable();

  // Est censé augmenter la puissance... sans vraiment d'effet
  system_phy_set_max_tpw(82);
  WiFi.setOutputPower(20.5); // this sets wifi to highest power
  

}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}

void receivedCallback( uint32_t from, String &msg ) {  
  Serial.printf("logClient: Received from %u msg=%s\n", from, msg.c_str());
  
  // Clignotement à chaque réception
  DigitalBlink(LED_BUILTIN);

  // Saving logServer
#if ARDUINOJSON_VERSION_MAJOR==6
  //Serial.println("6");
  DynamicJsonDocument jsonBuffer(1024 + msg.length());
  DeserializationError error = deserializeJson(jsonBuffer, msg);
  if (error) {
    Serial.printf("DeserializationError\n");
    return;
  }
  JsonObject root = jsonBuffer.as<JsonObject>();
#else
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(msg);
#endif
  //Serial.println("endif");
  if (root.containsKey("topic")) {
      if (String("logServer").equals(root["topic"].as<String>())) {
          // check for on: true or false
          ServerAlive = root["ServerAlive"];
          logServerId = root["nodeId"];
          LDRValue = root["LDRValue"];
          ConsoInstVA = root["ConsoInstVA"];
          maxConsoInstVA = root["maxConsoInstVA"];
          WhPer15Minute = root["WhPer15Minute"];
          WhPerHour = root["WhPerHour"];
          
          // Serial.printf("logServer detected!!!\n");
          // Serial.println("value1=" + String(value1));
          // Serial.println("ConsoInstVA=" + String(ConsoInstVA));
      }
      Serial.printf("Handled from %u msg=%s\n", from, msg.c_str());       
      
  }
}
