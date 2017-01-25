#ifndef WIRELESSLOVE_H
#define WIRELESSLOVE_H

#include "logging.h"
#include <WiFi101.h>
#include <WiFiUdp.h>
#include "protoLighthouse.h"

class WIFI_LOVE{
public:
    WIFI_LOVE(const char* ssid, const char* passwd, IPAddress &broadcastIP);
    uint16_t getCmndPort(); 
    uint32_t getLocalIP(); 

    int     printWifiStatus(); 
    int     initWifi(); 
    int     initUDPSockets(); 
    int     fmsgTest_s(); 
    int     fmsgBroadcast_s(const uint8_t * buffer, size_t size); 
    int     fmsgLogging_s(const uint8_t * buffer, size_t size); 
    int     fmsgSensorDataT_s(const uint8_t *buffer, size_t size); 
    int     fmsgSensorData_s(const uint8_t *buffer, size_t size); 
    bool    receiveConfig(); 
    int     getConnectionStatus();
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
