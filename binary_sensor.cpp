#include "Arduino.h"
#include "binary_sensor.h"

BinarySensor::BinarySensor(){}
BinarySensor::BinarySensor(String sensorName, int pin, void *pin_mode, String deviceClass, String mqttDiscoveryPrefix, String mqtt_node, PubSubClient *mqttClient)
{
  _sensorName = sensorName;
  _mqttSensorName = sensorName;
  _mqttSensorName.replace(" ","_");
  _mqttSensorName.toLowerCase();
  _pin = pin;
  _pinMode = pin_mode;
  _deviceClass = deviceClass;
  _mqttDiscoveryPrefix = mqttDiscoveryPrefix;
  _mqtt_node = mqtt_node;
  _mqttClient = mqttClient;
  
  initialize();
}

String BinarySensor::getStateTopic()
{
  return _mqttDiscoveryPrefix + "/binary_sensor/" + _mqtt_node + "_" + _mqttSensorName + "/state";
}

String BinarySensor::getConfigTopic()
{
  return _mqttDiscoveryPrefix + "/binary_sensor/" + _mqtt_node + "_" + _mqttSensorName + "/config";
}

String BinarySensor::getConfigPayload()
{
  return "{\"name\": \"" + _mqtt_node + "_" + _mqttSensorName + "\", \"device_class\": \"" + _deviceClass + "\", \"state_topic\": \"" + getStateTopic() + "\"}";
}

void BinarySensor::initialize()
{
  pinMode(_pin, _pinMode);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              HELPERS                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
String BinarySensor::getBinaryState()
{
  return digitalRead(_pin) == LOW ? "OFF" : "ON";
}

void BinarySensor::mqtt_publish()
{
  // publish MQTT discovery topics and device state
  boolean config_publish = mqtt_publish_config();
  Serial.println("Configuration Topic Published: " + config_publish);
  delay(200);
  boolean state_publish = mqtt_publish_state();
  Serial.println("Configuration Topic Published: " + state_publish);
}

boolean BinarySensor::mqtt_publish_config()
{
  Serial.println("MQTT discovery " + _sensorName + " config: [" + getConfigTopic() + "] : [" + getConfigPayload() + "]");
  return _mqttClient->publish(getConfigTopic().c_str(), getConfigPayload().c_str(), true);
}
boolean BinarySensor::mqtt_publish_state()
{
  // publish MQTT discovery topics and device state
  Serial.println("MQTT discovery " + _sensorName + " state: [" + getStateTopic() + "] : [" + getBinaryState() + "]");
  currentState = getBinaryState();
  return _mqttClient->publish(getStateTopic().c_str(), currentState.c_str());
}
