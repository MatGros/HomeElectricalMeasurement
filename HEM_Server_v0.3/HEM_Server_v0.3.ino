//************************************************************
// this is a simple example that uses the painlessMesh library to 
// setup a single node (this node) as a logging node
// The logClient example shows how to configure the other nodes
// to log to this server
//************************************************************
#include <Arduino.h>
#include <painlessMesh.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

// MQTT
#define   STATION_SSID     "Freebox_BSHOQE"
#define   STATION_PASSWORD "?"
#define HOSTNAME "MQTT_Bridge"
#define   MQTT_USER     "admin"
#define   MQTT_PASSWORD "?"

// Witty cloud HW config  
#define LDR A0 // Light Dependent Resistor
#define BUTTON = 4;
const int RED = 15;
const int GREEN = 12;
const int BLUE = 13;
#define ANALOG_LED_ON = 1024;
#define ANALOG_LED_OFF = 0;

// Déclaration des variables globales
bool ServerAlive=0;
int LDRValue = 0;
int OldMillis = millis();
bool FlagMillis = 0;
int PulseTime = 0;
float ConsoInstVA = 0;
int WhPer15Minute = 0;
int cptCalculationTask = 0;
int WhPerHour = 0;
int maxConsoInstVA = 0;

bool FlagClientAlive=0;

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;

// Prototype
void receivedCallback( uint32_t from, String &msg );
// MQTT
void mqttCallback(char* topic, byte* payload, unsigned int length);

IPAddress getlocalIP();
IPAddress myIP(0,0,0,0);
IPAddress mqttBroker(192, 168, 1, 42);
WiFiClient wifiClient;
PubSubClient mqttClient(mqttBroker, 1883, mqttCallback, wifiClient);

//Fonction pour publier un float sur un topic 
void mqtt_publish(String topic, float t){
  char top[topic.length()+1];
  topic.toCharArray(top,topic.length()+1);
  char t_char[50];
  String t_str = String(t);
  t_str.toCharArray(t_char, t_str.length() + 1);
  mqttClient.publish(top,t_char);
}

// Tache personnalisée pour scruter les changements d'état du capteur LDR
Task PulseTimeTask(10, TASK_FOREVER, []() {
  //Serial.println("PulseTimeTask");

  // Code executé tous les cycles
  // Serial.println(millis());
  LDRValue = int(analogRead(LDR));
  if (LDRValue>100)
  {
    // Serial.println("LDR = 1");
    

    if (FlagMillis==0)
    {   
      digitalWrite(LED_BUILTIN,0);
      
      PulseTime = millis()-OldMillis;
      ConsoInstVA = 3600 / (float(PulseTime)/1000);
      // Serial.println("PulseTime=" + String(PulseTime) + " ; ConsoInstVA=" + String(ConsoInstVA));
      OldMillis = millis();
      FlagMillis = 1;

      WhPer15Minute = WhPer15Minute + 1 ;
      WhPerHour = WhPerHour + 1 ;
      if (ConsoInstVA>maxConsoInstVA){
        // Serial.println("New ► maxConsoInstVA=" + String(maxConsoInstVA));
        maxConsoInstVA = ConsoInstVA;
      }  

      Serial.println("PulseTime=" + String(PulseTime) + " ; ConsoInstVA=" + String(ConsoInstVA) + " ; maxConsoInstVA=" + String(maxConsoInstVA));

    }
  }
  else
  {
    // Serial.println("LDR = 0");
    digitalWrite(LED_BUILTIN,1);
    FlagMillis = 0;
  }
});

// Tache personnalisée pour par exemple cacluler un moyenne toutes les 15min
Task CalculationTask(15*60*1000, TASK_FOREVER, []() {
// Task CalculationTask(15*60*1000/4, TASK_FOREVER, []() {
  // Serial.println("CalculationTask"); 
  Serial.println("WhPer15Minute");
  Serial.println(WhPer15Minute); 
  WhPer15Minute = 0;
  
  cptCalculationTask = cptCalculationTask + 1;
  
  if (cptCalculationTask >= 4) {
    Serial.println("WhPerHour");
    Serial.println(WhPerHour); 
    WhPerHour = 0;

    Serial.println("maxConsoInstVA");
    Serial.println(maxConsoInstVA); 
    maxConsoInstVA = 0;      
    cptCalculationTask = 0;
  }

});

