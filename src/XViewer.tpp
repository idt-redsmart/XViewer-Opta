#include "XViewer.h"


// Definizione esplicita delle istanze per forzare la compilazione
template class XViewer<WiFiServer, WiFiClient>;
template class XViewer<EthernetServer, EthernetClient>;

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::config(IPAddress serverIp)
{
	_serverIp = serverIp;
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::config(String ssid, String pass)
{
	_ssid = ssid;
	_checkPassLength(pass);
	_pass = pass;
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::config(IPAddress serverIp, String ssid, String pass)
{
	_serverIp = serverIp;
	_ssid = ssid;
	_checkPassLength(pass);
	_pass = pass;
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::init()
{
	pinMode(LEDR, OUTPUT);
	pinMode(LEDG, OUTPUT);
	pinMode(LEDB, OUTPUT);

	digitalWrite(LEDR, 1);
	digitalWrite(LEDG, 1);
	digitalWrite(LEDB, 0);

	if(std::is_same<ServerType, WiFiServer>::value)
	{
    	_dbgA("Using WifiServer");
		WiFi.config(_serverIp);
		// Create the Access point
		_wStatus = WiFi.beginAP(_ssid.c_str(), _pass.c_str());
		if (_wStatus != WL_AP_LISTENING)
		{
			while (true)
				;
		}
		// you're connected now, so print out the status
		_printWiFiStatus();

	} else if(std::is_same<ServerType, EthernetServer>::value)
	{
		byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // Indirizzo MAC unico
		Ethernet.begin(mac, _serverIp, _serverIp, _serverIp);
    	_dbgA("Using EthernetServer");
	}

	// wait 0.1 seconds for connection:
	delay(100);

	// start the web server
	_server.begin();

	cMillis = millis();

	_fs.init();
	_initSens();
	_initUser();
	_initAlarm();
	_initProd();
	_initPrinter();

	digitalWrite(LEDR, 0);
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::handleClient()
{
	if(std::is_same<ServerType, WiFiServer>::value)
		_checkWifiStatus();
	_ledLoop();

	uint8_t _lineCounter = 0;

	ClientType client = _server.available();
	if (client)
	{
		bool isGet = false;

		_dbgA("New client");

		bool currentLineIsBlank = true;

		String line = String();
		String body = String();

		while (client.connected())
		{
			if (client.available())
			{
				char c = client.read();
				line.concat(c);

				// SEND RESPONSE AFTER 2 \n
				if (c == '\n' && currentLineIsBlank)
				{
					if (isGet)
					{
						currentLineIsBlank = false;
						_sendResponse(client, _currentResponse);
						break;
					}
					else
					{
						currentLineIsBlank = false;

						line = line.substring(0, line.indexOf('\n'));

						// _dbgA("line: " + String(line));
						line = line.substring(line.indexOf('/') + 1, line.indexOf("HTTP") - 1);
						_dbgA("Incoming request: " + String(line));

						// SWITCH RESPONSE
						if (line.equals(P_ADD_USER))
							_currentResponse = response_postAddUser;
						else if (line.equals(P_UPDATE_USER))
							_currentResponse = response_postUpdateUser;
						else if (line.equals(P_LOGIN))
							_currentResponse = response_postLogin;
						else if (line.equals(P_SET_WORKING_USER))
							_currentResponse = response_postSetWorkingUser;
						else if (line.equals(P_UPDATE_SENSOR))
							_currentResponse = response_postUpdateSensor;
						else if (line.equals(P_ADD_PRODUCT))
							_currentResponse = response_postAddProduct;
						else if (line.equals(P_UPDATE_PRODUCT))
							_currentResponse = response_postUpdateProduct;
						else if (line.equals(P_SET_PRINTER))
							_currentResponse = response_postSetPrinterData;
						else if (line.equals(P_SET_PRODUCTION))
							_currentResponse = response_postSetCurrentProduction;
						else if (line.equals(P_ADD_ERROR))
							_currentResponse = response_postAddError;
						else if (line.equals(P_UPDATE_ERROR))
							_currentResponse = response_postUpdateError;
						else if (line.equals(P_UPDATE_RTC))
							_currentResponse = response_postUpdateRtc;

						// STORING BODY IN STRING
						while (client.available())
						{
							c = client.read();
							body.concat(c);
						}

						// SEND RESPONSE
						_dbgA(("BODY: " + body));

						_sendResponse(client, _currentResponse, body);
						break;
					}
				}

				if (c == '\n')
				{
					// new line
					currentLineIsBlank = true;

					if (_lineCounter == 0)
					{
						_dbgA(line);

						_lineCounter++;

						if (line.indexOf("GET") >= 0)
						{
							isGet = true;

							// _dbgA("line: " + String(line));
							line = line.substring(line.indexOf('/') + 1, line.indexOf("HTTP") - 1);
							if (line.equals(""))
								line = "GET / ";
							_dbgA("Incoming request: " + String(line));

							if (isspace(line[5]))
								_currentResponse = response_handleRoot;
							else if (line.equals(G_SENSOR))
								_currentResponse = response_getSensors;
							else if (line.equals(G_SENSOR_STATE))
								_currentResponse = response_getSensorsState;
							else if (line.equals(G_TOTAL_COUNTER))
								_currentResponse = response_getTotalCounters;
							else if (line.equals(G_PARTIAL_COUNTER))
								_currentResponse = response_getPartialCounters;
							else if (line.equals(G_USER))
								_currentResponse = response_getInfoUser;
							else if (line.equals(G_PRODUCT))
								_currentResponse = response_getProduct;
							else if (line.equals(G_PRINTER_DATA))
								_currentResponse = response_getPrinterData;
							else if (line.equals(G_PRODUCTION))
								_currentResponse = response_getCurrentProduction;
							else if (line.equals(G_ERROR))
								_currentResponse = response_getError;
							else if (line.equals(G_WORKING_USER))
								_currentResponse = response_getWorkingUser;
							else if (line.equals(G_FLASH_PROGRAM_COUNT))
								_currentResponse = response_getFlashProgramCounter;
							else if (line.equals(G_PROCESS_STATUS))
								_currentResponse = response_getProcessStatus;

							line = String();
						}
					}
				}
				else if (c != '\r')
				{
					// la riga non è vuota
					currentLineIsBlank = false;
				}
			}
		}
		delay(1);
		client.stop();
		_dbgA("Client disconnected");
	}
}

// SENSOR
template <typename ServerType, typename ClientType>
std::vector<sensorStruct_t> XViewer<ServerType, ClientType>::getSensorsVect()
{
	return _sensors.getSensorsVect();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::printSensors()
{
	_sensors.printSensors();
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::addSensor(uint8_t code, char name[SENSOR_NAME_SIZE], char type[SENSOR_TYPE_SIZE])
{
	return _sensors.addSensor(code, name, type);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::removeSensor(uint8_t code)
{
	return _sensors.removeSensor(code);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateSensName(uint8_t code, char newName[SENSOR_NAME_SIZE])
{
	return _sensors.updateName(code, newName);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateSensType(uint8_t code, char newType[SENSOR_TYPE_SIZE])
{
	return _sensors.updateType(code, newType);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateSensState(uint8_t code, bool newState)
{
	return _sensors.updateState(code, newState);
}

template <typename ServerType, typename ClientType>
sensorStruct_t XViewer<ServerType, ClientType>::getSensor(uint8_t code)
{
	return _sensors.getSensor(code);
}

// USER
template <typename ServerType, typename ClientType>
std::vector<userStruct_t> XViewer<ServerType, ClientType>::getUserVect()
{
	return _users.getUserVect();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::printUsers()
{
	_users.printUsers();
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::addUser(char username[USER_USERNAME_SIZE], char pass[USER_PASS_SIZE], bool isAdmin, bool autologin)
{
	return _users.addUser(username, pass, isAdmin, autologin);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::removeUser(char username[USER_USERNAME_SIZE])
{
	return _users.removeUser(username);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateUserPass(char username[USER_USERNAME_SIZE], char newPass[USER_PASS_SIZE])
{
	return _users.updatePass(username, newPass);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateUserIsAdmin(char username[USER_USERNAME_SIZE], bool newIsAdmin)
{
	return _users.updateIsAdmin(username, newIsAdmin);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateUserAutologin(char username[USER_USERNAME_SIZE], bool newAutologin)
{
	return _users.updateAutologin(username, newAutologin);
}

template <typename ServerType, typename ClientType>
userStruct_t XViewer<ServerType, ClientType>::getUser(char username[USER_USERNAME_SIZE])
{
	return _users.getUser(username);
}

template <typename ServerType, typename ClientType>
String XViewer<ServerType, ClientType>::login(char username[USER_USERNAME_SIZE], char pass[USER_PASS_SIZE])
{
	return _users.login(username, pass);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::setWorkingUser(char username[USER_USERNAME_SIZE])
{
	return _users.setWorkingUser(username);
}

template <typename ServerType, typename ClientType>
userStruct_t XViewer<ServerType, ClientType>::getWorkingUser()
{
	return _users.getWorkingUser();
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::setDefaultWorkingUser(char username[USER_USERNAME_SIZE])
{
	return _users.setDefaultWorkingUser(username);
}

template <typename ServerType, typename ClientType>
userStruct_t XViewer<ServerType, ClientType>::getDefaultWorkingUser()
{
	return _users.getDefaultWorkingUser();
}

// PROD
template <typename ServerType, typename ClientType>
std::vector<prodStruct_t> XViewer<ServerType, ClientType>::getProdVect()
{
	return _prods.getProdVect();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::printProds()
{
	return _prods.print();
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::addProd(char code[PROD_CODE_SIZE], char name[PROD_NAME_SIZE], char customer[PROD_CSTR_SIZE])
{
	return _prods.addProd(code, name, customer);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::removeProd(char code[PROD_CODE_SIZE])
{
	return _prods.removeProd(code);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateProdName(char code[PROD_CODE_SIZE], char newName[PROD_NAME_SIZE])
{
	return _prods.updateName(code, newName);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateProdCustomer(char code[PROD_CODE_SIZE], char newCstr[PROD_CSTR_SIZE])
{
	return _prods.updateCustomer(code, newCstr);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateProdCounter(const char* code, uint8_t counter, uint32_t newCounterVal)
{
	if(_prods.updateCounter(code, counter, newCounterVal) && _storeProd())
		return true;
	else
		return false;
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateProdCounter(const char* code, uint8_t counter)
{
	if(_prods.updateCounter(code, counter) && _storeProd())
		return true;
	else
		return false;
}

template <typename ServerType, typename ClientType>
prodStruct_t XViewer<ServerType, ClientType>::getCurrentProd()
{
	return _prods.getCurrentProd();
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::setCurrentProd(char code[PROD_CODE_SIZE])
{
	return _prods.setCurrentProd(code);
}

// ALARM
template <typename ServerType, typename ClientType>
std::vector<alarmStruct_t> XViewer<ServerType, ClientType>::getAlarmsVect()
{
	return _alarms.getAlarmsVect();
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::addAlarm(char code[ALARM_CODE_SIZE], char name[ALARM_NAME_SIZE])
{
	return _alarms.addAlarm(code, name);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::removeAlarm(char code[ALARM_CODE_SIZE])
{
	return _alarms.removeAlarm(code);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateAlarmName(char code[ALARM_CODE_SIZE], char newName[ALARM_NAME_SIZE])
{
	return _alarms.updateName(code, newName);
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::updateAlarmCounter(char code[ALARM_CODE_SIZE], uint16_t increase)
{
	return _alarms.updateCounter(code, increase);
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::printAlarms()
{
	return _alarms.print();
}

// PRINTER
template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::clearPrinterStruct()
{
	return _printer.clearStruct();
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::enablePrinter(bool enableDate, bool enableCustomer, bool enableUser)
{
	return _printer.enable(enableDate, enableCustomer, enableUser);
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::setPrinter(bool enableDate, bool enableCustomer, bool enableUser)
{
	return _printer.set(enableDate, enableCustomer, enableUser);
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::disablePrinter()
{
	return _printer.disable();
}

// PRIVATE
template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_sendResponse(ClientType &client, uint8_t response, String body)
{
	switch (response)
	{
	case response_handleRoot:
		// _handleRoot(client);
		break;
	case response_getSensors:
		_getSensors(client);
		break;
	case response_getSensorsState:
		_getSensorsState(client);
		break;
	case response_getTotalCounters:
		_getTotalCounters(client);
		break;
	case response_getPartialCounters:
		_getPartialCounters(client);
		break;
	case response_getInfoUser:
		_getInfoUser(client);
		break;
	case response_getProduct:
		_getProduct(client);
		break;
	case response_getPrinterData:
		_getPrinterData(client);
		break;
	case response_getCurrentProduction:
		_getCurrentProduction(client);
		break;
	case response_getError:
		_getError(client);
		break;
	case response_getWorkingUser:
		_getWorkingUser(client);
		break;
	case response_getFlashProgramCounter:
		// _getFlashProgramCounter(client);
		break;
	case response_getProcessStatus:
		_getProcessStatus(client);
		break;
		//
		// POST
		//
	case response_postAddUser:
		_postAddUser(client, body);
		break;
	case response_postUpdateUser:
		_postUpdateUser(client, body);
		break;
	case response_postLogin:
		_postLogin(client, body);
		break;
	case response_postSetWorkingUser:
		_postSetWorkingUser(client, body);
		break;
	case response_postUpdateSensor:
		_postUpdateSensor(client, body);
		break;
	case response_postAddProduct:
		_postAddProduct(client, body);
		break;
	case response_postUpdateProduct:
		_postUpdateProduct(client, body);
		break;
	case response_postSetPrinterData:
		_postSetPrinterData(client, body);
		break;
	case response_postSetCurrentProduction:
		_postSetProduction(client, body);
		break;
	case response_postAddError:
		_postAddError(client, body);
		break;
	case response_postUpdateError:
		_postUpdateError(client, body);
		break;
	case response_postUpdateRtc:
		_postUpdateRtc(client, body);
		break;

	default:
		// _handleNotFound(client);
		break;
	}

	_currentResponse = 0;
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_ledLoop()
{
	cMillis = millis();
	if (_blinkLed)
	{
		if (cMillis - sMillis >= LED_BLINK_TIME)
		{
			sMillis = cMillis;
			digitalWrite(ST_LED, !digitalRead(ST_LED));
			return;
		}
	}
	else
	{
		if (digitalRead(ST_LED) != 1)
			digitalWrite(ST_LED, 1);
		return;
	}
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_checkWifiStatus()
{
	if (_wStatus != WiFi.status())
	{
		_wStatus = WiFi.status();

		if (_wStatus == WL_AP_CONNECTED)
		{
			// a device has connected to the AP
			DBG("WIFI","Device connected to AP");
			_blinkLed = 1;
		}
		else
		{
			// a device has disconnected from the AP, and we are back in listening mode
			DBG("WIFI","Device disconnected to AP");
			_blinkLed = 0;
		}
	}
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_printWiFiStatus()
{
	DBG("WIFI","IP address: "+String(WiFi.localIP()));
	DBG("WIFI","SSID:  "+String(WiFi.SSID()));
	DBG("WIFI","PASS:  "+String(_pass));
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_checkPassLength(String pass)
{
	if (pass.length() < 8)
	{
		DBG("WIFI","ERROR! Password must be at least 8 char!");
		while (1)
			;
	}
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::_initSens()
{
	return _fs.readSensConfig(_sensors.getSensorsVect());
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::_initUser()
{
	return _fs.readUserConfig(_users.getUserVect());
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::_initAlarm()
{
	return _fs.readAlarmConfig(_alarms.getAlarmsVect());
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::_initProd()
{
	return _fs.readProdConfig(_prods.getProdVect());
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::_initPrinter()
{
	return _fs.readPrinterConfig(_printer.getPrinter());
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::_storeSens()
{
	return _fs.storeConfig(PATH_SENS_CONFIG, _sensors.vectToJson());
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::_storeUser()
{
	return _fs.storeConfig(PATH_USER_CONFIG, _users.vectToJson());
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::_storeAlarm()
{
	return _fs.storeConfig(PATH_ALARM_CONFIG, _alarms.vectToJson());
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::_storeProd()
{
	return _fs.storeConfig(PATH_PROD_CONFIG, _prods.vectToJson());
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::_storePrinter()
{
	return _fs.storeConfig(PATH_PRINTER_CONFIG, _printer.structToJson());
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::deleteSensConfig()
{
	if (_fs.deleteConfig(PATH_SENS_CONFIG))
	{
		DBG("SENSOR","Sensor config deleted");		
		return true;
	}
	else
	{
		DBG("SENSOR","Fail to delete sensor config");		
		return false;
	}
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::deleteUserConfig()
{
	if (_fs.deleteConfig(PATH_USER_CONFIG))
	{
		DBG("USER","User config deleted");		
		return true;
	}
	else
	{
		DBG("USER","Fail to delete user config");		
		return false;
	}
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::deleteAlarmConfig()
{
	if (_fs.deleteConfig(PATH_ALARM_CONFIG))
	{
		DBG("ALARM","Alarm config deleted");		
		return true;
	}
	else
	{
		DBG("ALARM","Fail to delete alarm config");		
		return false;
	}
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::deleteProdConfig()
{
	if (_fs.deleteConfig(PATH_PROD_CONFIG))
	{
		DBG("PROD","Prod config deleted");	
		return true;
	}
	else
	{
		DBG("PROD","Fail to delete prod config");	
		return false;
	}
}

template <typename ServerType, typename ClientType>
bool XViewer<ServerType, ClientType>::deleteAllConfig()
{
	bool resA = deleteAlarmConfig();
	bool resP = deleteProdConfig();
	bool resS = deleteSensConfig();
	bool resU = deleteUserConfig();

	return (resA && resP && resS && resU);
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::_dbgA(String str)
{
	if (_dbgAOn)
		Serial.println("[API]\t" + str);
}

template <typename ServerType, typename ClientType>
void XViewer<ServerType, ClientType>::setPrinterDbg(bool dbgOn)
{
	_printer.setDbg(dbgOn);
	_dbgPOn = dbgOn;
}
