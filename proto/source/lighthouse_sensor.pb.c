/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.8-dev at Tue Jan 24 17:31:43 2017. */

#include "lighthouse_sensor.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t mkr1000_lighthouse_configObject_fields[4] = {
    PB_FIELD(  1, UINT32  , REQUIRED, STATIC  , FIRST, mkr1000_lighthouse_configObject, ip, ip, 0),
    PB_FIELD(  2, INT32   , REQUIRED, STATIC  , OTHER, mkr1000_lighthouse_configObject, logging_port, ip, 0),
    PB_FIELD(  3, INT32   , REQUIRED, STATIC  , OTHER, mkr1000_lighthouse_configObject, sensor_port, logging_port, 0),
    PB_LAST_FIELD
};

const pb_field_t mkr1000_lighthouse_trackedObjectConfig_fields[3] = {
    PB_FIELD(  1, UINT32  , REQUIRED, STATIC  , FIRST, mkr1000_lighthouse_trackedObjectConfig, ip, ip, 0),
    PB_FIELD(  2, INT32   , REQUIRED, STATIC  , OTHER, mkr1000_lighthouse_trackedObjectConfig, command_port, ip, 0),
    PB_LAST_FIELD
};

const pb_field_t mkr1000_lighthouse_loggingObject_fields[2] = {
    PB_FIELD(  1, STRING  , REQUIRED, STATIC  , FIRST, mkr1000_lighthouse_loggingObject, message, message, 0),
    PB_LAST_FIELD
};

const pb_field_t mkr1000_lighthouse_commandObject_fields[2] = {
    PB_FIELD(  1, INT32   , REQUIRED, STATIC  , FIRST, mkr1000_lighthouse_commandObject, command, command, 0),
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */
