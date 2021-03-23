#include<PubSubClient.h>
#include "sensor.h"

class Switch : public Sensor
{
  private:
    void* _pinMode;
    void initialize();

  public:
    Switch(String sensorName, int pin, void *pin_mode, String mqttDiscoveryPrefix, String mqtt_node, PubSubClient *mqttClient);
    Switch();
    String getConfigTopic();
    String getStateTopic();
    String getConfigPayload();
    String getCommandTopic();
    void mqtt_publish();
    boolean mqtt_publish_config();
    boolean mqtt_publish_state();
    boolean mqtt_subscribe_command();
    void turn_on();
    void turn_off();
    String getBinaryState();
    String currentState;
};
