#include<PubSubClient.h>
class BinarySensor
{
  private:

    String _sensorName;
    String _mqttSensorName;
    String _mqttDiscoveryPrefix;
    String _mqtt_node;
    int _pin;
    void* _pinMode;
    String _deviceClass;
    PubSubClient* _mqttClient;

  public:
    BinarySensor(String sensorName, int pin, void *pin_mode, String deviceClass, String mqttDiscoveryPrefix, String mqtt_node, PubSubClient *mqttClient);
    BinarySensor();
    String getConfigTopic();
    String getStateTopic();
    String getConfigPayload();
    int pin(){return _pin;}
    void initialize();
    void mqtt_publish();
    boolean mqtt_publish_config();
    boolean mqtt_publish_state();
    String getBinaryState();
    String currentState;
};
