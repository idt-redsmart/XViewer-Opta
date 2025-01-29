#ifndef X_VIEWER_H
#define X_VIEWER_H

#include "def.h"
#include "XViewerStruct.h"
#include "sensors.h"
#include "user.h"
#include "production.h"
#include "alarm.h"
#include "printer.h"
#include "fs.h"

#define DBG(type, object) Serial.println("[" + String(type) +"]\t"+ String(object))


template <typename ServerType, typename ClientType>
class XViewer
{
public:
    XViewer() : _server(SERVER_PORT), _serverIp(SERVER_IP) {}

    /**
     * @brief   Set the IP address of the web server that exposes the APIs to the app.
     *          Use the default network 'SSID' and 'password'.
     * @param serverIp: IPAddress object to be assigned to the web server.
     */
    void config(IPAddress serverIp);

    /**
     * @brief   Set the 'ssid' e 'password' of the web server that exposes the APIs to the app.
     *          Use the default IP address.
     * @param ssid: ssid to be assigned to the network.
     * @param pass: Password to be assigned to the network.
     */
    void config(String ssid, String pass);

    /**
     * @brief   Set IPAddress, SSID, and password to be assigned to the web server exposing APIs to the app.
     * @param serverIp: IPAddress object to be assigned to the web server.
     * @param ssid: ssid to be assigned to the network.
     * @param pass: Password to be assigned to the network.
     */
    void config(IPAddress serverIp, String ssid, String pass);

    /**
     * @brief   Initialize the WiFi access point (WiFiAP) and web server. Use this in setup.
     *          Set credentials and initialize data for various sections through configuration files.
     *          Once initialization is complete, turn on the status LED in green to indicate that the library has finished the setup.
     */
    void init();

    /**
     * @brief   With this loop method, the web server remains listening for new clients and new requests.
                Once the request is read character by character, it concatenates a string containing the URI of the request.
                This string is used to switch between various responses to be sent back to the client, and it also saves an index that is passed as a parameter to "_sendResponse".
    */
    void handleClient();

    /**
     * @brief   Check the current status of the WiFi.
     * @return  Return the current status of the WiFi.
     */
    uint8_t getWifiStatus() { return WiFi.status(); }

    // SENSORS
    /**
     * @brief   Return the 'std::vector' containing sensor structures.
     */
    std::vector<sensorStruct_t> getSensorsVect();

    /**
     * @brief   Prints to the serial all the sensor structures present in the vector.
     */
    void printSensors();

    /**
     * @brief   Allows adding a sensor to the vector by specifying: code, name, and type.
     *          Uses the 'code' parameter as a unique identifier; therefore, it is not possible to add two sensors with the same code.
     *          The state is set to false by default.
     * @param code: Unique code of the sensor.
     * @param name: Descriptive name of the sensor.
     * @param type: Sensor type.
     * @return  True if the sensors was added successfully, false if the add operation failed.
     */
    bool addSensor(uint8_t code, char name[SENSOR_NAME_SIZE], char type[SENSOR_TYPE_SIZE]);

    /**
     * @brief   Remove a single sensor identified by the 'code' parameter.
     * @param code: Unique code of the sensor to remove.
     * @return  True if the sensors was removed successfully, false if the remove operation failed.
     */
    bool removeSensor(uint8_t code);

    /**
     * @brief   Update the 'name' parameter of the sensor identified by the 'code' parameter.
     * @param code: Unique code of the sensor to update.
     * @param newName: New 'name' to be assigned to the sensor.
     * @return  True if the sensors name was updated successfully, false if the update operation failed.
     */
    bool updateSensName(uint8_t code, char newName[SENSOR_NAME_SIZE]);

    /**
     * @brief   Update the 'type' parameter of the sensor identified by the 'code' parameter.
     * @param code: Unique code of the sensor to update.
     * @param newType: New 'type' to be assigned to the sensor.
     * @return  True if the sensor type was updated successfully, false if the update operation failed.
     */
    bool updateSensType(uint8_t code, char newType[SENSOR_TYPE_SIZE]);

    /**
     * @brief   Update the 'state' parameter of the sensor identified by the 'code' parameter.
     * @param code: Unique code of the sensor to update.
     * @param newState: New 'state' to be assigned to the sensor.
     * @return  True if the sensor state was updated successfully, false if the update operation failed.
     */
    bool updateSensState(uint8_t code, bool newState);

