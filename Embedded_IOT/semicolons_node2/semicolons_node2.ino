/*
 Precision Farmers @ Semicolons16
 Author: Amey Pandit & Amitkumar Ghatwal
 Date: 12 March 2016
 Version: 1.0
*/

#include <SPI.h>
#include <Ethernet.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <PubSubClient.h>
#include <dht.h>
dht DHT;

//Hardware Pin Description
#define VALVE_PIN       7
#define DHT11_PIN       3

//MQTT Topic
#define VALVE_ON      "farmer/node2/valve"
#define VALVE_COM_ON  "farmer/common/valve"

#define DEBUG


// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xAB, 0xFE, 0xFE, 0xED };
byte server[] = { 10, 244, 25, 18 };
byte ip[]     = { 10, 244, 25, 19 };

//Global Variables
int mqtt_status = 0;
boolean newDataAvbl = false;
static char gTopic[20];
static char inBuf[60];

//Callback function when message is received by the client.
void callback(char* topic, byte* payload, unsigned int length) {
  
  strcpy(gTopic,topic); //Just coopy the topic and the data. Do the processing in the main loop
    
  for (int i=0;i<length;i++) 
  {
    inBuf[i] = payload[i];
  }  
  newDataAvbl = true;
}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void setup()
{
  
  Serial.begin(9600);
  delay(500);
#ifdef DEBUG
  Serial.println("Started!");
#endif

  pinMode(VALVE_PIN, OUTPUT);
  
  setTime(8,29,0,1,1,11); // set dummy time
  Alarm.timerRepeat(5, getSensorData);
  
  //Connect to Ethernet
  Ethernet.begin(mac, ip);
  delay(45000);              //Required delay for the ethernet to be up properly
  
  //Connect to MQTT client
  if (client.connect("Node_2")) {

#ifdef DEBUG
    Serial.println("MQTT connected");
#endif

    int s = client.state();
//    Serial.println(s);
    
    client.subscribe(VALVE_ON);
    client.subscribe(VALVE_COM_ON);
    
  }
  else
  {

#ifdef DEBUG
    Serial.println("MQTT NOT connected");
#endif

    int s = client.state();
    Serial.println(s);
  }
}

void loop()
{
  client.loop();  
  Alarm.delay(1000); //Update the Alarms every second
  
  if(newDataAvbl == true)
  {
        //Check the topic and start the desired actuator
        if (strcmp(gTopic, VALVE_ON) == 0)
        {
          if(inBuf[0] == '1')
          {
            digitalWrite(VALVE_PIN, HIGH);
          }
          else if(inBuf[0] == '0')
          {
            digitalWrite(VALVE_PIN, LOW);
          }
          else
          {
            
          }
        }
        else //Common Topic to turn on/off the valve
        {
           if(inBuf[0] == '1')
          {
            digitalWrite(VALVE_PIN, HIGH);
          }
          else if(inBuf[0] == '0')
          {
            digitalWrite(VALVE_PIN, LOW);
          }
          else
          {
            
          }
        }
        
        inBuf[0] = 0;
        newDataAvbl = false;
  }
  
}

static char buffer[10]; 
void getSensorData()
{
  
  //Read temperature and humidity and publish to broker
  int chk = DHT.read11(DHT11_PIN);
  
  buffer[0] = 0;
  dtostrf(DHT.humidity, 4, 2, buffer);
  client.publish("farmer/node2/humidity",buffer);
  
  buffer[0] = 0;
  dtostrf(DHT.temperature, 4, 2, buffer);
  client.publish("farmer/node2/temperature",buffer);        
}


