#include "alarm.h"

bool Alarm::addAlarm(char code[ALARM_CODE_SIZE], char name[ALARM_NAME_SIZE])
{
    sprintf(_buffer, "Add alarm\tCode: %32s | Name: %32s", code, name);
    _dbg(_buffer);

    // Scan alarm vector and check if the code added is already in use.
    for (uint8_t index = 0; index < _alarms.size(); index++)
    {
        if (strcmp(code, _alarms.at(index).code) == 0)
        {
            sprintf(_buffer, "Code %32s already in use. Name: %32s", _alarms.at(index).code, _alarms.at(index).name);
            _dbg(_buffer);

            return false;
        }
    }
    // Check vector max size
    if (_alarms.size() + 1 > ALARM_MAX_NUMBER)
    {
        _dbg("Maximum number of alarm reached. Can't add");
        return false;
    }

    // Code is not used - add
    alarmStruct_t tmp;
    memcpy(tmp.name, name, ALARM_NAME_SIZE);
    memcpy(tmp.code, code, ALARM_CODE_SIZE);
    tmp.counter = 0;

    _alarms.push_back(tmp);

    sprintf(_buffer, "Alarm added\tCode: %32s | Name: %32s", code, name);
    _dbg(_buffer);

    return true;
}

bool Alarm::removeAlarm(char code[ALARM_CODE_SIZE])
{
    bool alarmFound = false;
    int8_t indexAlarm = -1;
    std::vector<alarmStruct_t> tmpVect(_alarms);

    _dbg("Trying to remove code: " + String(code));

    for (uint8_t index = 0; index < tmpVect.size(); index++)
    {
        if (strcmp(tmpVect.at(index).code, code) == 0)
        {
            alarmFound = true;
            indexAlarm = index;
            break;
        }
    }

    if (alarmFound)
    {
        tmpVect.erase(tmpVect.begin() + indexAlarm);
    }
    else
    {
        _dbg("Alarm code to remove not found. Looking for code: " + String(code));
        return false;
    }

    _alarms.clear();

    for (uint8_t index = 0; index < tmpVect.size(); index++)
    {
        alarmStruct_t tmp;

        memcpy(tmp.code, tmpVect.at(index).code, ALARM_CODE_SIZE);
        memcpy(tmp.name, tmpVect.at(index).name, ALARM_NAME_SIZE);
        tmp.counter = tmpVect.at(index).counter;

        _alarms.push_back(tmp);
    }
    return true;
}

bool Alarm::updateName(char code[ALARM_CODE_SIZE], char newName[ALARM_NAME_SIZE])
{
    // Scan alarm vector and check if the code is in vector
    for (uint8_t index = 0; index < _alarms.size(); index++)
    {
        if (strcmp(_alarms.at(index).code, code) == 0)
        {
            sprintf(_buffer, "Code %32s found. Updating name: %32s with new name: %32s", _alarms.at(index).code, _alarms.at(index).name, newName);
            _dbg(_buffer);

            memcpy(_alarms.at(index).name, newName, ALARM_NAME_SIZE);

            return true;
        }
    }

    sprintf(_buffer, "Code %32s not found.", code);
    _dbg(_buffer);
    return false;
}

bool Alarm::updateCounter(char code[ALARM_CODE_SIZE], uint16_t increase)
{
    // Scan alarm vector and check if the code is in vector
    for (uint8_t index = 0; index < _alarms.size(); index++)
    {
        if (strcmp(_alarms.at(index).code, code) == 0)
        {
            sprintf(_buffer, "Code %32s found. Updating counter.", _alarms.at(index).code);
            _dbg(_buffer);

            _alarms.at(index).counter += increase;

            return true;
        }
    }

    sprintf(_buffer, "Code %32s not found.", code);
    _dbg(_buffer);
    return false;
}

void Alarm::print()
{
    _dbg("\nPRINTING ALARMS");

    for (uint8_t index = 0; index < _alarms.size(); index++)
    {
        sprintf(_buffer, "Code: %32s | Name: %32s | Counter: %6u ",
                _alarms.at(index).code, _alarms.at(index).name, _alarms.at(index).counter);
        _dbg(_buffer);
    }
}

DynamicJsonDocument Alarm::vectToJson()
{
    DynamicJsonDocument json(CONFIG_ALARM_JSON_SIZE);

    String str;

    json[K_ALARM_NO] = _alarms.size();
    JsonArray alarmObj = json.createNestedArray("vect");

    for (uint8_t index = 0; index != _alarms.size(); index++)
    {
        JsonObject alarmArr = alarmObj.createNestedObject();

        alarmArr[K_ALARM_CODE] = _alarms.at(index).code;
        alarmArr[K_ALARM_NAME] = _alarms.at(index).name;
        alarmArr[K_ALARM_COUNTER] = _alarms.at(index).counter;
    }

    serializeJsonPretty(json, str);
    return json;
}

void Alarm::_dbg(String str)
{
    if (_dbgOn)
        Serial.println("[AL]\t" + str);
}