    /**
     * @brief   Identify a single sensor structure in the vector using the 'code' and return it.
     * @param code: Unique code of the sensor to be returned.
     * @return  The requested structure if present.
     */
    sensorStruct_t getSensor(uint8_t code);

    // USER
    /**
     * @brief   Return the 'std::vector' containing users structures.
     */
    std::vector<userStruct_t> getUserVect();

    /**
     * @brief   Prints to the serial all the users structures present in the vector.
     */
    void printUsers();

    /**
     * @brief   Allows adding a user to the vector by specifying: username, password, the permission level and the 'autologin' flag.
     *          Uses the 'username' parameter as a unique identifier; therefore, it is not possible to add two users with the same username.
     *          The autologin is set to false by default.
     * @param username: Unique username of the user.
     * @param pass: Password dell'utente.
     * @param isAdmin: If set to true, the user will be defined as 'admin'; otherwise, 'operator'.
     * @param autologin: This flag determines whether the user is set as the default working user. The library handles the parameter internally but can be specified.
     * @return  True if the user was added successfully, false if the add operation failed.
     */
    bool addUser(char username[USER_USERNAME_SIZE], char pass[USER_PASS_SIZE], bool isAdmin, bool autologin = false);

    /**
     * @brief   Remove a single user identified by the 'username' parameter.
     * @param username: Unique username of the user to remove.
     * @return  True if the user was removed successfully, false if the remove operation failed.
     */
    bool removeUser(char username[USER_USERNAME_SIZE]);

    /**
     * @brief   Update the 'pass' parameter of the user identified by the 'username' parameter.
     * @param username: Unique username of the user to update.
     * @param newPass: New 'pass' to be assigned to the user.
     * @return  True if the user password was updated successfully, false if the update operation failed.
     */
    bool updateUserPass(char username[USER_USERNAME_SIZE], char newPass[USER_PASS_SIZE]);

    /**
     * @brief   Update the 'isAdmin' parameter of the user identified by the 'username' parameter.
     * @param username: Unique username of the user to update.
     * @param newIsAdmin: New 'isAdmin' to be assigned to the user.
     * @return  True if the user permission was updated successfully, false if the update operation failed.
     */
    bool updateUserIsAdmin(char username[USER_USERNAME_SIZE], bool newIsAdmin);

    /**
     * @brief   Update the 'autologin' parameter of the user identified by the 'username' parameter.
     * @param username: Unique username of the user to update.
     * @param newAutologin: New 'autologin' to be assigned to the user.
     * @return  True if the user autologin was updated successfully, false if the update operation failed.
     */
    bool updateUserAutologin(char username[USER_USERNAME_SIZE], bool newAutologin);

    /**
     * @brief   Identify a single user structure in the vector using the 'username' and return it.
     * @param username: Unique username of the user to be returned.
     * @return  The requested structure if present.
     */
    userStruct_t getUser(char username[USER_USERNAME_SIZE]);

    /**
     * @brief   Used for credential verification to log in the user.
     * @param username: Username of the user to log in.
     * @param pass: Password of the user to log in.
     * @return  Return the outcome of the operation as a string, also used as a response to the app's POST.
     */
    String login(char username[USER_USERNAME_SIZE], char pass[USER_PASS_SIZE]);

    /**
     * @brief   Set the current operator and use the username as a parameter for the printer's data structure.
     * @param username: Username of the user to set as the working user.
     * @return  True if the operator was set successfully.
     */
    bool setWorkingUser(char username[USER_USERNAME_SIZE]);

    /**
     * @brief   Return the current working user.
     * @return  Structure of the current working user.
     */
    userStruct_t getWorkingUser();

    /**
     * @brief   Similar to 'setWorkingUser' but allows setting the user as the default working user, automatically logging them in as the working user.
     *          The management of this parameter is currently to be handled externally.
     *          Set the 'autologin' flag of the specified user to true and that of all other users to false, creating a single default operator.
     * @param username: Username of the user to set as the default working user.
     * @return  True if the operator was set successfully.
     */
    bool setDefaultWorkingUser(char username[USER_USERNAME_SIZE]);

