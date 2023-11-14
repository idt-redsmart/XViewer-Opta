#ifndef XVIEWER_STRUCT_H
#define XVIEWER_STRUCT_H

#include "def.h"

#define SENSOR_MAX_NUMBER 64
#define SENSOR_NAME_SIZE 32
#define SENSOR_TYPE_SIZE 16

typedef struct
{
    uint8_t code;
    char name[SENSOR_NAME_SIZE];
    char type[SENSOR_TYPE_SIZE];
    bool state;
} sensorStruct_t;

#define USER_MAX_NUMBER 32
#define USER_USERNAME_SIZE 32
#define USER_PASS_SIZE 32

typedef struct
{
    char username[USER_USERNAME_SIZE];
    char pass[USER_PASS_SIZE];
    bool isAdmin;
    bool autologin;
} userStruct_t;

#define PROD_MAX_NUMBER 32
#define PROD_CODE_SIZE 32
#define PROD_NAME_SIZE 64
#define PROD_CSTR_SIZE 32

typedef struct
{
    uint32_t Ok = 0;
    uint32_t Nok = 0;
} counterPStruct_t;

typedef struct
{
    char code[PROD_CODE_SIZE];
    char name[PROD_NAME_SIZE];
    char customer[PROD_CSTR_SIZE];
    counterPStruct_t totalCounters, partialCounters;

} prodStruct_t;

#define ALARM_MAX_NUMBER 64
#define ALARM_CODE_SIZE 32
#define ALARM_NAME_SIZE 32

typedef struct
{
    char code[ALARM_CODE_SIZE];
    char name[ALARM_NAME_SIZE];
    uint16_t counter;
} alarmStruct_t;


typedef struct
{
    bool enable;
    bool printDate;
    bool printCstr;
    bool printUser;
    char customerStr[PROD_CSTR_SIZE];
    char userStr[USER_USERNAME_SIZE];
    char productCode[PROD_CODE_SIZE];
    char productName[PROD_NAME_SIZE];
} printerStruct_t;

enum processPhaseEnum
{
    PROCESS_IDLE = 1,
    PROCESS_ONGOING,
    PROCESS_OK,
    PROCESS_ERROR,    
};
#endif