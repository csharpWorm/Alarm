#include "Arduino.h"
#include "switch.h"

Switch::Switch(){}
Switch::Switch(String switchName, int pin, void *pin_mode, String mqttDiscoveryPrefix, String mqtt_node, PubSubClient *mqttClient)
{
  _switchName = switchName;
  _mqttSwitchName = switchName;
  _mqttSwitchName.replace(" ","_");
  _mqttSwitchName.toLowerCase();
  _pin = pin;
  _mqttDiscoveryPrefix = mqttDiscoveryPrefix;
  _mqtt_node = mqtt_node;
  _mqttClient = mqttClient;
  _pinMode = pin_mode;
}

String Switch::getStateTopic()
{
  return _mqttDiscoveryPrefix + "/switch/" + _mqtt_node + "_" + _mqttSwitchName + "/state";
}

String Switch::getConfigTopic()
{
  return _mqttDiscoveryPrefix + "/switch/" + _mqtt_node + "_" + _mqttSwitchName + "/config";
}

String Switch::getConfigPayload()
{
  return "{\"name\": \"" + _mqtt_node + "_" + _mqttSwitchName + "\", \"command_topic\": \"" + getCommandTopic() + "\", \"state_topic\": \"" + getStateTopic() + "\"}";
}

String Switch::getCommandTopic()
{
  return _mqttDiscoveryPrefix + "/switch/" + _mqtt_node + "_" + _mqttSwitchName + "/set";
}

void Switch::initialize()
{
  pinMode(_pin, _pinMode);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                              HELPERS                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////
String Switch::getBinaryState()
{
  return digitalRead(_pin) == LOW ? "OFF" : "ON";
}

void Switch::mqtt_publish()
{
  // publish MQTT discovery topics and device state
  boolean config_publish = mqtt_publish_config();
  Serial.println("Configuration Topic Published: " + config_publish);
  delay(200);
  boolean state_publish = mqtt_publish_state();
  Serial.println("Configuration Topic Published: " + state_publish);
  delay(200);
  boolean command_publish = mqtt_subscribe_command();
 Serial.println("Configuration Topic Published: " + command_publish);
}

boolean Switch::mqtt_publish_config()
{
  Serial.println("MQTT discovery " + _switchName + " config: [" + getConfigTopic() + "] : [" + getConfigPayload() + "]");
  return _mqttClient->publish(getConfigTopic().c_str(), getConfigPayload().c_str(), true);
}
boolean Switch::mqtt_publish_state()
{
  // publish MQTT discovery topics and device state
  Serial.println("MQTT discovery " + _switchName + " state: [" + getStateTopic() + "] : [" + getBinaryState() + "]");
  currentState = getBinaryState();
  return _mqttClient->publish(getStateTopic().c_str(), currentState.c_str());
}

boolean Switch::mqtt_subscribe_command()
{
  Serial.println("MQTT discovery " + _switchName + " command: [" + getCommandTopic() + "]");
  return _mqttClient->subscribe(getCommandTopic().c_str());
}

void Switch::turn_on()
{
  digitalWrite(_pin, HIGH);
  Serial.println("Turning on");
  _mqttClient->publish(getStateTopic().c_str(), "ON");
}
void Switch::turn_off()
{
  digitalWrite(_pin, LOW);
  Serial.println("Turning off");
  _mqttClient->publish(getStateTopic().c_str(), "OFF");
}
