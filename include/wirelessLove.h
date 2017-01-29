#ifndef WIRELESSLOVE_H
#define WIRELESSLOVE_H

#include "logging.h"
#include <WiFi101.h>
#include <WiFiUdp.h>
#include "protoLighthouse.h"

class WIFI_LOVE{
public:
    /**
    * @param ssid name of wifi network
    * @param passwd the password
    * @param Bcast ip of your network (check with ifconfig)
    */
    WIFI_LOVE(const char* ssid, const char* passwd, IPAddress &broadcastIP);
    /**
    * @return command port
    */
    uint16_t getCmndPort();
    /**
    * @return MKRs' IP
    */
    uint32_t getLocalIP();

    /** prints information about the wifi network */
    void    printWifiStatus();
    /**
    * Initializes the wifi connection
    * @return succesful connection to wifi
    */
    int     initWifi();
    /**
    * Initializes the UDP socket
    * @return success
    */
    int     initUDPSockets();
    /**
    * Sends a UDP test package
    */
    void     test_send();
    /**
    * Broadcasts the config message
    * @param buffer the encoded protobuf buffer
    * @param size how big in bytes
    * @param success
    */
    int     broadcast_send(const uint8_t * buffer, size_t size);
    /**
    * Sends a logging message
    * @param buffer the encoded protobuf buffer
    * @param size how big in bytes
    * @param success
    */
    int     logging_send(const uint8_t * buffer, size_t size);
    /**
    * Sends a sensor bitfield augmented with a milliseconds timestamp
    * @param buffer the sensor bitfield
    * @param size how big in bytes
    * @param success
    */
    int     sensor_timestamped_send(const uint8_t *buffer, size_t size);
    /**
    * Sends a sensor bitfield
    * @param buffer the sensor bitfield
    * @param size how big in bytes
    * @param success
    */
    int     sensor_send(const uint8_t *buffer, size_t size);
    /**
    * Checks if a cofig message was received. If true it initialzes the Sockets
    * for logging and sensor ports
    * @return successfully configured
    */
    bool    receiveConfig();
    /**
    * Blocks until MKR received a config message from host
    */
    void    checkHostConfig();

    int      LoveStatus = WL_IDLE_STATUS;
    char     ssid[100];
    char     pass[100];
    bool     timeout = false;

    uint16_t  sensorPort   = 2390;
    uint16_t  commandPort  = 2391;
    uint16_t  logginPort   = 2392;
    uint16_t  configPort   = 8001;

    IPAddress remoteIP;;
    IPAddress broadcastIP;

    WiFiUDP  UDP_config;
    WiFiUDP  UDP_sensors;
    WiFiUDP  UDP_commands;
    WiFiUDP  UDP_logging;
    PROTO_LOVE protoLove;
};


#endif
