#ifndef DEF_H
#define DEF_H

#include <Arduino.h>
#include <WiFi.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <mbed.h>
#include <mbed_mktime.h>
#include <vector>

#include "uri.h"

#define SERVER_PORT 9000
#define SERVER_IP "192.168.1.1"
#define SSID_WIFI "XViewerAP"
#define PASS_WIFI "XViewerAP"

#define ST_LED LEDB
#define LED_BLINK_TIME 500

#define K_SENS_NO "sensNo"
#define K_SENS_VECT "vect"
#define K_SENS_CODE "code"
#define K_SENS_NAME "name"
#define K_SENS_TYPE "type"
#define K_SENS_STATE "state"
#define CONFIG_SENS_JSON_SIZE (32 + (85 * SENSOR_MAX_NUMBER))

#define K_USER_NO "userNo"
#define K_USER_VECT "vect"
#define K_USER_USERNAME "username"
#define K_USER_PASS "pass"
#define K_USER_ADMIN "admin"
#define K_USER_AL "aLogin"
#define CONFIG_USER_JSON_SIZE (32 + (150 * USER_MAX_NUMBER))

#define K_ALARM_NO "alarmNo"
#define K_ALARM_VECT "vect"
#define K_ALARM_NAME "name"
#define K_ALARM_CODE "code"
#define K_ALARM_COUNTER "counter"
#define CONFIG_ALARM_JSON_SIZE (32 + (130 * ALARM_MAX_NUMBER))

#define K_PROD_NO "prodNo"
#define K_PROD_VECT "vect"
#define K_PROD_CODE "code"
#define K_PROD_NAME "name"
#define K_PROD_CSTR "cstr"
#define K_PROD_T_OK "t_ok"
#define K_PROD_T_NOK "t_nok"
#define K_PROD_P_OK "p_ok"
#define K_PROD_P_NOK "p_nok"
#define CONFIG_PROD_JSON_SIZE (32 + (250 * PROD_MAX_NUMBER))

#define PATH_PREFIX "/config"
#define PATH_SENS_CONFIG PATH_PREFIX "/sensConfig.txt"
#define PATH_USER_CONFIG PATH_PREFIX "/userConfig.txt"
#define PATH_ALARM_CONFIG PATH_PREFIX "/alarmConfig.txt"
#define PATH_PROD_CONFIG PATH_PREFIX "/prodConfig.txt"
#define PATH_PRINTER_CONFIG PATH_PREFIX "/printerConfig.txt"



#endif