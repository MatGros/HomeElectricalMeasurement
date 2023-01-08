//************************************************************
// this is a simple example that uses the painlessMesh library to 
// setup a single node (this node) as a logging node
// The logClient example shows how to configure the other nodes
// to log to this server
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

// Witty cloud HW config  
#define LDR A0 // Light Dependent Resistor
#define BUTTON = 4;
#define RED = 15;
#define GREEN = 12;
#define BLUE = 13;
#define ANALOG_LED_ON = 1024;
#define ANALOG_LED_OFF = 0;

// Déclaration des variables globales
int ServerAlive=0;
int OldMillis = millis();
bool FlagMillis = 0;
int PulseTime = 0;

// Déclaration des variables envoyées au client
float ConsoInstVA = 0;

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;
// Prototype
void receivedCallback( uint32_t from, String &msg );

// Tache personnalisée pour scruter les changements d'état du capteur LDR
Task PulseTimeTask(10, TASK_FOREVER, []() {
  //Serial.println("PulseTimeTask");
  
  // Code executé tous les cycles
  // Serial.println(millis());
  if (analogRead(LDR)>100)
  {
    // Serial.println("LDR = 1");

    if (FlagMillis==0)
    {
      PulseTime = millis()-OldMillis;
      ConsoInstVA = 3600 / (float(PulseTime)/1000);
      Serial.println("ConsoInstVA=" + String(ConsoInstVA)+ ";PulseTime=" + String(PulseTime));
      OldMillis = millis();
      FlagMillis = 1;
    }
  }
  else
  {
    // Serial.println("LDR = 0");
    FlagMillis = 0;
  }

});


// Send my ID every 10 (10000) seconds to inform others
Task logServerTask(2000, TASK_FOREVER, []() {

#if ARDUINOJSON_VERSION_MAJOR==6
        DynamicJsonDocument jsonBuffer(1024);
        JsonObject msg = jsonBuffer.to<JsonObject>();
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& msg = jsonBuffer.createObject();
#endif
    msg["topic"] = "logServer"; // Ne pas supprimer sinon erreur lors de la deserializeJson
    ServerAlive = not ServerAlive;
    msg["ServerAlive"] = ServerAlive;
    msg["nodeId"] = mesh.getNodeId();
    msg["value1"] = random(0, 10000);
    msg["ConsoInstVA"] = int(ConsoInstVA);
    

    String str;
#if ARDUINOJSON_VERSION_MAJOR==6
    serializeJson(msg, str);
#else
    msg.printTo(str);
#endif
    mesh.sendBroadcast(str);

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
    
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE | DEBUG ); // all types on
  //mesh.setDebugMsgTypes( ERROR | CONNECTION | SYNC | S_TIME );  // set before init() so that you can see startup messages
  // mesh.setDebugMsgTypes( ERROR | CONNECTION | S_TIME );  // set before init() so that you can see startup messages
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION);  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.onReceive(&receivedCallback);

  mesh.onNewConnection([](size_t nodeId) {
    Serial.printf("New Connection %u\n", nodeId);
  });

  mesh.onDroppedConnection([](size_t nodeId) {
    Serial.printf("Dropped Connection %u\n", nodeId);
  });

  // Add the task to the your scheduler
  userScheduler.addTask(logServerTask);
  logServerTask.enable();
  userScheduler.addTask(PulseTimeTask);
  PulseTimeTask.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  
//  // Code executé tous les cycles
//  // Serial.println(millis());
//  if (analogRead(LDR)>100)
//  {
//    // Serial.println("LDR = 1");
//
//    if (FlagMillis==0)
//    {
//      PulseTime = millis()-OldMillis;
//      ConsoInstVA = 3600 / (float(PulseTime)/1000);
//      Serial.println("ConsoInstVA=" + String(ConsoInstVA)+ ";PulseTime=" + String(PulseTime));
//      OldMillis = millis();
//      FlagMillis = 1;
//    }
//  }
//  else
//  {
//    // Serial.println("LDR = 0");
//    FlagMillis = 0;
//  }
  
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("logServer: Received from %u msg=%s\n", from, msg.c_str());
}
