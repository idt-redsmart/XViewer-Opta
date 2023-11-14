
#include <XViewer.h>

bool XViewer::setProcessStatus(processPhaseEnum status, String alarmCode)
{

    int errorCodeInt = -1;

    std::vector<alarmStruct_t> tmp(_alarms.getAlarmsVect());

    for (uint8_t index = 0; index < tmp.size(); index++)
    {
        if (alarmCode.compareTo(tmp[index].code) == 0)
            errorCodeInt = index;
    }

    switch (status)
    {
    case PROCESS_IDLE:
        _currentProcessStauts = PROCESS_IDLE;
        _currentProcessError = -1;
        return true;

    case PROCESS_ONGOING:
        _currentProcessStauts = PROCESS_ONGOING;
        _currentProcessError = -1;
        return true;

    case PROCESS_OK:
        _currentProcessStauts = PROCESS_OK;
        _currentProcessError = -1;
        return true;

    case PROCESS_ERROR:
        _currentProcessStauts = PROCESS_ERROR;
        _currentProcessError = errorCodeInt;
        return true;

    default:
        Serial.println("Process status not found. Can't set this status");
        return false;
    }
}

StaticJsonDocument<200> XViewer::getProcessStatus()
{
    StaticJsonDocument<200> jsonOut;
    // JsonObject objOut = jsonOut.to<JsonObject>();
    std::vector<alarmStruct_t> tmp(_alarms.getAlarmsVect());

    switch (_currentProcessStauts)
    {
    case PROCESS_IDLE:
        jsonOut["Res"] = PROCESS_IDLE;
        break;

    case PROCESS_ONGOING:
        jsonOut["Res"] = PROCESS_ONGOING;
        break;

    case PROCESS_OK:
        jsonOut["Res"] = PROCESS_OK;
        break;

    case PROCESS_ERROR:
        jsonOut["Res"] = PROCESS_ERROR;
        jsonOut["Error code"] = tmp[_currentProcessError].code;
        break;

    default:
        break;
    }

    return jsonOut;
}
