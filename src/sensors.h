#ifndef SENSORS_H
#define SENSORS_H

#include "def.h"
#include "XViewerStruct.h"

#define SENSOR_STRUCT_CODE_NOT_FOUND 255
#define SENSOR_STRUCT_NAME_NOT_FOUND "NAME NOT FOUND"
#define SENSOR_STRUCT_TYPE_NOT_FOUND "TYPE NOT FOUND"
#define SENSOR_STRUCT_STATE_NOT_FOUND 0

class Sensors
{
public:
    void init();

    std::vector<sensorStruct_t> &getSensorsVect() { return _sensors; }

    bool addSensor(uint8_t code, char name[SENSOR_NAME_SIZE], char type[SENSOR_TYPE_SIZE]);
    bool removeSensor(uint8_t code);
    bool updateName(uint8_t code, char newName[SENSOR_NAME_SIZE]);
    bool updateType(uint8_t code, char newType[SENSOR_TYPE_SIZE]);
    bool updateState(uint8_t code, bool newState);
    sensorStruct_t getSensor(uint8_t code);

    DynamicJsonDocument vectToJson();

    void printSensors();

    void setDbg(bool dbgOn) { _dbgOn = dbgOn; }

private:
    std::vector<sensorStruct_t> _sensors;
    uint8_t _sensorsNumber = 0;

    bool _dbgOn = false;

    void _dbg(String str = "", bool header = true);

};

#endif