    /**
     * @brief   Return the current default working user.
     * @return  Structure of the current default working user.
     */
    userStruct_t getDefaultWorkingUser();

    // PROD
    /**
     * @brief   Return the 'std::vector' containing production structures.
     */
    std::vector<prodStruct_t> getProdVect();

    /**
     * @brief   Prints to the serial all the production structures present in the vector.
     */
    void printProds();

    /**
     * @brief   Allows adding a production to the vector by specifying: code, name and the customer.
     *          Uses the 'code' parameter as a unique identifier; therefore, it is not possible to add two productions with the same code.
     * @param code: Unique code of the production.
     * @param name: Descriptive name of the production.
     * @param customer: Customer to assign the production to.
     * @return  True if the production was added successfully, false if the add operation failed.
     */
    bool addProd(char code[PROD_CODE_SIZE], char name[PROD_NAME_SIZE], char customer[PROD_CSTR_SIZE]);

    /**
     * @brief   Remove a single production identified by the 'code' parameter.
     * @param code: Unique code of the production to remove.
     * @return  True if the production was removed successfully, false if the remove operation failed.
     */
    bool removeProd(char code[PROD_CODE_SIZE]);

    /**
     * @brief   Update the 'name' parameter of the production identified by the 'code' parameter.
     * @param code: Unique code of the production to update.
     * @param newName: New 'name' to be assigned to the production.
     * @return  True if the production name was updated successfully, false if the update operation failed.
     */
    bool updateProdName(char code[PROD_CODE_SIZE], char newName[PROD_NAME_SIZE]);

    /**
     * @brief   Update the 'customer' parameter of the production identified by the 'code' parameter.
     * @param code: Unique code of the production to update.
     * @param newCstr: New 'customer' to be assigned to the production.
     * @return  True if the production customer was updated successfully, false if the update operation failed.
     */
    bool updateProdCustomer(char code[PROD_CODE_SIZE], char newCstr[PROD_CSTR_SIZE]);

    /**
     * @brief   Update a counter for the specified production.
     *          Specify the counter to update and the new value of the counter.
     * @param code: Unique code of the production to update.
     * @param counter: Counter to update (T_OK, T_NOK, P_OK, P_NOK)
     * @param newCounterVal: "New value to assign to the counter.
     * @return  True if the indicated counter was found and updated successfully.
     */
    bool updateProdCounter(const char* code, uint8_t counter, uint32_t newCounterVal);

    /**
     * @brief   Increment the counter by one for the specified production.
     *          Specify the counter to increment.
     * @param code: Unique code of the production to increment.
     * @param counter: Counter to increment (T_OK, T_NOK, P_OK, P_NOK)
     * @return  True if the indicated counter was found and incremented successfully.
     */
    bool updateProdCounter(const char* code, uint8_t counter);

    /**
     * @brief   Return the structure of the current production set.
     */
    prodStruct_t getCurrentProd();

    /**
     * @brief   Set the production indicated by the code as the current production.
     *          The production will be used in print data, and the app will display its respective counters.
     * @param code: Unique code of the production to set.
     * @return  True if the production was found and set successfully.
     */
    bool setCurrentProd(char code[PROD_CODE_SIZE]);

    // ALARM
    /**
     * @brief   Return the 'std::vector' containing alarm structures.
     */
    std::vector<alarmStruct_t> getAlarmsVect();

    /**
     * @brief   Allows adding an alarm to the vector by specifying: code and name.
     *          Uses the 'code' parameter as a unique identifier; therefore, it is not possible to add two alarm with the same code.
     * @param code: Unique code of the alarm.
     * @param name: Descriptive name of the alarm.
     * @return  True if the alarm was added successfully, false if the add operation failed.
     */
    bool addAlarm(char code[ALARM_CODE_SIZE], char name[ALARM_NAME_SIZE]);

    /**
     * @brief   Remove a single alarm identified by the 'code' parameter.
     * @param code: Unique code of the alarm to remove.
     * @return  True if the alarm was removed successfully, false if the remove operation failed.
     */
    bool removeAlarm(char code[ALARM_CODE_SIZE]);

