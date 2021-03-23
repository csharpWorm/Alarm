#include "sensor.h"

class MoistureSensor : public Sensor
{
  private:
    int _airValue;   //you need to replace this value with Value_1
    int _waterValue;  //you need to replace this value with Value_2
    double getMoisturePercent();
  public:
    MoistureSensor(String sensorName, int pin, int airValue, int waterValue, String mqttDiscoveryPrefix, String mqtt_node, PubSubClient *mqttClient);
    MoistureSensor();
    String getConfigTopic();
    String getStateTopic();
    String getConfigPayload();
    int getValue();
    void mqtt_publish();
    boolean mqtt_publish_config();
    boolean mqtt_publish_state();
    boolean mqtt_subscribe_command();
    int currentValue;
};
