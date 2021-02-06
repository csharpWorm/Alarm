//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Inlcude Libraries                                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "binary_sensor.h"
#include "switch.h"
#include <SimpleTimer.h> // using https://github.com/marcelloromani/arduino/tree/master/SimpleTimer
#include <PubSubClient.h>
#include <Ethernet.h>
#include <SPI.h>
#include "Secrets.h"

EthernetClient ethClient;
PubSubClient client(ethClient);
SimpleTimer timer;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                             MODIFY THESE FOR YOUR ENVIRONMENT                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *mqtt_server = "192.168.2.21";              // Your MQTT server IP address
const int mqtt_port = 1883;                            // Your MQTT port
const char *mqtt_user = SECRET_MQTT_USER;              // mqtt username, set to "" for no user value is in Secrets.h
const char *mqtt_pass = SECRET_MQTT_PASSWORD;          // mqtt password, set to "" for no password value is in Secrets.h
const String mqtt_node = "Alarm";                      // Your unique hostname for this device
const String mqttDiscoveryPrefix = "homeassistant";    // Home Assistant MQTT Discovery, see https://home-assistant.io/docs/mqtt/discovery/
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x06}; // MAC address
IPAddress ip(192, 168, 2, 13);                         // Set the static IP address to use if the DHCP fails to assign
IPAddress myDns(192, 168, 2, 1);                       // Set the dns address to use if the DHCP fails to assign

//#define DEBUG_MQTT_SETUP                             // UnComment this line if you want to debug the initial mqtt publish of state, config and command messages to ensure they are being sent
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            BINARY_SENSORS                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Home Assistant MQTT Discovery
// Create a Sensor for each pin
//BinarySensor(String sensorName, int pin, void* pin_mode, String deviceClass, String mqttDiscoveryPrefix, String mqtt_node, PubSubClient *mqttClient);
BinarySensor sensors[20] = {BinarySensor("Door Front", 22, INPUT_PULLUP, "door", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Door Garage", 23, INPUT_PULLUP, "door", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Door Kitchen", 24, INPUT_PULLUP, "door", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Door Basement South", 25, INPUT_PULLUP, "door", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Door Basement North", 26, INPUT_PULLUP, "door", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Piano Room", 27, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Kitchen", 28, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Mudroom", 29, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Master", 30, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Master Bath", 31, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Theater East", 32, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Theater West", 33, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Guest Bed East", 34, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Guest Bed West", 35, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Gym", 36, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Boys", 37, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Office", 38, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Toy Room", 39, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Girls", 40, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client),
                            BinarySensor("Window Girls Bath", 41, INPUT_PULLUP, "window", mqttDiscoveryPrefix, mqtt_node, &client)
                           };
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     SETUP RELAYS AS SWITCHES                                         //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Switch(String switchName, int pin, void* pin_mode, String mqttDiscoveryPrefix, String mqtt_node, PubSubClient *mqttClient);
Switch relays[8] = {Switch("Heat Master", 42, OUTPUT, mqttDiscoveryPrefix, mqtt_node, &client),
                    Switch("Heat Kitchen", 43, OUTPUT, mqttDiscoveryPrefix, mqtt_node, &client),
                    Switch("Heat Piano", 44, OUTPUT, mqttDiscoveryPrefix, mqtt_node, &client),
                    Switch("Heat Office", 45, OUTPUT, mqttDiscoveryPrefix, mqtt_node, &client),
                    Switch("Heat Toyroom", 46, OUTPUT, mqttDiscoveryPrefix, mqtt_node, &client),
                    Switch("Heat Basement", 47, OUTPUT, mqttDiscoveryPrefix, mqtt_node, &client),
                    Switch("Heat Theater", 48, OUTPUT, mqttDiscoveryPrefix, mqtt_node, &client),
                    Switch("Heat Main AC", 49, OUTPUT, mqttDiscoveryPrefix, mqtt_node, &client)
                   };
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      MAIN (setup & loop)                                             //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Initialize Sensor pinmode
  for (int i = 0; i < (sizeof(sensors) / sizeof(sensors[0])); i++)
  {
    sensors[i].initialize();
  }

  // Initialize relay pinmode
  for (int i = 0; i < (sizeof(relays) / sizeof(relays[0])); i++)
  {
    relays[i].initialize();
  }

#ifndef DEBUG_MQTT_SETUP
  timer.setInterval(200, checkButtons);
  timer.setInterval(2000, mqtt_client_loop);
#endif
}

void loop()
{
  if (!client.connected())
  {
    mqtt_connect();
  }
  else
  {
    timer.run();
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  MQTT connection and subscriptions                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void mqtt_connect()
{
  Serial.println("Attempting MQTT connection...");
  // Attempt to connect
  if (client.connect(mqtt_node.c_str(), mqtt_user, mqtt_pass))
  {
    Serial.println("connected");
    // publish MQTT discovery topics and device state
    // The strings below will spill over the PubSubClient_MAX_PACKET_SIZE 128
    // You'll need to manually set MQTT_MAX_PACKET_SIZE in PubSubClient.h to 1024
    for (int i = 0; i < (sizeof(sensors) / sizeof(sensors[0])); i++)
    {
      sensors[i].mqtt_publish();
    }

    //Relays
    for (int i = 0; i < (sizeof(relays) / sizeof(relays[0])); i++)
    {
      relays[i].mqtt_publish();
    }
  }
  else
  {
    Serial.println("MQTT connection failed, rc=" + String(client.state()));
    Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
  }
}

void mqtt_client_loop()
{
  client.loop();
}

void callback(char* topic, byte * payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  payload[length] = '\0';
  String newPayload = String((char *)payload);
  newPayload.toLowerCase();

  for (int i = 0; i < (sizeof(relays) / sizeof(relays[0])); i++)
  {
    if (strcmp(topic, relays[i].getCommandTopic().c_str()) == 0)
    {
      if (strcmp(newPayload.c_str(), "on") == 0)
      {
        relays[i].turn_on();
        break;
      }
      else
      {
        relays[i].turn_off();
        break;
      }
    }
  }
}

void checkButtons()
{
  for (int i = 0; i < (sizeof(sensors) / sizeof(sensors[0])); i++)
  {
    // if the current state and actual state do not match publish the new state
    if (!sensors[i].getBinaryState().equalsIgnoreCase(sensors[i].currentState))
      sensors[i].mqtt_publish_state();
  }
}
