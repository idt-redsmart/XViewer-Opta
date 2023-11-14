
#define G_SENSOR "getSensors"
#define G_SENSOR_STATE "getSensorsState"
#define G_USER "getInfoUser"
#define G_PRODUCT "getProducts"
#define G_PRINTER_DATA "getPrinterData"
#define G_PRODUCTION "getCurrentProduction"
#define G_ERROR "getErrors"
#define G_WORKING_USER "getWorkingUser"
#define G_TOTAL_COUNTER "getTotalCounters"
#define G_PARTIAL_COUNTER "getPartialCounters"
#define G_FLASH_PROGRAM_COUNT "getFlashProgramCounter"
#define G_PROCESS_STATUS "getProcessStatus"


#define P_ADD_USER "postAddUser"
#define P_UPDATE_USER "postUpdateUser"
#define P_LOGIN "postLogin"
#define P_SET_WORKING_USER "postSetWorkingUser"
#define P_UPDATE_SENSOR "postUpdateSensor"
#define P_ADD_PRODUCT "postAddProduct"
#define P_UPDATE_PRODUCT "postUpdateProduct"
#define P_SET_PRINTER "postSetPrinterData"
#define P_SET_PRODUCTION "postSetCurrentProduction"
#define P_ADD_ERROR "postAddError"
#define P_UPDATE_ERROR "postUpdateError"
#define P_UPDATE_RTC "postUpdateRtc"

enum response
{
    response_handleRoot = 1,
    response_getSensors,
    response_getSensorsState,
    response_getTotalCounters,
    response_getPartialCounters,
    response_getInfoUser,
    response_getProduct,
    response_getPrinterData,
    response_getCurrentProduction,
    response_getError,
    response_getWorkingUser,
    response_getFlashProgramCounter,
    response_getProcessStatus,

    response_postAddUser,
    response_postUpdateUser,
    response_postLogin,
    response_postSetWorkingUser,
    response_postUpdateSensor,
    response_postAddProduct,
    response_postUpdateProduct,
    response_postPrinterEnable,
    response_postSetPrinterData,
    response_postSetCurrentProduction,
    response_postAddError,
    response_postUpdateError,
    response_postUpdateRtc,
};