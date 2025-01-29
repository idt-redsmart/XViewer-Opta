#include "XViewer.h"

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_getSensors(ClientType &client)
{
    std::vector<sensorStruct_t> tmpV(_sensors.getSensorsVect());

    uint16_t jsonSize = 32 + (128 * tmpV.size()); // char of jsonArray + (char of jsonObject + _sensorsNumber)

    DynamicJsonDocument json(jsonSize);
    String str;

    JsonObject sensorObj = json.createNestedObject("sensors");

    sensorObj["number"] = tmpV.size();
    JsonArray sensorInfoObj = sensorObj.createNestedArray("info");

    for (uint8_t index = 0; index != tmpV.size(); index++)
    {
        if (tmpV.at(index).code == 255)
            break;

        JsonObject sensorInfoArr = sensorInfoObj.createNestedObject();

        sensorInfoArr["code"] = tmpV.at(index).code;
        sensorInfoArr["name"] = tmpV.at(index).name;
        sensorInfoArr["type"] = tmpV.at(index).type;
    }

    serializeJsonPretty(json, str);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(str.length());
    client.println();
    client.println(str);

    json.clear();

    // dbg(API, "CALLED API GET SENSOR");
    _dbgA("CALLED API GET SENSOR");
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_getSensorsState(ClientType &client)
{
    std::vector<sensorStruct_t> tmpV(_sensors.getSensorsVect());

    uint16_t jsonSize = 256 * tmpV.size();
    DynamicJsonDocument json(jsonSize);
    String str;

    JsonArray sensorObj = json.createNestedArray("sensors state");

    for (uint8_t index = 0; index < tmpV.size(); index++)
    {
        if (tmpV.at(index).code == 255)
            break;

        JsonObject singleSensor = sensorObj.createNestedObject();
        singleSensor["code"] = tmpV.at(index).code;
        singleSensor["value"] = tmpV.at(index).state;
    }

    serializeJsonPretty(json, str);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(str.length());
    client.println();
    client.println(str);

    // dbg(API, "CALLED API GET SENSOR STATE");
    _dbgA("CALLED API GET SENSOR STATE");
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_getTotalCounters(ClientType &client)
{
    DynamicJsonDocument json(256);
    String str;

    prodStruct_t tmp(_prods.getCurrentProd());

    JsonArray countersArr = json.createNestedArray("Total counter");
    JsonObject counterOk = countersArr.createNestedObject();
    JsonObject counterNok = countersArr.createNestedObject();
    counterOk["Counter OK"] = tmp.totalCounters.Ok;    // getCounter(TOTAL_COUNTER_OK);
    counterNok["Counter NOK"] = tmp.totalCounters.Nok; // getCounter(TOTAL_COUNTER_NOK);

    serializeJsonPretty(json, str);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(str.length());
    client.println();
    client.println(str);

    json.clear();

    // dbg(API, "CALLED API GET TOTAL COUNTERS");
    _dbgA("CALLED API GET TOTAL COUNTERS");
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_getPartialCounters(ClientType &client)
{
    DynamicJsonDocument json(256);
    String str;

    prodStruct_t tmp(_prods.getCurrentProd());

    JsonArray countersArr = json.createNestedArray("Partial counter");
    JsonObject counterOk = countersArr.createNestedObject();
    JsonObject counterNok = countersArr.createNestedObject();
    counterOk["Counter OK"] = tmp.partialCounters.Ok;    // getCounter(PARTIAL_COUNTER_OK);
    counterNok["Counter NOK"] = tmp.partialCounters.Nok; // getCounter(PARTIAL_COUNTER_NOK);

    serializeJsonPretty(json, str);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(str.length());
    client.println();
    client.println(str);

    json.clear();

    // dbg(API, "CALLED API GET PARTIAL COUNTERS");
    _dbgA("CALLED API GET PARTIAL COUNTERS");
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_getInfoUser(ClientType &client)
{
    uint16_t jsonSize = (32 + sizeof(userStruct_t)) * (USER_MAX_NUMBER + 1);

    DynamicJsonDocument json(jsonSize);
    String str;

    std::vector<userStruct_t> tmp(_users.getUserVect());

    JsonArray userArr = json.createNestedArray("Users");

    for (uint8_t index = 0; index < tmp.size(); index++)
    {
        JsonObject userObj = userArr.createNestedObject();

        userObj["username"] = String(tmp.at(index).username);
        userObj["password"] = String(tmp.at(index).pass);
        userObj["isAdmin"] = tmp.at(index).isAdmin;
    }

    serializeJsonPretty(json, str);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(str.length());
    client.println();
    client.println(str);

    json.clear();

    // dbg(API, "CALLED API GET INFO USER");
    _dbgA("CALLED API GET INFO USER");
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_getProduct(ClientType &client)
{
    uint16_t jsonSize = 64 * PROD_MAX_NUMBER;
    DynamicJsonDocument json(jsonSize);
    String str;

    std::vector<prodStruct_t> tmp(_prods.getProdVect());

    JsonArray productObj = json.createNestedArray("products");

    for (uint8_t index = 0; index < tmp.size(); index++)
    {
        JsonObject productArray = productObj.createNestedObject();

        productArray["productCode"] = tmp.at(index).code;
        productArray["productName"] = tmp.at(index).name;
        productArray["Customer"] = tmp.at(index).customer;
    }

    serializeJsonPretty(json, str);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(str.length());
    client.println();
    client.print(str);
    client.println();

    // dbg(API, "CALLED API GET PRODUCT ");
    _dbgA("CALLED API GET PRODUCT ");
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_getPrinterData(ClientType &client)
{
    printerStruct_t tmp(getCurrentPrinter());

    DynamicJsonDocument json(512);
    String str;
    String date = getCurrentTime();

    json["enable"] = tmp.enable;
    json["dateEnable"] = tmp.printDate;
    json["date"] = date;
    json["customerEnable"] = tmp.printCstr;
    json["customer"] = tmp.customerStr;
    json["userEnable"] = tmp.printUser;
    json["user"] = tmp.userStr;
    json["productCode"] = tmp.productCode;
    json["productName"] = tmp.productName;

    serializeJsonPretty(json, str);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(str.length());
    client.println();
    client.print(str);
    client.println();

    // dbg(API, "CALLED API GET PRINTER DATA");
    _dbgA("CALLED API GET PRINTER DATA");
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_getCurrentProduction(ClientType &client)
{
    DynamicJsonDocument json(512);
    String str = String();

    prodStruct_t tmp(_prods.getCurrentProd());

    json["currentProductCode"] = tmp.code;
    json["currentProductName"] = tmp.name;
    json["currentProductCustomer"] = tmp.customer;

    serializeJsonPretty(json, str);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(str.length());
    client.println();
    client.print(str);
    client.println();

    // dbg(API, "CALLED API GET CURRENT PRODUCTION");
    _dbgA("CALLED API GET CURRENT PRODUCTION");
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_getError(ClientType &client)
{
    std::vector<alarmStruct_t> tmp(_alarms.getAlarmsVect());

    uint16_t jsonSize = 256 * tmp.size();

    DynamicJsonDocument json(jsonSize);
    String str;

    JsonArray alarmObj = json.createNestedArray("errors");

    for (uint8_t index = 0; index < tmp.size(); index++)
    {
        JsonObject singleError = alarmObj.createNestedObject();

        singleError["code"] = tmp.at(index).code;
        singleError["name"] = tmp.at(index).name;
        singleError["count"] = tmp.at(index).counter;
    }

    serializeJsonPretty(json, str);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(str.length());
    client.println();
    client.print(str);
    client.println();

    // dbg(API, "CALLED API GET ERROR");
    _dbgA("CALLED API GET ERROR");
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_getWorkingUser(ClientType &client)
{
    DynamicJsonDocument json(256);
    String str;

    int8_t indexWU = _users.getIndexWorkingUser();
    std::vector<userStruct_t> tmp(_users.getUserVect());

    if (indexWU >= 0)
    {
        json["username"] = String(tmp.at(indexWU).username);
        json["autologin"] = tmp.at(indexWU).autologin;
    }
    else
    {
        json["username"] = "";
        json["autologin"] = false;
    }

    serializeJsonPretty(json, str);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(str.length());
    client.println();
    client.println(str);

    // dbg(API, "CALLED API GET WORKING USER");
    _dbgA("CALLED API GET WORKING USER");
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_getProcessStatus(ClientType &client)
{
    String res = String();

    serializeJsonPretty(getProcessStatus(), res);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/plain");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(res.length());
    client.println();
    client.print(res);
    client.println();
}

// POST

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postAddUser(ClientType &client, String body)
{
    const char *json = body.c_str();
    StaticJsonDocument<200> doc;
    deserializeJson(doc, json);

    const char *user = doc["username"];
    char username[USER_USERNAME_SIZE];
    memcpy(username, user, USER_USERNAME_SIZE);

    const char *pwd = doc["password"];
    char pass[USER_PASS_SIZE];
    memcpy(pass, pwd, USER_PASS_SIZE);

    bool isAdmin = doc["isAdmin"];
    bool res = false;

    _dbgA("username: " + String(username));
    _dbgA("password: " + String(pass));
    _dbgA("is admin? " + String(isAdmin));

    res = addUser(username, pass, isAdmin);
    if (res)
        _storeUser();

    String resStr = (res) ? "Done" : ("User: " + String(username) + " already stored.");

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/plain");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(resStr.length());
    client.println();
    client.print(resStr);
    client.println();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postLogin(ClientType &client, String body)
{
    const char *jsonIn = body.c_str();
    StaticJsonDocument<200> doc;
    uint8_t indexInUse = 255;
    String res = String();
    std::vector<userStruct_t> tmp(_users.getUserVect());

    deserializeJson(doc, jsonIn);
    const char *user = doc["username"];
    char username[USER_USERNAME_SIZE];
    memcpy(username, user, USER_USERNAME_SIZE);

    const char *pwd = doc["password"];
    char pass[USER_PASS_SIZE];
    memcpy(pass, pwd, USER_PASS_SIZE);

    res = login(username, pass);

    DynamicJsonDocument jsonOut(512);

    jsonOut["res"] = res;
    if (strcmp(res.c_str(), "Done") == 0 || strcmp(res.c_str(), "User already logged in") == 0)
    {
        for (uint8_t index = 0; index < tmp.size(); index++)
        {
            if (strcmp(username, tmp.at(index).username) == 0)
            {
                indexInUse = index;
                break;
            }
        }
        jsonOut["isAdmin"] = tmp.at(indexInUse).isAdmin;
    }

    res = String();

    serializeJsonPretty(jsonOut, res);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(res.length());
    client.println();
    client.print(res);
    client.println();

    if (indexInUse != 255)
        _dbgA("User logged: " + String(tmp.at(indexInUse).username));
    else
        _dbgA("User logged: ");
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postSetWorkingUser(ClientType &client, String body)
{
    const char *jsonIn = body.c_str();
    StaticJsonDocument<200> doc;
    String res = String();

    deserializeJson(doc, jsonIn);
    const char *user = doc["username"];
    char username[USER_USERNAME_SIZE];
    memcpy(username, user, USER_USERNAME_SIZE);

    bool autologinIn = doc["autologin"];
    bool resBool = false;

    if (autologinIn)
    {
        resBool = setDefaultWorkingUser(username);
    }
    else
    {
        resBool = setWorkingUser(username);
    }

    if (resBool)
    {
        res = "Done";
        _storeUser();
    }
    else
        res = "Failed";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/plain");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(res.length());
    client.println();
    client.print(res);
    client.println();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postUpdateSensor(ClientType &client, String body)
{
    const char *jsonIn = body.c_str();
    String res = String();
    StaticJsonDocument<200> doc;
    deserializeJson(doc, jsonIn);

    uint8_t codeIn = (uint8_t)doc["code"];
    const char *nameIn = doc["name"];
    char name[SENSOR_NAME_SIZE];
    memcpy(name, nameIn, SENSOR_NAME_SIZE);

    _dbgA("code in: " + String(codeIn));

    // res = (updateSensName(codeIn, name) ? "Done" : "Failed");

    if (updateSensName(codeIn, name))
    {
        res = "Done";
        _storeSens();
    }
    else
        res = "Failed";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/plain");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(res.length());
    client.println();
    client.print(res);
    client.println();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postAddProduct(ClientType &client, String body)
{
    const char *jsonIn = body.c_str();
    StaticJsonDocument<512> doc;
    deserializeJson(doc, jsonIn);

    const char *newProductCodeIn = doc["newProductCode"];
    char newProductCode[PROD_CODE_SIZE];
    memcpy(newProductCode, newProductCodeIn, PROD_CODE_SIZE);

    const char *newProductNameIn = doc["newProductName"];
    char newProductName[PROD_NAME_SIZE];
    memcpy(newProductName, newProductNameIn, PROD_NAME_SIZE);

    const char *newCustomerIn = doc["newCustomer"];
    char newCustomer[PROD_CSTR_SIZE];
    memcpy(newCustomer, newCustomerIn, PROD_CSTR_SIZE);

    bool res = addProd(newProductCode, newProductName, newCustomer);
    if (res)
        _storeProd();

    String resStr = (res) ? "Done" : ("Product: " + String(newProductCode) + " already stored.");

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/plain");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(resStr.length());
    client.println();
    client.print(resStr);
    client.println();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postUpdateUser(ClientType &client, String body)
{
    const char *jsonIn = body.c_str();
    StaticJsonDocument<200> doc;
    deserializeJson(doc, jsonIn);

    const char *usernameIn = doc["username"];
    char username[USER_USERNAME_SIZE];
    memcpy(username, usernameIn, USER_USERNAME_SIZE);

    bool deleteIn = doc["delete"];

    const char *passIn = doc["password"];
    char pass[USER_PASS_SIZE];
    memcpy(pass, passIn, USER_PASS_SIZE);

    bool isAdminIn = doc["isAdmin"];
    bool resRemoved = false;
    String res = String();

    if (deleteIn)
    {
        resRemoved = removeUser(username);
    }
    else
    {
        bool res1 = updateUserPass(username, pass);
        bool res2 = updateUserIsAdmin(username, isAdminIn);
        if (res1 || res2)
        {
            res = "Done";
            _storeUser();
        }
        else
            res = "Failed";
    }

    if (deleteIn && resRemoved)
        res = "User removed: " + String(username);
    else if (deleteIn && !resRemoved)
        res = "User not found";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/plain");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(res.length());
    client.println();
    client.print(res);
    client.println();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postSetPrinterData(ClientType &client, String body)
{
    const char *jsonIn = body.c_str();
    StaticJsonDocument<200> doc;
    deserializeJson(doc, jsonIn);

    bool enableIn = doc["enable"];
    bool enableDateIn = doc["enableDate"];
    bool enableUserIn = doc["enableUser"];

    const char *manualUserIn = doc["manualUser"];
    char manualUser[USER_USERNAME_SIZE];
    memcpy(manualUser, manualUserIn, USER_USERNAME_SIZE);

    bool enableCustomerIn = doc["enableCustomer"];

    String userTmp = String();
    char buffer[64];

    _printer.setUserStr(manualUser);

    printerStruct_t tmpPrinter(_printer.getPrinter());
    std::vector<userStruct_t> tmpUser(_users.getUserVect());

    sprintf(buffer, "enable: %d | enableDate: %d | enableUser: %d | enableCustomer: %d", enableIn, enableDateIn, enableUserIn, enableCustomerIn);
    _dbgA(buffer);
    setPrinter(enableDateIn, enableCustomerIn, enableUserIn);
    (enableIn) ? enablePrinter() : disablePrinter();

    for (uint8_t index = 0; index < tmpUser.size(); index++)
    {
        if (strcmp(tmpPrinter.userStr, tmpUser.at(index).username) == 0)
        {
            userTmp = "";
            break;
        }
        else
        {
            if (index == tmpUser.size() - 1)
            {
                userTmp = String(manualUser);
                // memcpy(_printerData.userStr, userTmp.c_str(), USER_USERNAME_SIZE);
                _printer.setUserStr(manualUser);
                _storePrinter();
                //_storeDataPrinterInFlash(_printerData);
            }
        }
    }

    tmpPrinter = _printer.getPrinter();

    DynamicJsonDocument jsonOut(2048);
    String str;

    jsonOut["enable"] = tmpPrinter.enable;
    jsonOut["enableDate"] = tmpPrinter.printDate;
    jsonOut["enableUser"] = tmpPrinter.printUser;
    jsonOut["user"] = userTmp;
    jsonOut["enableCustomer"] = tmpPrinter.printCstr;
    jsonOut["customer"] = tmpPrinter.customerStr;
    jsonOut["productCode"] = tmpPrinter.productCode;
    jsonOut["productName"] = tmpPrinter.productName;

    serializeJsonPretty(jsonOut, str);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: application/json");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(str.length());
    client.println();
    client.print(str);
    client.println();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postUpdateProduct(ClientType &client, String body)
{
    const char *jsonIn = body.c_str();
    StaticJsonDocument<200> doc;
    deserializeJson(doc, jsonIn);
    bool deleteIn = doc["delete"];

    const char *codeIn = doc["productCode"];
    char code[PROD_CODE_SIZE];
    memcpy(code, codeIn, PROD_CODE_SIZE);

    const char *nameIn = doc["productName"];
    char name[PROD_NAME_SIZE];
    memcpy(name, nameIn, PROD_NAME_SIZE);

    const char *customerIn = doc["customer"];
    char cstr[PROD_CSTR_SIZE];
    memcpy(cstr, customerIn, PROD_CSTR_SIZE);

    bool resRemoved = false;
    bool resUpdated = false;
    String res = String();

    if (deleteIn)
    {
        resRemoved = removeProd(code);
    }
    else
    {
        resUpdated = (updateProdName(code, name) || updateProdCustomer(code, cstr));
        res = (resUpdated) ? "Done" : "Failed";
    }

    if (deleteIn && resRemoved)
        res = "Product removed: " + String(code);
    else if (deleteIn && !resRemoved)
        res = "Product not found";

    if (resRemoved || resUpdated)
        _storeProd();

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/plain");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(res.length());
    client.println();
    client.print(res);
    client.println();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postSetProduction(ClientType &client, String body)
{
    const char *jsonIn = body.c_str();
    StaticJsonDocument<200> doc;
    deserializeJson(doc, jsonIn);

    const char *productCode = doc["productCode"];
    char code[PROD_CODE_SIZE];
    memcpy(code, productCode, PROD_CODE_SIZE);

    String res = String();
    String contentType = String();

    printerStruct_t tmpPrinter(_printer.getPrinter());

    if (!setCurrentProd(code))
    {
        res = "Product selected not stored";
        contentType = "text/plain";
    }
    else
    {
        DynamicJsonDocument jsonOut(512);
        _setPrinterProdData();
        jsonOut["customer"] = tmpPrinter.customerStr;
        jsonOut["productCode"] = tmpPrinter.productCode;
        jsonOut["productName"] = tmpPrinter.productName;

        serializeJsonPretty(jsonOut, res);
        contentType = "application/json";
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: " + contentType);
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(res.length());
    client.println();
    client.print(res);
    client.println();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postAddError(ClientType &client, String body)
{
    const char *jsonIn = body.c_str();
    StaticJsonDocument<200> doc;
    deserializeJson(doc, jsonIn);

    const char *codeIn = doc["code"];
    char code[ALARM_CODE_SIZE];
    memcpy(code, codeIn, ALARM_CODE_SIZE);

    const char *nameIn = doc["name"];
    char name[ALARM_NAME_SIZE];
    memcpy(name, nameIn, ALARM_NAME_SIZE);

    String res = String();

    // (addAlarm(code, name)) ? res = "Done" : res = "Failed";

    if (addAlarm(code, name))
    {
        res = "Done";
        _storeAlarm();
    }
    else
        res = "Failed";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/plain");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(res.length());
    client.println();
    client.print(res);
    client.println();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postUpdateError(ClientType &client, String body)
{
    const char *jsonIn = body.c_str();
    StaticJsonDocument<200> doc;
    deserializeJson(doc, jsonIn);

    const char *codeIn = doc["code"];
    char code[ALARM_CODE_SIZE];
    memcpy(code, codeIn, ALARM_CODE_SIZE);

    const char *newName = doc["name"];
    char name[ALARM_NAME_SIZE];
    memcpy(name, newName, ALARM_NAME_SIZE);

    String res = String();

    // (updateAlarmName(code, name)) ? res = " Done" : res = "Failed";

    if (updateAlarmName(code, name))
    {
        res = " Done";
        _storeAlarm();
    }
    else
        res = "Failed";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/plain");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(res.length());
    client.println();
    client.print(res);
    client.println();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_postUpdateRtc(ClientType &client, String body)
{
    const char *jsonIn = body.c_str();
    StaticJsonDocument<200> doc;
    deserializeJson(doc, jsonIn);
    uint32_t epoch = doc["epoch"];
    String res = String();

    res = updateRtc(epoch);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type: text/plain");
    client.println("Connection: close");
    client.print("Content-length: ");
    client.println(res.length());
    client.println();
    client.print(res);
    client.println();
}