    /**
     * @brief   Update the 'name' parameter of the alarm identified by the 'code' parameter.
     * @param code: Unique code of the alarm to update.
     * @param newName: New 'name' to be assigned to the alarm.
     * @return  True if the alarm name was updated successfully, false if the update operation failed.
     */
    bool updateAlarmName(char code[ALARM_CODE_SIZE], char newName[ALARM_NAME_SIZE]);

    /**
     * @brief   Update the 'counter' parameter of the alarm identified by the 'code' parameter.
     * @param code: Unique code of the alarm to update.
     * @param increase: Increment value of the counter, default is 1.
     * @return  True if the alarm counter was updated successfully, false if the update operation failed.
     */
    bool updateAlarmCounter(char code[ALARM_CODE_SIZE], uint16_t increase = 1);

    /**
     * @brief   Prints to the serial all the alarm structures present in the vector.
     */
    void printAlarms();

    // PRINTER
    /**
     * @brief   Return the structure with the current settings of the printer.
     */
    printerStruct_t getCurrentPrinter(bool printDbg = false);

    /**
     * @brief   Reset print settings to default values.
     */
    void clearPrinterStruct();

    /**
     * @brief   Enable the printer. Other print parameters can be specified.
     * @param enableDate: Enable printing of the current date.
     * @param enableCustomer: Enable printing of the customer of the currently selected production.
     * @param enableUser: Enable printing of the operator currently set as the working user or specified by the app.
     */
    void enablePrinter(bool enableDate = false, bool enableCustomer = false, bool enableUser = false);

    /**
     * @brief   Same as 'enableprinter' but does not enable the printer.
     *          Can be used to set print settings for later use.
     */
    void setPrinter(bool enableDate, bool enableCustomer, bool enableUser);

    /**
     * @brief Disable the printer.
     */
    void disablePrinter();

    // DBG
    /**
     * @brief Set the debug for the 'alarm' section, disabled by default.
     * @param dbgOn: If true, debug is activated, default is true.
     */
    void setAlarmDbg(bool dbgOn = true) { _alarms.setDbg(dbgOn); }

    /**
     * @brief Set the debug for the 'prod' section, disabled by default.
     * @param dbgOn: If true, debug is activated, default is true.
     */
    void setProdDbg(bool dbgOn = true) { _prods.setDbg(dbgOn); }

    /**
     * @brief Set the debug for the 'sensor' section, disabled by default.
     * @param dbgOn: If true, debug is activated, default is true.
     */
    void setSensDbg(bool dbgOn = true) { _sensors.setDbg(dbgOn); }

    /**
     * @brief Set the debug for the 'user' section, disabled by default.
     * @param dbgOn: If true, debug is activated, default is true.
     */
    void setUserDbg(bool dbgOn = true) { _users.setDbg(dbgOn); }

    /**
     * @brief Set the debug for the 'printer' section, disabled by default.
     * @param dbgOn: If true, debug is activated, default is true.
     */
    void setPrinterDbg(bool dbgOn = true);

    // --------------- RTC ---------------
    /**
     * @brief This method updates the RTC epoch using the value passed as a parameter.
     * @param epoch: value of "epoch" to set, in seconds.
     * @return Return of 'getCurrentTime' called after the set.
     */
    String updateRtc(uint32_t epoch);

    /**
     * @brief   It reads the current time from the previously set RTC.
     *          Formats the date as (dd/MM/yyyy) and the time as (hh:mm:ss), then prints it to the serial output.
     * @return String with formatted date and time.
     */
    String getCurrentTime();

    // --------------- PROCESS STATUS ---------------
    /**
     * @brief   Set the current process phase to one of the 4 predefined phases.
     *          The process phase should be updated during the process and displayed on the app.
     * @param status: Current phase to set (PROCESS_IDLE, PROCESS_ONGOING, PROCESS_OK or PROCESS_ERROR).
     * @param alarmCode: Alarm code to display in case of PROCESS_ERROR.
     */
    bool setProcessStatus(processPhaseEnum status, String alarmCode = "");

    /**
     * @brief   Return a JSON indicating the current process phase and the alarm if present.
     */
    StaticJsonDocument<200> getProcessStatus();

    void setApiDbg(bool dbgOn = true) { _dbgAOn = dbgOn; }

