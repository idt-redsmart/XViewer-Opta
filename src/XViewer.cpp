
#include "XViewer.h"

void XViewer::config(IPAddress serverIp)
{
	_serverIp = serverIp;
}

void XViewer::config(String ssid, String pass)
{
	_ssid = ssid;
	_checkPassLength(pass);
	_pass = pass;
}

void XViewer::config(IPAddress serverIp, String ssid, String pass)
{
	_serverIp = serverIp;
	_ssid = ssid;
	_checkPassLength(pass);
	_pass = pass;
}

void XViewer::init()
{
	pinMode(LEDR, OUTPUT);
	pinMode(LEDG, OUTPUT);
	pinMode(LEDB, OUTPUT);

	digitalWrite(LEDR, 1);
	digitalWrite(LEDG, 1);
	digitalWrite(LEDB, 0);

	WiFi.config(_serverIp);

	// Create the Access point
	_wStatus = WiFi.beginAP(_ssid.c_str(), _pass.c_str());
	if (_wStatus != WL_AP_LISTENING)
	{
		while (true)
			;
	}

	// wait 1 seconds for connection:
	delay(1000);

	// start the web server
	_server.begin();

	// you're connected now, so print out the status
	_printWiFiStatus();

	cMillis = millis();

	_fs.init();
	_initSens();
	_initUser();
	_initAlarm();
	_initProd();

	digitalWrite(LEDR, 0);
}