// Send my ID every 10 (10000) seconds to inform others
Task logServerTask(10000, TASK_FOREVER, []() {

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
    msg["LDR"] = int(LDRValue);
    msg["ConsoInstVA"] = int(ConsoInstVA);  
    msg["maxConsoInstVA"] = int(maxConsoInstVA); 
    msg["WhPer15Minute"] = int(WhPer15Minute);
    msg["WhPerHour"] = int(WhPerHour); 
    
    mqtt_publish("linky/ServerAlive", float(ServerAlive));
    mqtt_publish("linky/LDR", LDR);
    mqtt_publish("linky/ConsoInstVA", ConsoInstVA);
    mqtt_publish("linky/maxConsoInstVA", maxConsoInstVA);
    mqtt_publish("linky/WhPer15Minute", WhPer15Minute);
    mqtt_publish("linky/WhPer15Minute", WhPer15Minute);
    mqtt_publish("linky/WhPerHour", WhPerHour);
    

    float temp = random(30);
    mqtt_publish("outTopic", temp);
    
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

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("arduinoClient", "admin", "14258")) {
    // if (mqttClient.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqttClient.publish("outTopic","hello world");
      // ... and resubscribe
      mqttClient.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize GPIO2 pin as an output
  digitalWrite(LED_BUILTIN, 1);

  Serial.begin(115200);
    
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE | DEBUG ); // all types on
  //mesh.setDebugMsgTypes( ERROR | CONNECTION | SYNC | S_TIME );  // set before init() so that you can see startup messages
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION);  // set before init() so that you can see startup messages
  // mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 11 ); // dernier chiffre est le canal wifi
  mesh.onReceive(&receivedCallback);

  // MQTT
  mesh.stationManual(STATION_SSID, STATION_PASSWORD);  
  mesh.setHostname(HOSTNAME);
  // Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
  mesh.setRoot(true);
  // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
  mesh.setContainsRoot(true);

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
  userScheduler.addTask(CalculationTask);
  CalculationTask.enable();

  system_phy_set_max_tpw(82);
  WiFi.setOutputPower(20.5); // this sets wifi to highest power

  digitalWrite(LED_BUILTIN, 0);
  delay(1000);
  digitalWrite(LED_BUILTIN, 1);
  delay(1000);
  digitalWrite(LED_BUILTIN, 0);  
}

void loop() {

  // it will run the user scheduler as well
  mesh.update();

  // if (mqttClient.connect("arduinoClient", "admin", "$7$101$OejVw1BFwL24AMKp$IVn5TveApHHkS1RDpMrQYW5jgknNmivl5rTWMeFeKU/X6r9iX2Q8zQ3OqrtgdJ83Y51jkXskbwn9geZCVkk1wA==")) {
  //   Serial.println("MQTT ► OK");    
  // }
  
  // MQTT
  mqttClient.loop();

  // if (mqttClient.connect("arduinoClient", "admin", "147258")) {
  //   mqttClient.publish("outTopic","hello world");
  //   mqttClient.subscribe("inTopic");
  // } 
  
  // MQTT
  // mqttClient.publish("Linky/ConsoInstVA","123456");
  // mqttClient.subscribe("Linky/ConsoInstVA");

  // Attente d'avoir une IP pour pouvoir espérer ouvrir un connection au serveur MQTT
  if(myIP != getlocalIP()){
    myIP = getlocalIP();
    Serial.println("My IP is " + myIP.toString());

    if (mqttClient.connect("painlessMeshClient","admin","147258")) {
      mqttClient.publish("painlessMesh/from/gateway","Ready!");
      //mqttClient.subscribe("painlessMesh/to/#"); 
      //mqttClient.subscribe("painlessMesh/from/gateway");     
    } 
  }  

}

void receivedCallback( uint32_t from, String &msg ) {
  // Serial.printf("logServer: Received from %u msg=%s\n", from, msg.c_str());

  if (FlagClientAlive==1) {
    analogWrite(BLUE,512);  
    FlagClientAlive = 0;
  }
  else {
    analogWrite(BLUE,0);  
    FlagClientAlive = 1 ;
  }  

  
  
}

void mqttCallback(char* topic, uint8_t* payload, unsigned int length) {
  char* cleanPayload = (char*)malloc(length+1);
  memcpy(cleanPayload, payload, length);
  cleanPayload[length] = '\0';
  String msg = String(cleanPayload);
  free(cleanPayload);

  String targetStr = String(topic).substring(16);

  // Serial.printf("mc topic:%s  payload:%s\n", topic, payload); // oklessMesh/from/gateway
  Serial.printf("mc topic:%s\n", topic); // oklessMesh/from/gateway
  Serial.printf("mc payload:%s\n", payload); // oklessMesh/from/gateway

  if(targetStr == "gateway")
  {
    Serial.println("gateway");
    if(msg == "getNodes")
    {
      Serial.println("getNodes");
      auto nodes = mesh.getNodeList(true);
      String str;
      for (auto &&id : nodes)
        str += String(id) + String(" ");
      mqttClient.publish("painlessMesh/from/gateway", str.c_str());
    }
  }
  else if(targetStr == "broadcast") 
  {
    Serial.println("broadcast");
    mesh.sendBroadcast(msg);
  }
  else
  {
    uint32_t target = strtoul(targetStr.c_str(), NULL, 10);
    Serial.println("target");    
    Serial.println(String(target)); 
    if(mesh.isConnected(target))
    {
      Serial.println("mesh.isConnecte");
      mesh.sendSingle(target, msg);
    }
    else
    {
      Serial.println("mesh.NotConnect");
      mqttClient.publish("painlessMesh/from/gateway", "Client not connected!");
    }
  }
}

IPAddress getlocalIP() {
  return IPAddress(mesh.getStationIP());
}
