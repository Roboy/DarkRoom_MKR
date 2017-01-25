#define ARDUINO_MAIN

#include "wirelessLove.h"
#include "fifo.h"
#include "logging.h"
#include "SPI.h"


#define BITS_LAST(k,n)  ((k) & ((1 << (n))-1))                  // get n-last bits from number k 
#define RANGE(k, m, n)    BITS_LAST(((k) >> m), ((n)-(m)))        // get bit range [m-n] from number k 
#define FIFO_SIZE 16

typedef enum _SPI_PINS{
    TRDY = 0,
    SS_N = 7,  
}_SPI_PINS; 

typedef struct _Sensor{
    FIFO128t    mSweepFIFO; 
}Sensor; 

volatile static Sensor sensors; 

WIFI_LOVE *whylove;

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

// Initialize C library
extern "C" void __libc_init_array(void);

static void 
initPeripherals()
{
    delay(1000); 
    /************** WIFI *****************************/
    if(ES_WIFI_SUCCESS != whylove->initWifi()){
        Serial.println("Error in initializing the WiFi!"); 
    }else{
        whylove->printWifiStatus(); 
    }

    if(ES_WIFI_SUCCESS != whylove->initUDPSockets()){
        Serial.println("Error in initializing the UDP Sockets!"); 
    }
   
    /************** SPI ****************************/
    pinMode(TRDY    ,INPUT); 
    pinMode(SS_N    ,OUTPUT); 
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);  
}

static void sensor_spi(void)
{
    static uint32_t prevData = 0; 
    static uint8_t dataT = 0; 
    static uint32_t dataR_f= 0; 
    static uint16_t dataR_1 =0, dataR_2 = 0; 

    digitalWrite(SS_N, LOW); 
    SPI.transfer(dataT);
    dataR_1 = SPI.transfer16(dataT);
    dataR_2 = SPI.transfer16(dataT);
    digitalWrite(SS_N, HIGH); 

    dataR_f = dataR_1 << 16 | dataR_2;     
    FIFO128_write(sensors.mSweepFIFO, dataR_f); 
}

static void 
initPins_Interrupts(void)
{
    attachInterrupt(TRDY, sensor_spi, HIGH); 
}

// TODO: Improve logging system:
// #1 attach Interrupt to the TRDY Pin
// #3 Communication via UDP 
// #4 BitMask to enable granularity logging

int main( void )
{
    init();
    __libc_init_array();
    initVariant();
    delay(1);

#if defined(USBCON)
    USBDevice.init();
    USBDevice.attach();
#endif

    IPAddress broadcastIP(10,25,15,255);
    whylove = new WIFI_LOVE("1-UTUM-Guest", "", broadcastIP);

    // init the FIFO Buffer
    FIFO_init(sensors.mSweepFIFO); 

    initPeripherals(); 
    initPins_Interrupts(); 
    enableLogging = true;   

    
    whylove->checkHostConfig();
    LOG(logINFO, "received config");

    // send sensor data if available
    for (;;)
    {
        noInterrupts(); 
        uint32_t detectedSweep = FIFO128_read(sensors.mSweepFIFO); 
        interrupts(); 
        while( NULL != detectedSweep){
            if( (detectedSweep >> 12 & 0x01) == 1 ){ // if valid 
                if(whylove->fmsgSensorDataT_s( (uint8_t *) &detectedSweep, 4)){
                    // reading done, decode received data according to our protocol to print logging output
                    if(enableLogging){
                        int id             = detectedSweep & 0x01FF; 
                        int lighthouse     = (detectedSweep >> 9) & 0x01; 
                        int vertical       = (detectedSweep >> 10) & 0x01; 
                        int sweepDuration  = (detectedSweep >> 13) & 0x07FFFF;

                        LOG_d(logINFO, "ID: ", id); 
                        LOG_d(logINFO, "vertical: ", vertical); 
                        LOG_d(logINFO, "lighthouse: ", lighthouse); 
                        LOG_d(logINFO, "sweepDuration: ", sweepDuration); 
                        LOG(logINFO, "******************************************");
                    }
                }else{
                    LOG_d(logINFO, "could not send UDP ", 0);
                }
            }
            noInterrupts(); 
            detectedSweep = FIFO128_read(sensors.mSweepFIFO); 
            interrupts(); 
        } 
        LOG_d(logINFO, "fifo empty", 0);
    }

    return 0;
}
