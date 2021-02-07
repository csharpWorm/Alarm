#include<PubSubClient.h>
class Switch
{
  private:

    String _switchName;
    String _mqttSwitchName;
    String _mqttDiscoveryPrefix;
    String _mqtt_node;
    int _pin;
    void* _pinMode;
    PubSubClient* _mqttClient;
    void initialize();

  public:
    Switch(String switchName, int pin, void *pin_mode, String mqttDiscoveryPrefix, String mqtt_node, PubSubClient *mqttClient);
    Switch();
    String getConfigTopic();
    String getStateTopic();
    String getConfigPayload();
    String getCommandTopic();
    int pin(){return _pin;}
    void mqtt_publish();
    boolean mqtt_publish_config();
    boolean mqtt_publish_state();
    boolean mqtt_subscribe_command();
    void turn_on();
    void turn_off();
    String getBinaryState();
    String currentState;
};
