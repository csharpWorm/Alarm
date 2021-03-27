#include "Arduino.h"
#include "sensor.h"

Sensor::Sensor(){}
Sensor::Sensor(String sensorName, int pin, String mqttDiscoveryPrefix, String mqtt_node, PubSubClient *mqttClient)
{
  _sensorName = sensorName;
  _mqttSensorName = sensorName;
  _mqttSensorName.replace(" ","_");
  _mqttSensorName.toLowerCase();
  _pin = pin;
  _mqttDiscoveryPrefix = mqttDiscoveryPrefix;
  _mqtt_node = mqtt_node;
  _mqttClient = mqttClient;
}

String Sensor::getStateTopic()
{
  // Should be defined in child class
  return "";
}

String Sensor::getConfigTopic()
{
  // Should be defined in child class
  return "";
}

String Sensor::getConfigPayload()
{
  // Should be defined in child class
  return "";
}

String Sensor::getCommandTopic()
{
  // Should be defined in child class
  return "";
}

int Sensor::getSensorValue()
{
  return analogRead(_pin);
}