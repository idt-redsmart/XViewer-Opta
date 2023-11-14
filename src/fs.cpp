#include "fs.h"

bool FsClass::init()
{
    mbed::BlockDevice *raw = mbed::BlockDevice::get_default_instance();
    mbed::MBRBlockDevice *mbr = new mbed::MBRBlockDevice(raw, 2);

    int err = mbr->init();

    if (err < 0)
    {
        _dbg("[FS] Error initializing Block Device " + String(err));
    }

    mbed::FATFileSystem *fs = new mbed::FATFileSystem("config");

    err = fs->mount(mbr);

    if (err < 0)
    {
        _dbg("[FS] Error mounting filesystem  " + String(err));
        return false;
    }
    return true;
}

bool FsClass::deleteConfig(const char *path)
{
    Serial.print("Deleting config: ");
    _dbg(path);

    if (remove(path) == 0)
        return true;
    else
        return false;
}

bool FsClass::storeConfig(const char *path, DynamicJsonDocument json)
{
    FILE *configFile = fopen(path, "r");
    String payload;

    serializeJson(json, payload);

    if (configFile != NULL)
    {
        _dbg("[FS] File found: " + String(path) + ". Opening");
    }
    else
    {
        _dbg("[FS] File not found: " + String(path) + ". Adding");
    }

    configFile = fopen(path, "w");
    if (fwrite((uint8_t *)payload.c_str(), 1, payload.length(), configFile))
    {
        _dbg("Writing OK");
        fclose(configFile);
    }
    else
    {
        _dbg("Writing failed");
        fclose(configFile);
        return false;
    }
    return true;
}

bool FsClass::readSensConfig(std::vector<sensorStruct_t> &sensVect)
{
    String res = String();
    FILE *configFile = fopen(PATH_SENS_CONFIG, "r");

    _dbg("Reading sensors config file at: " + String(PATH_SENS_CONFIG));

    if (configFile == NULL)
    {
        _dbg("File not found");
        return false;
    }

    _dbg("File found");

    uint8_t isChar = 1;
    char cRead;

    while (isChar > 0)
    {
        isChar = fread((uint8_t *)&cRead, sizeof(cRead), 1, configFile);

        if (isChar)
            res += cRead;
    }

    fclose(configFile);

    // _dbg("FILE: " + res);

    DynamicJsonDocument json(CONFIG_SENS_JSON_SIZE);
    deserializeJson(json, res);
    uint8_t sensNo = json[K_SENS_NO];

    for (uint8_t index = 0; index < sensNo; index++)
    {
        sensorStruct_t tmp;
        tmp.code = json[K_SENS_VECT][index][K_SENS_CODE];

        const char *name = json[K_SENS_VECT][index][K_SENS_NAME];
        memcpy(tmp.name, name, SENSOR_NAME_SIZE);

        const char *type = json[K_SENS_VECT][index][K_SENS_TYPE];
        memcpy(tmp.type, type, SENSOR_TYPE_SIZE);

        tmp.state = json[K_SENS_VECT][index][K_SENS_STATE];

        sensVect.push_back(tmp);
    }

    return true;
}

bool FsClass::readUserConfig(std::vector<userStruct_t> &userVect)
{
    String res = String();
    FILE *configFile = fopen(PATH_USER_CONFIG, "r");

    _dbg("Reading user config file at: " + String(PATH_USER_CONFIG));

    if (configFile == NULL)
    {
        _dbg("File not found");
        return false;
    }

    _dbg("File found");

    uint8_t isChar = 1;
    char cRead;

    while (isChar > 0)
    {
        isChar = fread((uint8_t *)&cRead, sizeof(cRead), 1, configFile);

        if (isChar)
            res += cRead;
    }

    fclose(configFile);

    DynamicJsonDocument json(CONFIG_USER_JSON_SIZE);
    deserializeJson(json, res);
    uint8_t userNo = json[K_USER_NO];

    for (uint8_t index = 0; index < userNo - 1; index++)
    {
        userStruct_t tmp;

        const char *username = json[K_USER_VECT][index][K_USER_USERNAME];
        memcpy(tmp.username, username, USER_USERNAME_SIZE);

        const char *pass = json[K_USER_VECT][index][K_USER_PASS];
        memcpy(tmp.pass, pass, USER_PASS_SIZE);

        tmp.isAdmin = json[K_USER_VECT][index][K_USER_ADMIN];

        tmp.autologin = json[K_USER_VECT][index][K_USER_AL];

        userVect.push_back(tmp);
    }
    return true;
}

