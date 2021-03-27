#ifndef SENSOR_H
#define SENSOR_H

#include<PubSubClient.h>

class Sensor
{
  protected:

    String _sensorName;
    String _mqttSensorName;
    String _mqttDiscoveryPrefix;
    String _mqtt_node;
    int _pin;
    PubSubClient* _mqttClient;

  public:
    Sensor(String sensorName, int pin, String mqttDiscoveryPrefix, String mqtt_node, PubSubClient *mqttClient);
    Sensor();
    virtual String getConfigTopic();
    virtual String getStateTopic();
    virtual String getConfigPayload();
    virtual String getCommandTopic();
    int getSensorValue();
    int pin(){return _pin;}
    //virtual void mqtt_publish();
    //virtual boolean mqtt_publish_config();
    //virtual boolean mqtt_publish_state();
    //virtual boolean mqtt_subscribe_command();
    //virtual String getBinaryState();
};

#endif
