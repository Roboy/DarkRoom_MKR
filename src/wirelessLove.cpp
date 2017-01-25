#include <SPI.h>
#include "wirelessLove.h"


 const char  TestBuffer[] ="Hello World"; 
 const int    timestampSize = 2; 

<<<<<<< HEAD
/************************ VARIABLES ***************************************/
static int      LoveStatus = WL_IDLE_STATUS;
static char     ssid[]="1-UTUM-Guest"; 
static char     pass[] = ""; 
static bool     timeout = false; 

static uint16_t  sensorPort_l= 2390; 
static uint16_t  commandPort_l = 2391; 
static uint16_t  logginPort_l = 2392; 

static uint16_t  sensorPort_t = 8000; 
static uint16_t  logginPort_t = 8001; 

static const char  remoteIP[] = "10.25.13.118"; 
static const char  TestBuffer[]="Hello World"; 
static const int    timestampSize = 2; 


static WiFiUDP  UDP_sensors; 
static WiFiUDP  UDP_commands; 
static WiFiUDP  UDP_loggin; 


static int printMacAddress(void)
{
    byte mac[6]; 
    WiFi.macAddress(mac); 
    Serial.print("MAC: "); 
    for(int i = 6; i >= 0; --i){
        Serial.print("MAC: "); 
        Serial.println(mac[i], HEX); 
    }
    return (int) ES_WIFI_SUCCESS; 
=======
WIFI_LOVE::WIFI_LOVE(const char* SSID, const char* PASSWD, IPAddress &broadcastIP): 
broadcastIP(broadcastIP){
    sprintf(pass,PASSWD);
    sprintf(ssid,SSID);
>>>>>>> devel
}

uint16_t WIFI_LOVE::getCmndPort()
{
    return (LoveStatus == WL_CONNECTED) ? commandPort : 0 ; 
}

uint32_t WIFI_LOVE::getLocalIP()
{
    return (LoveStatus == WL_CONNECTED) ? WiFi.localIP() : 0 ; 
}

int WIFI_LOVE::printWifiStatus(void)
{
    Serial.print("SSID: "); 
    Serial.println(WiFi.SSID()); 

    IPAddress ip = WiFi.localIP(); 
    Serial.print("IP Address: "); 
    Serial.println(ip);

    long rssi = WiFi.RSSI(); 
    Serial.print("signal strength (RSSI):"); 
    Serial.print(rssi); 
    Serial.println(" dBm"); 

    uint32_t UDPBufferSize = SOCKET_BUFFER_MTU; 
    Serial.print("Buffer Size from UDP Socket: "); 
    Serial.println(UDPBufferSize); 

    return (int) ES_WIFI_SUCCESS; 
}

int WIFI_LOVE::initWifi(void)
{
    uint32_t timoutCounter = 0; 
    
    // check for presence of the WiFiShield:
    if(WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present"); 
        return (int) ES_WIFI_FAIL_INIT_NO_SHIELD;  
    }

    // wait max 10 seconds to connect to the provided Wifi
    while(WiFi.begin(ssid) != WL_CONNECTED && !timeout){
        delay(1000); 
        timoutCounter++; 
        if(timoutCounter == 10){
            timeout = true; 
        }
    }

    if(true == timeout){
        LoveStatus = ES_WIFI_FAIL_INIT_CANNOT_CONNECT; 
        return (int) ES_WIFI_FAIL_INIT_CANNOT_CONNECT; 
    }

    LoveStatus = WL_CONNECTED;
    enableLogging = true; 
    return (int) ES_WIFI_SUCCESS; 
}

int WIFI_LOVE::initUDPSockets(void)
{
    if (0 == UDP_config.begin(commandPort)){
        return (int) ES_WIFI_FAIL_UDP_SOCKET; 
    }

    return (int) ES_WIFI_SUCCESS; 
}

int WIFI_LOVE::fmsgTest_s(void)
{
    UDP_sensors.beginPacket(remoteIP, sensorPort); 
    UDP_sensors.write(TestBuffer);
    UDP_sensors.endPacket();
    
    return (int) ES_WIFI_SUCCESS; 
}

int WIFI_LOVE::fmsgBroadcast_s(const uint8_t * buffer, size_t size)
{
    uint8_t res = ES_WIFI_ERROR; 
    if(0 == UDP_config.beginPacket(broadcastIP, configPort))
    {
        LOG(logWARNING, "Can not connect to the supplied IP or PORT");     
        return  res; 
    } 

    if(size != UDP_config.write(buffer, size)){
        Serial.println("Size of the UDP Package to big! Truncated overlapping data"); 
    }
    UDP_config.endPacket();
    return (int) ES_WIFI_SUCCESS; 
}

int WIFI_LOVE::fmsgLogging_s(const uint8_t * buffer, size_t size)
{
    UDP_logging.beginPacket(remoteIP, configPort); 
    if(size != UDP_logging.write(buffer, size)){
        Serial.println("Size of the UDP Package to big! Truncated overlapping data"); 
    }
    UDP_logging.endPacket();
    return (int) ES_WIFI_SUCCESS; 
}

bool WIFI_LOVE::receiveConfig()
{
    bool rcv = false; 
    int packetSize = 0; 

    packetSize = UDP_config.parsePacket(); 
    LOG_d(logINFO, "check if command rcvd ", packetSize); 
    if(packetSize > 0 )
    { 
        unsigned char buffer[255]; 
        LOG(logINFO, "rcvd command from host..."); 
        size_t len = UDP_config.read(buffer , sizeof buffer); 
        if(len > 0 && len < 60) 
        {
            buffer[len] = '\0'; 
            protoLove.decode_config_Proto(buffer, len); 
            rcv = true; 

            remoteIP = protoLove.configObjMsg.ip;
            logginPort = protoLove.configObjMsg.logging_port; 
            sensorPort = protoLove.configObjMsg.sensor_port;

            if (!UDP_sensors.begin(sensorPort)){
                return false; 
            }

            if (!UDP_logging.begin(logginPort)){
                return false; 
            }
        }   
    }
    return rcv; 
}

int WIFI_LOVE::fmsgSensorDataT_s(const uint8_t * buffer, size_t size)
{
    LOG_d(logVERBOSE, "Send UDP Packet with Timestamp, size: ", size + timestampSize); 
    UDP_sensors.beginPacket(remoteIP, sensorPort); 
    unsigned long t = millis(); 
    uint8_t *addr = (uint8_t*) &t;
    for(int i =  0; i < timestampSize; i++){
        UDP_sensors.write(addr[i]);
    }

    if(size != UDP_sensors.write(buffer, size)){
        Serial.println("Size of the UDP Package to big! Truncated overlapping data"); 
    }

    UDP_sensors.endPacket();
    return (int) ES_WIFI_SUCCESS; 
}

int WIFI_LOVE::fmsgSensorData_s(const uint8_t * buffer, size_t size)
{
    UDP_sensors.beginPacket(remoteIP, sensorPort); 
    if(size != UDP_sensors.write(buffer, size)){
        LOG(logERROR,"Size of the UDP Package to big! Truncated overlapping data"); 
    }
    UDP_sensors.endPacket();
    return (int) ES_WIFI_SUCCESS; 
}

int WIFI_LOVE::getConnectionStatus(void)
{
    int dV = LoveStatus; 
    return dV; 
}

void WIFI_LOVE::checkHostConfig(){
    // wait for a config message
    bool receivedConfig = false;
    while(!receivedConfig)
    {
        pb_byte_t buffer[512] = {0};
        size_t msg_len;
        if(protoLove.encode_trackedObjConfig( getLocalIP(), getCmndPort(), buffer, msg_len)){
            if(fmsgBroadcast_s(buffer, msg_len)){
                LOG(logINFO, "trackedObjectConfig protobuffer successfully sent via UDP Socket"); 
            }else{
                LOG(logERROR, "Sending failed!"); 
            }
        } 
        receivedConfig = receiveConfig();
        delay(1000); 
    }    
}