bool FsClass::readAlarmConfig(std::vector<alarmStruct_t> &alarmVect)
{
    String res = String();
    FILE *configFile = fopen(PATH_ALARM_CONFIG, "r");

    _dbg("Reading alarm config file at: " + String(PATH_ALARM_CONFIG));

    if (configFile == NULL)
    {
        _dbg("File not found");
        return false;
    }

    _dbg("File found");

    uint8_t isChar = 1;
    char cRead;

    while (isChar > 0)
    {
        isChar = fread((uint8_t *)&cRead, sizeof(cRead), 1, configFile);

        if (isChar)
            res += cRead;
    }

    fclose(configFile);

    DynamicJsonDocument json(CONFIG_ALARM_JSON_SIZE);
    deserializeJson(json, res);
    uint8_t alarmNo = json[K_ALARM_NO];

    for (uint8_t index = 0; index < alarmNo; index++)
    {
        alarmStruct_t tmp;

        const char *code = json[K_ALARM_VECT][index][K_ALARM_CODE];
        memcpy(tmp.code, code, ALARM_CODE_SIZE);

        const char *name = json[K_ALARM_VECT][index][K_ALARM_NAME];
        memcpy(tmp.name, name, ALARM_NAME_SIZE);

        tmp.counter = json[K_ALARM_VECT][index][K_ALARM_COUNTER];

        alarmVect.push_back(tmp);
    }
    return true;
}

bool FsClass::readProdConfig(std::vector<prodStruct_t> &prodVect)
{
    String res = String();
    FILE *configFile = fopen(PATH_PROD_CONFIG, "r");

    _dbg("Reading alarm config file at: " + String(PATH_PROD_CONFIG));

    if (configFile == NULL)
    {
        _dbg("File not found");
        return false;
    }

    _dbg("File found");

    uint8_t isChar = 1;
    char cRead;

    while (isChar > 0)
    {
        isChar = fread((uint8_t *)&cRead, sizeof(cRead), 1, configFile);

        if (isChar)
            res += cRead;
    }

    fclose(configFile);

    DynamicJsonDocument json(CONFIG_PROD_JSON_SIZE);
    deserializeJson(json, res);
    uint8_t prodNo = json[K_PROD_NO];

    for (uint8_t index = 0; index < prodNo; index++)
    {
        prodStruct_t tmp;

        const char *code = json[K_PROD_VECT][index][K_PROD_CODE];
        memcpy(tmp.code, code, PROD_CODE_SIZE);

        const char *name = json[K_PROD_VECT][index][K_PROD_NAME];
        memcpy(tmp.name, name, PROD_NAME_SIZE);

        const char *customer = json[K_PROD_VECT][index][K_PROD_CSTR];
        memcpy(tmp.customer, customer, PROD_CSTR_SIZE);

        tmp.totalCounters.Ok = json[K_PROD_VECT][index][K_PROD_T_OK];
        tmp.totalCounters.Nok = json[K_PROD_VECT][index][K_PROD_T_NOK];

        tmp.partialCounters.Ok = json[K_PROD_VECT][index][K_PROD_P_OK];
        tmp.partialCounters.Nok = json[K_PROD_VECT][index][K_PROD_P_NOK];

        prodVect.push_back(tmp);
    }
    return true;
}

void FsClass::_dbg(String str)
{
    if (_dbgOn)
        Serial.println("[FS]\t" + str);
}
