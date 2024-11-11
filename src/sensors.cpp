#include "sensors.h"

void Sensors::init()
{
}

bool Sensors::addSensor(uint8_t code, char name[SENSOR_NAME_SIZE], char type[SENSOR_TYPE_SIZE])
{
    char buffer[64 + SENSOR_NAME_SIZE + SENSOR_TYPE_SIZE];
    sprintf(buffer, "Add sensor\tCode: %2u | Name: %32s | Type: %16s", code, name, type);
    _dbg(buffer);

    // Scan sensor vector and check if the code added is already in use.
    for (uint8_t index = 0; index < _sensors.size(); index++)
    {
        if (code == _sensors.at(index).code)
        {
            sprintf(buffer, "Code %2u already in use. Name: %32s | Type: %16s", _sensors.at(index).code, _sensors.at(index).name, _sensors.at(index).type);
            _dbg(buffer);

            return false;
        }
    }

    // Check vector max size
    if (_sensors.size() + 1 >= SENSOR_MAX_NUMBER)
    {
        _dbg("Maximum number of sensors reached. Can't add");
        return false;
    }

    // Code is not used - add
    sensorStruct_t tmp;
    tmp.code = code;
    memcpy(tmp.name, name, SENSOR_NAME_SIZE);
    memcpy(tmp.type, type, SENSOR_TYPE_SIZE);
    tmp.state = false;

    _sensors.push_back(tmp);

    sprintf(buffer, "Sensor added\tCode: %2u | Name: %32s | Type: %16s", code, name, type);
    _dbg(buffer);

    _sensorsNumber = _sensors.size();

    return true;
}

bool Sensors::removeSensor(uint8_t code)
{
    // char buffer[64 + SENSOR_NAME_SIZE + SENSOR_TYPE_SIZE];   used for DBG
    bool sensFound = false;
    int8_t indexSens = -1;
    std::vector<sensorStruct_t> tmpVect(_sensors);

    _dbg("Trying to remove code: " + String(code));

    for (uint8_t index = 0; index < tmpVect.size(); index++)
    {
        if (tmpVect.at(index).code == code)
        {
            sensFound = true;
            indexSens = index;
            break;
        }
    }

    if (sensFound)
    {
        tmpVect.erase(tmpVect.begin() + indexSens);
    }
    else
    {
        _dbg("Sensor code to remove not found. Looking for code: " + String(code));
        return false;
    }

    _sensors.clear();

    for (uint8_t index = 0; index < tmpVect.size(); index++)
    {
        sensorStruct_t tmp;

        tmp.code = tmpVect.at(index).code;
        memcpy(tmp.name, tmpVect.at(index).name, SENSOR_NAME_SIZE);
        memcpy(tmp.type, tmpVect.at(index).type, SENSOR_TYPE_SIZE);
        tmp.state = tmpVect.at(index).state;

        _sensors.push_back(tmp);

        // DBG
        //  sprintf(buffer, "Push back sensor\tCode: %2u | Name: %32s | Type: %16s", tmp.code, tmp.name, tmp.type);
        //  _dbg(buffer);
    }

    _sensorsNumber = _sensors.size();
    return true;
}

bool Sensors::updateName(uint8_t code, char newName[SENSOR_NAME_SIZE])
{
    char buffer[64 + (SENSOR_NAME_SIZE * 2)];

    // Scan sensor vector and check if the code is in vector
    for (uint8_t index = 0; index < _sensors.size(); index++)
    {
        if (code == _sensors.at(index).code)
        {
            sprintf(buffer, "Code %2u found. Updating name: %32s with new name: %32s", _sensors.at(index).code, _sensors.at(index).name, newName);
            _dbg(buffer);

            memcpy(_sensors.at(index).name, newName, SENSOR_NAME_SIZE);

            return true;
        }
    }

    sprintf(buffer, "Code %2u not found.", code);
    _dbg(buffer);
    return false;
}

