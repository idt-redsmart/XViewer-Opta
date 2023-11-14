#ifndef ALARM_H
#define ALARM_H

#include "def.h"
#include "XViewerStruct.h"

class Alarm
{
public:
    void init();

    std::vector<alarmStruct_t> &getAlarmsVect() { return _alarms; }

    bool addAlarm(char code[ALARM_CODE_SIZE], char name[ALARM_NAME_SIZE]);
    bool removeAlarm(char code[ALARM_CODE_SIZE]);
    bool updateName(char code[ALARM_CODE_SIZE], char newName[ALARM_NAME_SIZE]);
    bool updateCounter(char code[ALARM_CODE_SIZE], uint16_t increase = 1);

    void print();

    DynamicJsonDocument vectToJson();

    void setDbg(bool dbgOn) { _dbgOn = dbgOn; }

private:
    std::vector<alarmStruct_t> _alarms;

    char _buffer[256];
    bool _dbgOn = false;

    void _dbg(String str);
};

#endif