void XViewer::handleClient()
{
	_checkWifiStatus();
	_ledLoop();

	uint8_t _lineCounter = 0;

	WiFiClient client = _server.available();
	if (client)
	{
		bool isGet = false;

		Serial.println("New client");

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

						// Serial.println("line: " + String(line));
						line = line.substring(line.indexOf('/') + 1, line.indexOf("HTTP") - 1);
						Serial.println("Incoming request: " + String(line));

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
						Serial.println(("BODY: " + body));

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
						Serial.println(line);

						_lineCounter++;

						if (line.indexOf("GET") >= 0)
						{
							isGet = true;

							// Serial.println("line: " + String(line));
							line = line.substring(line.indexOf('/') + 1, line.indexOf("HTTP") - 1);
							if (line.equals(""))
								line = "GET / ";
							Serial.println("Incoming request: " + String(line));

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
		Serial.println("Client disconnected");
	}
}

// SENSOR
std::vector<sensorStruct_t> XViewer::getSensorsVect()
{
	return _sensors.getSensorsVect();
}

void XViewer::printSensors()
{
	_sensors.printSensors();
}

bool XViewer::addSensor(uint8_t code, char name[SENSOR_NAME_SIZE], char type[SENSOR_TYPE_SIZE])
{
	return _sensors.addSensor(code, name, type);
}

bool XViewer::removeSensor(uint8_t code)
{
	return _sensors.removeSensor(code);
}

bool XViewer::updateSensName(uint8_t code, char newName[SENSOR_NAME_SIZE])
{
	return _sensors.updateName(code, newName);
}

bool XViewer::updateSensType(uint8_t code, char newType[SENSOR_TYPE_SIZE])
{
	return _sensors.updateType(code, newType);
}

bool XViewer::updateSensState(uint8_t code, bool newState)
{
	return _sensors.updateState(code, newState);
}

sensorStruct_t XViewer::getSensor(uint8_t code)
{
	return _sensors.getSensor(code);
}

// USER
std::vector<userStruct_t> XViewer::getUserVect()
{
	return _users.getUserVect();
}

void XViewer::printUsers()
{
	_users.printUsers();
}

bool XViewer::addUser(char username[USER_USERNAME_SIZE], char pass[USER_PASS_SIZE], bool isAdmin, bool autologin)
{
	return _users.addUser(username, pass, isAdmin, autologin);
}

bool XViewer::removeUser(char username[USER_USERNAME_SIZE])
{
	return _users.removeUser(username);
}

bool XViewer::updateUserPass(char username[USER_USERNAME_SIZE], char newPass[USER_PASS_SIZE])
{
	return _users.updatePass(username, newPass);
}

bool XViewer::updateUserIsAdmin(char username[USER_USERNAME_SIZE], bool newIsAdmin)
{
	return _users.updateIsAdmin(username, newIsAdmin);
}

bool XViewer::updateUserAutologin(char username[USER_USERNAME_SIZE], bool newAutologin)
{
	return _users.updateAutologin(username, newAutologin);
}

userStruct_t XViewer::getUser(char username[USER_USERNAME_SIZE])
{
	return _users.getUser(username);
}

String XViewer::login(char username[USER_USERNAME_SIZE], char pass[USER_PASS_SIZE])
{
	return _users.login(username, pass);
}

bool XViewer::setWorkingUser(char username[USER_USERNAME_SIZE])
{
	return _users.setWorkingUser(username);
}

userStruct_t XViewer::getWorkingUser()
{
	return _users.getWorkingUser();
}

bool XViewer::setDefaultWorkingUser(char username[USER_USERNAME_SIZE])
{
	return _users.setDefaultWorkingUser(username);
}

userStruct_t XViewer::getDefaultWorkingUser()
{
	return _users.getDefaultWorkingUser();
}

// PROD
std::vector<prodStruct_t> XViewer::getProdVect()
{
	return _prods.getProdVect();
}

void XViewer::printProds()
{
	return _prods.print();
}

bool XViewer::addProd(char code[PROD_CODE_SIZE], char name[PROD_NAME_SIZE], char customer[PROD_CSTR_SIZE])
{
	return _prods.addProd(code, name, customer);
}

bool XViewer::removeProd(char code[PROD_CODE_SIZE])
{
	return _prods.removeProd(code);
}

bool XViewer::updateProdName(char code[PROD_CODE_SIZE], char newName[PROD_NAME_SIZE])
{
	return _prods.updateName(code, newName);
}

bool XViewer::updateProdCustomer(char code[PROD_CODE_SIZE], char newCstr[PROD_CSTR_SIZE])
{
	return _prods.updateCustomer(code, newCstr);
}

bool XViewer::updateProdCounter(char code[PROD_CODE_SIZE], uint8_t counter, uint32_t newCounterVal)
{
	return _prods.updateCounter(code, counter, newCounterVal);
}

bool XViewer::updateProdCounter(char code[PROD_CODE_SIZE], uint8_t counter)
{
	return _prods.updateCounter(code, counter);
}

prodStruct_t XViewer::getCurrentProd()
{
	return _prods.getCurrentProd();
}

bool XViewer::setCurrentProd(char code[PROD_CODE_SIZE])
{
	return _prods.setCurrentProd(code);
}

// ALARM
std::vector<alarmStruct_t> XViewer::getAlarmsVect()
{
	return _alarms.getAlarmsVect();
}

bool XViewer::addAlarm(char code[ALARM_CODE_SIZE], char name[ALARM_NAME_SIZE])
{
	return _alarms.addAlarm(code, name);
}

bool XViewer::removeAlarm(char code[ALARM_CODE_SIZE])
{
	return _alarms.removeAlarm(code);
}

bool XViewer::updateAlarmName(char code[ALARM_CODE_SIZE], char newName[ALARM_NAME_SIZE])
{
	return _alarms.updateName(code, newName);
}

bool XViewer::updateAlarmCounter(char code[ALARM_CODE_SIZE], uint16_t increase)
{
	return _alarms.updateCounter(code, increase);
}

void XViewer::printAlarms()
{
	return _alarms.print();
}

// PRINTER
void XViewer::clearPrinterStruct()
{
	return _printer.clearStruct();
}

void XViewer::enablePrinter(bool enableDate, bool enableCustomer, bool enableUser)
{
	return _printer.enable(enableDate, enableCustomer, enableUser);
}

void XViewer::setPrinter(bool enableDate, bool enableCustomer, bool enableUser)
{
	return _printer.set(enableDate, enableCustomer, enableUser);
}

void XViewer::disablePrinter()
{
	return _printer.disable();
}

// PRIVATE
void XViewer::_sendResponse(WiFiClient &client, uint8_t response, String body)
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

void XViewer::_ledLoop()
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
		if (digitalRead(ST_LED) != 0)
			digitalWrite(ST_LED, 0);
		return;
	}
}

