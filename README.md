# Alarm
Code written for the arduino Mega with ethernet hat.
The code will take end devices connected to the board as either switches or binary sensors and publish their information to MQTT.  This allows any home automation system that supports MQTT to work with this, namely Home assistant.  

This was developed for use in an existing wired home alarm system.  The binary_sensor and switch files can be used/imported into code independent of this to allow for MQTT auto discovery setup.  Some modifications may need to be made depending on your use case.

## Synopsis
First lets modify the user environment variables and configure our [binary sensors and switches](https://github.com/csharpWorm/Alarm/blob/main/Alarm.ino#L29-L68) for use.  Supplying the necessary pin connected to, the pin mode, device class and other pertinent information necessary.

After our setup work is complete we try to establish a network connection via ethernet DHCP based on the user modifiable variables. If there is a problem with the ethernet hat or you're not connected to the network error messages will be printed to the serial output. If for some reason DHCP is not able to assign an IP address it tries to use a static fall back IP supplied by the user.

Once the network connection is established we setup the connection to the MQTT broker using the credientials and port provided by the user.  The callback method is mapped for receiving messages for subscribed Topics and the timer is setup for the necessary loop intervals.  Once the MQTT broker connection has been established the sensors MQTT discovery state, config, and command topics are created and sent to the broker. This allows for seemless integration with Home Assistant.

Code will run a check every 200ms to see if any of the binary sensors states have changed. If so it wil report the change to MQTT broker and update the state.
Every 2000ms(2sec) the code loops the broker to capture any messages that may have been sent. The callback will check the Topic and Payload to determine the necessary course of action (turn on or off a Relay).

To modify the behavior of a switch check out the turn_on and turn_off methods in [switch.cpp](https://github.com/csharpWorm/Alarm/blob/main/switch.cpp#L82-L93)

## :gear: Features
- User can create binary sensors for end devices connected to board in code
- User can create switches for end devices connected to board in code
- Connect to ethernet via DHCP but allows for static fall back
- Uses SimpleTimer from https://github.com/marcelloromani/arduino/tree/master/SimpleTimer to run methods without blocking
- Connects to MQTT using PubSubClient
- **Configures sensors with necessary information for MQTT auto discovery in Home Assistant**
- [Alarm.ino](https://github.com/csharpWorm/Alarm/blob/main/Alarm.ino#L29-L68) is setup with examples on how to configure the binary sensors and switches.

## :memo: Bill of Materials
- [Arduino Mega 2560](https://www.amazon.com/ELEGOO-ATmega2560-ATMEGA16U2-Projects-Compliant/dp/B01H4ZLZLQ/ref=sxts_sxwds-bia-wc-rsf-ajax1_0?cv_ct_cx=atmega2560&dchild=1&keywords=atmega2560&pd_rd_i=B01H4ZLZLQ&pd_rd_r=e8c6854b-8020-40fa-91de-0dd0f7431474&pd_rd_w=q9l5v&pd_rd_wg=OpFTe&pf_rd_p=5c711241-c674-4eef-b21c-fe6add670f33&pf_rd_r=EMK0VA052MPKF31GMSB2&psc=1&qid=1612656902&sr=1-1-e30f047d-8e3c-4340-8179-6a77ce88d756)
- [Arduino Mega Ethernet Shield](https://www.amazon.com/gp/product/B00HG82V1A/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)
- Wired End Devices (contact sensors, relays, etc...)

## Arduino programming

The first order of business will be to setup the Arduino IDE to flash the provided Arduino sketch to the Arduino Mega 2560.  [Download the IDE for your platform](https://www.arduino.cc/en/Main/Software) and [follow these instructions to familiarize yourself with the IDE and how to upload to the board](https://www.arduino.cc/en/Guide/ArduinoMega2560).

Next you will need to add the PubSubClient library for MQTT.  [Follow this guide for the general process](https://www.arduino.cc/en/Guide/Libraries) and add the 'PubSubClient' from the Library Manager.  Once that is installed you will need to edit the `PubSubClient.h` file and change the line `#define MQTT_MAX_PACKET_SIZE 128` to `#define MQTT_MAX_PACKET_SIZE 1024`.  You can find the installed library under the path shown in `File > Preferences > Sketchbook location`.

[At the top of the Arduino sketch are several fields you must modify to fit your environment](https://github.com/csharpWorm/Alarm/blob/main/Alarm.ino#L18-L28) (MQTT broker IP, node name, etc).  Once those fields have been set you can upload to your microcontroller and monitor sensor status in Home Assistant.  Some of these values are contained in the [Secrets.h](https://github.com/csharpWorm/Alarm/blob/main/Secrets.h#L1-L2) file to help decouple the sensistive data from the core code.

> ## WARNING:
> 
> If you will be deploying more than one of these devices you **must** change the node names to be unique.  Failure to do so will result in a cascading series of MQTT connections/disconnections as your devices compete for access to your broker.

## If you like it and want to donate
- [Paypal](https://paypal.me/CSharpWorm)
- [Venmo](https://venmo.com/code?user_id=2938016083476480091)
