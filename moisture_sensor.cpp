///////////////////////////////////////
// Known to work with DFRobotics     //
// Capacitive analog moisture sensor //
// SKU:SEN0193 and SEN0308           //
///////////////////////////////////////
#include "Arduino.h"
#include "moisture_sensor.h"

MoistureSensor::MoistureSensor() : Sensor(){}
MoistureSensor::MoistureSensor(String sensorName, int pin, int airValue, int waterValue, String mqttDiscoveryPrefix, String mqtt_node, PubSubClient *mqttClient) : Sensor(sensorName, pin, mqttDiscoveryPrefix, mqtt_node, mqttClient)
{
  _airValue = airValue;
  _waterValue = waterValue;
//  initialize();
}

String MoistureSensor::getStateTopic()
{
  return _mqttDiscoveryPrefix + "/sensor/" + _mqtt_node + "_" + _mqttSensorName + "/state";
}

String MoistureSensor::getConfigTopic()
{
  return _mqttDiscoveryPrefix + "/sensor/" + _mqtt_node + "_" + _mqttSensorName + "/config";
}

String MoistureSensor::getConfigPayload()
{
  return "{\"device_class\": \"humidity\", \"name\": \"" + _mqtt_node + "_" + _mqttSensorName + "\", \"state_topic\": \"" + getStateTopic() + "\", \"unit_of_measurement\": \"%\", \"value_template\": \"{{ value_json.moisture}}\" }";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              HELPERS                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
double MoistureSensor::getMoisturePercent()
{
  return 100 - map(getSensorValue(), _waterValue, _airValue, 0, 100);
}

void MoistureSensor::mqtt_publish()
{
  // publish MQTT discovery topics and device state
  boolean config_publish = mqtt_publish_config();
  Serial.println("Configuration Topic Published: " + config_publish);
  delay(200);
  boolean state_publish = mqtt_publish_state();
  Serial.println("State Topic Published: " + state_publish);
}

boolean MoistureSensor::mqtt_publish_config()
{
  Serial.println("MQTT discovery " + _sensorName + " config: [" + getConfigTopic() + "] : [" + getConfigPayload() + "]");
  return _mqttClient->publish(getConfigTopic().c_str(), getConfigPayload().c_str(), true);
}
boolean MoistureSensor::mqtt_publish_state()
{
  // publish MQTT discovery topics and device state
  Serial.println("MQTT discovery " + _sensorName + " state: [" + getStateTopic() + "] : [" + getMoisturePercent() + "]");
  currentValue = getSensorValue();
  String moisturePercent_str = "{\"moisture\": " + String(getMoisturePercent()) + "}";
  return _mqttClient->publish(getStateTopic().c_str(), moisturePercent_str.c_str());
}
