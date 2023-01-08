//************************************************************
// this is a simple example that uses the painlessMesh library to 
// setup a node that logs to a central logging node
// The logServer example shows how to configure the central logging nodes
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

// Déclaration des variables globales
int ClientAlive=0;
int value1=0;
int value2=0;

// Déclaration des variables reçus du serveur
float ConsoInstVA = 0;

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;

// Prototype
void receivedCallback( uint32_t from, String &msg );

size_t logServerId = 0;

// Send message to the logServer every 10 seconds 
Task myLoggingTask(10000, TASK_FOREVER, []() {
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
//    msg["value1"] = random(0, 10000);
//    msg["value2"] = random(0, 10000);
//    msg["value3"] = random(0, 10000);
//    msg["value4"] = random(0, 10000);
//    msg["value5"] = random(0, 10000);
//    msg["value6"] = random(0, 10000);
//    msg["value7"] = random(0, 10000);
//    msg["value8"] = random(0, 10000);
//    msg["value9"] = random(0, 10000);
//    msg["value10"] = random(0, 10000);
    
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
    
  //mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.onReceive(&receivedCallback);

  // Add the task to the your scheduler
  userScheduler.addTask(myLoggingTask);
  myLoggingTask.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("logClient: Received from %u msg=%s\n", from, msg.c_str());

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
          value1 = root["value1"];
          ConsoInstVA = root["ConsoInstVA"];
          logServerId = root["nodeId"];
          Serial.printf("logServer detected!!!\n");
          Serial.println("value1=" + String(value1));
          Serial.println("ConsoInstVA=" + String(ConsoInstVA));
      }
      Serial.printf("Handled from %u msg=%s\n", from, msg.c_str());
  }
}