    // DELETE CONFIG
    /**
     * @brief   Delete the current sensor configuration used to initialize data at startup.
     *          Use it to undo changes made through the app.
     *          It is necessary to rerun a setup to repopulate the vectors.
     */
    bool deleteSensConfig();

    /**
     * @brief   Delete the current user configuration used to initialize data at startup.
     *          Use it to undo changes made through the app.
     *          It is necessary to rerun a setup to repopulate the vectors.
     */
    bool deleteUserConfig();

    /**
     * @brief   Delete the current alarm configuration used to initialize data at startup.
     *          Use it to undo changes made through the app.
     *          It is necessary to rerun a setup to repopulate the vectors.
     */
    bool deleteAlarmConfig();

    /**
     * @brief   Delete the current prod configuration used to initialize data at startup.
     *          Use it to undo changes made through the app.
     *          It is necessary to rerun a setup to repopulate the vectors.
     */
    bool deleteProdConfig();

    /**
     * @brief   Delete all configurations used to initialize data at startup.
     *          Undo changes made through the app.
     *          It is necessary to rerun a setup to repopulate the vectors.
     */
    bool deleteAllConfig();

private:
    ServerType _server;

    IPAddress _serverIp;
    String _ssid = SSID_WIFI; // your network SSID (name)
    String _pass = PASS_WIFI; // your network password (use for WPA, or use as key for WEP)

    int _wStatus = WL_IDLE_STATUS;
    bool _blinkLed = 0; // in led loop is used to know if status led have to blink or not
    uint32_t cMillis, sMillis;

    Sensors _sensors;
    Users _users;
    Production _prods;
    Alarm _alarms;
    Printer _printer;
    FsClass _fs;

    // --------------- PROCESS STATUS ---------------
    uint8_t _currentProcessStauts = 0;
    int8_t _currentProcessError = -1;

    bool _dbgPOn = false;
    bool _dbgAOn = false;

    void _ledLoop();
    void _checkWifiStatus();
    void _printWiFiStatus();
    void _checkPassLength(String pass);
    String _boolToString(bool data);
    void _setPrinterProdData();
    void _dbgP(String str = "");
    void _dbgA(String str = "");

    bool _initSens();
    bool _initUser();
    bool _initAlarm();
    bool _initProd();
    bool _initPrinter();


    bool _storeSens();
    bool _storeUser();
    bool _storeAlarm();
    bool _storeProd();
    bool _storePrinter();

    // API
    void _handleRoot(ClientType &client);
    void _handleNotFound(ClientType &client);
    void _getSensors(ClientType &client);
    void _getSensorsState(ClientType &client);
    void _getTotalCounters(ClientType &client);
    void _getPartialCounters(ClientType &client);
    void _getInfoUser(ClientType &client);
    void _getProduct(ClientType &client);
    void _getError(ClientType &client);
    void _getPrinterData(ClientType &client);
    void _getCurrentProduction(ClientType &client);
    void _getWorkingUser(ClientType &client);
    void _getFlashProgramCounter(ClientType &client);
    void _getProcessStatus(ClientType &client);

    void _postAddUser(ClientType &client, String body);
    void _postUpdateUser(ClientType &client, String body);
    void _postLogin(ClientType &client, String body);
    void _postSetWorkingUser(ClientType &client, String body);
    void _postUpdateSensor(ClientType &client, String body);
    void _postAddProduct(ClientType &client, String body);
    void _postUpdateProduct(ClientType &client, String body);
    void _postSetPrinterData(ClientType &client, String body);
    void _postSetProduction(ClientType &client, String body);
    void _postAddError(ClientType &client, String body);
    void _postUpdateError(ClientType &client, String body);
    void _postUpdateRtc(ClientType &client, String body);

    /**
     * @brief Based on the request received as a parameter, it switches between the different available methods to provide the correct response to the received request.
     * @param client: The "client" object from which the request has been received. Is used to send the response.
     * @param response: Enum of the response to be sent. (enum response)
     * @param body: The body received from the request, if the request is a POST. Default is an empty string "" in case of GET request.
     */
    void _sendResponse(ClientType &client, uint8_t response, String body = "");

    uint8_t _currentResponse = 0; // switch response index
};

#include "XViewer.tpp"

#endif