void XViewer::_checkWifiStatus()
{
	if (_wStatus != WiFi.status())
	{
		_wStatus = WiFi.status();

		if (_wStatus == WL_AP_CONNECTED)
		{
			// a device has connected to the AP
			Serial.println("[WF]    Device connected to AP");
			_blinkLed = 1;
		}
		else
		{
			// a device has disconnected from the AP, and we are back in listening mode
			Serial.println("[WF]    Device disconnected to AP");

			_blinkLed = 0;
		}
	}
}

void XViewer::_printWiFiStatus()
{
	Serial.print("[WF]    IP address: ");
	Serial.println(WiFi.localIP());
	Serial.println("[WF]    SSID: " + String(WiFi.SSID()));
	Serial.println("[WF]    PASS: " + String(_pass));
}

void XViewer::_checkPassLength(String pass)
{
	if (pass.length() < 8)
	{
		Serial.println("Password must be at least 8 char!");
		while (1)
			;
	}
}

bool XViewer::_initSens()
{
	return _fs.readSensConfig(_sensors.getSensorsVect());
}

bool XViewer::_initUser()
{
	return _fs.readUserConfig(_users.getUserVect());
}

bool XViewer::_initAlarm()
{
	return _fs.readAlarmConfig(_alarms.getAlarmsVect());
}

bool XViewer::_initProd()
{
	return _fs.readProdConfig(_prods.getProdVect());
}

bool XViewer::_storeSens()
{
	return _fs.storeConfig(PATH_SENS_CONFIG, _sensors.vectToJson());
}

bool XViewer::_storeUser()
{
	return _fs.storeConfig(PATH_USER_CONFIG, _users.vectToJson());
}

bool XViewer::_storeAlarm()
{
	return _fs.storeConfig(PATH_ALARM_CONFIG, _alarms.vectToJson());
}

bool XViewer::_storeProd()
{
	return _fs.storeConfig(PATH_PROD_CONFIG, _prods.vectToJson());
}

bool XViewer::deleteSensConfig()
{
	if (_fs.deleteConfig(PATH_SENS_CONFIG))
	{
		Serial.println("Sensor config deleted");
		return true;
	}
	else
	{
		Serial.println("Fail to delete sensor config");
		return false;
	}
}

bool XViewer::deleteUserConfig()
{
	if (_fs.deleteConfig(PATH_USER_CONFIG))
	{
		Serial.println("User config deleted");
		return true;
	}
	else
	{
		Serial.println("Fail to delete user config");
		return false;
	}
}

bool XViewer::deleteAlarmConfig()
{
	if (_fs.deleteConfig(PATH_ALARM_CONFIG))
	{
		Serial.println("Alarm config deleted");
		return true;
	}
	else
	{
		Serial.println("Fail to delete alarm config");
		return false;
	}
}

bool XViewer::deleteProdConfig()
{
	if (_fs.deleteConfig(PATH_PROD_CONFIG))
	{
		Serial.println("Prod config deleted");
		return true;
	}
	else
	{
		Serial.println("Fail to delete prod config");
		return false;
	}
}

bool XViewer::deleteAllConfig()
{
	bool resA = deleteAlarmConfig();
	bool resP = deleteProdConfig();
	bool resS = deleteSensConfig();
	bool resU = deleteUserConfig();

	return (resA && resP && resS && resU);
}

void XViewer::_dbgA(String str)
{
	if (_dbgAOn)
		Serial.println("[API]\t" + str);
}

void XViewer::setPrinterDbg(bool dbgOn)
{
	_printer.setDbg(dbgOn);
	_dbgPOn = dbgOn;
}