bool Sensors::updateType(uint8_t code, char newType[SENSOR_TYPE_SIZE])
{
    char buffer[64 + (SENSOR_TYPE_SIZE * 2)];

    // Scan sensor vector and check if the code is in vector
    for (uint8_t index = 0; index < _sensors.size(); index++)
    {
        if (code == _sensors.at(index).code)
        {
            sprintf(buffer, "Code %2u found. Updating name: %16s with new name: %16s", _sensors.at(index).code, _sensors.at(index).type, newType);
            _dbg(buffer);

            memcpy(_sensors.at(index).type, newType, SENSOR_TYPE_SIZE);

            return true;
        }
    }

    sprintf(buffer, "Code %2u not found.", code);
    _dbg(buffer);
    return false;
}

bool Sensors::updateState(uint8_t code, bool newState)
{
    char buffer[64];

    // Scan sensor vector and check if the code is in vector
    for (uint8_t index = 0; index < _sensors.size(); index++)
    {
        if (code == _sensors.at(index).code)
        {
            _sensors.at(index).state = newState;
            sprintf(buffer, "Code %2u found. New state: %1u", _sensors.at(index).code, _sensors.at(index).state);
            _dbg(buffer);

            return true;
        }
    }

    sprintf(buffer, "Code %2u not found.", code);
    _dbg(buffer);
    return false;
}

sensorStruct_t Sensors::getSensor(uint8_t code)
{
    char buffer[128 + SENSOR_NAME_SIZE + SENSOR_TYPE_SIZE];
    sensorStruct_t res;
    // Scan sensor vector and check if the code is in vector
    for (uint8_t index = 0; index < _sensors.size(); index++)
    {
        if (code == _sensors.at(index).code)
        {
            res.code = _sensors.at(index).code;
            memcpy(res.name, _sensors.at(index).name, SENSOR_NAME_SIZE);
            memcpy(res.type, _sensors.at(index).type, SENSOR_TYPE_SIZE);
            res.state = _sensors.at(index).state;

            sprintf(buffer, "Found sensor struct | Code:%2u | Name:%32s | Type:%16s | State:%1u", res.code, res.name, res.type, res.state);
            _dbg(buffer);

            return res;
        }
    }
    res.code = SENSOR_STRUCT_CODE_NOT_FOUND;
    memcpy(res.name, SENSOR_STRUCT_NAME_NOT_FOUND, SENSOR_NAME_SIZE);
    memcpy(res.type, SENSOR_STRUCT_TYPE_NOT_FOUND, SENSOR_TYPE_SIZE);
    res.state = SENSOR_STRUCT_STATE_NOT_FOUND;
    return res;
}

void Sensors::printSensors()
{
    char buffer[64 + SENSOR_NAME_SIZE + SENSOR_TYPE_SIZE];

    _dbg("\nPRINTING SENSORS", 0);

    for (uint8_t index = 0; index < _sensors.size(); index++)
    {
        sprintf(buffer, "Code: %2u | Name: %32s | Type: %16s | State: %1u", _sensors.at(index).code, _sensors.at(index).name, _sensors.at(index).type, _sensors.at(index).state);
        _dbg(buffer, 0);
    }
}

DynamicJsonDocument Sensors::vectToJson()
{
    DynamicJsonDocument json(CONFIG_SENS_JSON_SIZE);

    String str;

    json[K_SENS_NO] = _sensors.size();
    JsonArray sensorInfoObj = json.createNestedArray("vect");

    for (uint8_t index = 0; index != _sensors.size(); index++)
    {
        if (_sensors.at(index).code == 255)
            break;

        JsonObject sensorInfoArr = sensorInfoObj.createNestedObject();

        sensorInfoArr[K_SENS_CODE] = _sensors.at(index).code;
        sensorInfoArr[K_SENS_NAME] = _sensors.at(index).name;
        sensorInfoArr[K_SENS_TYPE] = _sensors.at(index).type;
        sensorInfoArr[K_SENS_STATE] = _sensors.at(index).state;
    }

    serializeJsonPretty(json, str);
    return json;
}

void Sensors::_dbg(String str, bool header)
{
    if (_dbgOn)
    {
        if (header)
            Serial.println("[SENSOR]\t" + str);
        else
            Serial.println(str);
    }
}
