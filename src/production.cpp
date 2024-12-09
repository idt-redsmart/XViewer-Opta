#include "production.h"

bool Production::addProd(char code[PROD_CODE_SIZE], char name[PROD_NAME_SIZE], char customer[PROD_CSTR_SIZE])
{
    sprintf(_buffer, "_____\nADD PROD\nCode: %32s\nName: %32s\nCstr: %32s", code, name, customer);
    _dbg(_buffer, 0);

    // Scan prod vector and check if the code added is already in use.
    for (uint8_t index = 0; index < _prod.size(); index++)
    {
        if (strcmp(code, _prod.at(index).code) == 0)
        {
            sprintf(_buffer, "\nCode: %32s already in use.\nName: %32s\nCstr: %32s",
                    _prod.at(index).code, _prod.at(index).name, _prod.at(index).customer);
            _dbg(_buffer, 0);

            return false;
        }
    }

    // Check vector max size
    if (_prod.size() + 1 > PROD_MAX_NUMBER)
    {
        _dbg("Maximum number of production reached. Can't add");
        return false;
    }

    // Produciotn is not used - add
    prodStruct_t tmp;
    memcpy(tmp.code, code, PROD_CODE_SIZE);
    memcpy(tmp.name, name, PROD_NAME_SIZE);
    memcpy(tmp.customer, customer, PROD_CSTR_SIZE);

    _prod.push_back(tmp);

    sprintf(_buffer, "\nPROD ADDED\nCode: %32s\nName: %32s\nCstr: %32s\n_____", code, name, customer);
    _dbg(_buffer, 0);

    return true;
}

bool Production::removeProd(char code[PROD_CODE_SIZE])
{
    bool prodFound = false;
    int8_t indexProd = -1;
    std::vector<prodStruct_t> tmpVect(_prod);

    _dbg("Trying to remove prod code: " + String(code));

    for (uint8_t index = 0; index < tmpVect.size(); index++)
    {
        if (strcmp(tmpVect.at(index).code, code) == 0)
        {
            prodFound = true;
            indexProd = index;
            break;
        }
    }

    if (prodFound)
    {
        tmpVect.erase(tmpVect.begin() + indexProd);
    }
    else
    {
        _dbg("Prod code to remove not found. Looking for code: " + String(code));
        return false;
    }

    _prod.clear();

    for (uint8_t index = 0; index < tmpVect.size(); index++)
    {
        prodStruct_t tmp;

        memcpy(tmp.code, tmpVect.at(index).code, PROD_CODE_SIZE);
        memcpy(tmp.name, tmpVect.at(index).name, PROD_NAME_SIZE);
        memcpy(tmp.customer, tmpVect.at(index).customer, PROD_CSTR_SIZE);
        tmp.totalCounters.Ok = tmpVect.at(index).totalCounters.Ok;
        tmp.totalCounters.Nok = tmpVect.at(index).totalCounters.Nok;
        tmp.partialCounters.Ok = tmpVect.at(index).partialCounters.Ok;
        tmp.partialCounters.Nok = tmpVect.at(index).partialCounters.Nok;

        _prod.push_back(tmp);
    }
    return true;
}

bool Production::updateName(char code[PROD_CODE_SIZE], char newName[PROD_NAME_SIZE])
{
    // Scan prod vector and check if the code is in vector
    for (uint8_t index = 0; index < _prod.size(); index++)
    {
        if (strcmp(code, _prod.at(index).code) == 0)
        {
            sprintf(_buffer, "Code: %32s found. Updating name: %32s with new name: %32s",
                    _prod.at(index).code, _prod.at(index).name, newName);
            _dbg(_buffer);

            memcpy(_prod.at(index).name, newName, PROD_NAME_SIZE);

            return true;
        }
    }

    sprintf(_buffer, "Code: %32s not found.", code);
    _dbg(_buffer);
    return false;
}

bool Production::updateCustomer(char code[PROD_CODE_SIZE], char newCstr[PROD_CSTR_SIZE])
{
    // Scan prod vector and check if the code is in vector
    for (uint8_t index = 0; index < _prod.size(); index++)
    {
        if (strcmp(code, _prod.at(index).code) == 0)
        {
            sprintf(_buffer, "Code: %32s found. Updating cstr: %32s with new cstr: %32s",
                    _prod.at(index).code, _prod.at(index).customer, newCstr);
            _dbg(_buffer);

            memcpy(_prod.at(index).customer, newCstr, PROD_CSTR_SIZE);

            return true;
        }
    }

    sprintf(_buffer, "Code: %32s not found.", code);
    _dbg(_buffer);
    return false;
}

bool Production::updateCounter(const char* code, uint8_t counter)
{
    // Scan prod vector and check if the code is in vector
    for (uint8_t index = 0; index < _prod.size(); index++)
    {
        if (strcmp(code, _prod.at(index).code) == 0)
        {
            sprintf(_buffer, "Code: %32s found. Increasing counter: %6s",
                    _prod.at(index).code, _counterIndexToStr(counter).c_str());
            _dbg(_buffer);

            switch (counter)
            {
            case T_OK:
                _prod.at(index).totalCounters.Ok++;
                break;
            case T_NOK:
                _prod.at(index).totalCounters.Nok++;
                break;
            case P_OK:
                _prod.at(index).partialCounters.Ok++;
                break;
            case P_NOK:
                _prod.at(index).partialCounters.Nok++;
                break;

            default:
                _dbg("Counter not found. Available counters: T_OK, T_NOK, P_OK, P_NOK");
                break;
            }

            return true;
        }
    }

    sprintf(_buffer, "Code: %32s not found.", code);
    _dbg(_buffer);
    return false;
}

bool Production::updateCounter(const char* code, uint8_t counter, uint32_t newCounterVal)
{
    // Scan prod vector and check if the code is in vector
    for (uint8_t index = 0; index < _prod.size(); index++)
    {
        if (strcmp(code, _prod.at(index).code) == 0)
        {
            sprintf(_buffer, "Code: %32s found. Updating counter: %6s with new value: %8lu",
                    _prod.at(index).code, _counterIndexToStr(counter).c_str(), newCounterVal);
            _dbg(_buffer);

            switch (counter)
            {
            case T_OK:
                _prod.at(index).totalCounters.Ok = newCounterVal;
                break;
            case T_NOK:
                _prod.at(index).totalCounters.Nok = newCounterVal;
                break;
            case P_OK:
                _prod.at(index).partialCounters.Ok = newCounterVal;
                break;
            case P_NOK:
                _prod.at(index).partialCounters.Nok = newCounterVal;
                break;

            default:
                _dbg("Counter not found. Available counters: T_OK, T_NOK, P_OK, P_NOK");
                break;
            }

            return true;
        }
    }

    sprintf(_buffer, "Code: %32s not found.", code);
    _dbg(_buffer);
    return false;
}

prodStruct_t Production::getCurrentProd()
{
    prodStruct_t res;

    if (_indexCurrentProd < 0)
    {
        memcpy(res.code, PROD_STRUCT_CODE_NOT_FOUND, PROD_CODE_SIZE);
        memcpy(res.name, PROD_STRUCT_NAME_NOT_FOUND, PROD_NAME_SIZE);
        memcpy(res.customer, PROD_STRUCT_CSTR_NOT_FOUND, PROD_CSTR_SIZE);
        res.totalCounters.Ok = PROD_STRUCT_COUNTER_NOT_FOUND;
        res.totalCounters.Nok = PROD_STRUCT_COUNTER_NOT_FOUND;
        res.partialCounters.Ok = PROD_STRUCT_COUNTER_NOT_FOUND;
        res.partialCounters.Nok = PROD_STRUCT_COUNTER_NOT_FOUND;
    }
    else
    {
        memcpy(res.code, _prod.at(_indexCurrentProd).code, PROD_CODE_SIZE);
        memcpy(res.name, _prod.at(_indexCurrentProd).name, PROD_NAME_SIZE);
        memcpy(res.customer, _prod.at(_indexCurrentProd).customer, PROD_CSTR_SIZE);
        res.totalCounters.Ok = _prod.at(_indexCurrentProd).totalCounters.Ok;
        res.totalCounters.Nok = _prod.at(_indexCurrentProd).totalCounters.Nok;
        res.partialCounters.Ok = _prod.at(_indexCurrentProd).partialCounters.Ok;
        res.partialCounters.Nok = _prod.at(_indexCurrentProd).partialCounters.Nok;
    }

    // _dbg("Current production: " + String(res.code));

    return res;
}

bool Production::setCurrentProd(char code[PROD_CODE_SIZE])
{
    bool res = false;

    for (uint8_t index = 0; index < _prod.size(); index++)
    {
        if (strcmp(code, _prod[index].code) == 0)
        {
            if (index == _indexCurrentProd)
                return true;

            _indexCurrentProd = index;
            _dbg("Current prod set: " + String(_prod[_indexCurrentProd].code));
            res = true;
            break;
        }
    }
    return res;
}

void Production::print()
{
    _dbg("\n_____\nPRINTING PRODUCTION", 0);
    if (_indexCurrentProd > 0)
        _dbg("Current prod code: " + String(_prod.at(_indexCurrentProd).code), 0);

    for (uint8_t index = 0; index < _prod.size(); index++)
    {
        sprintf(_buffer, "Code: %32s\nName: %32s\nCstr: %32s\nTOk: %8lu | TNok : %8lu | POk: %8lu | PNok: %8lu\n_____",
                _prod.at(index).code, _prod.at(index).name, _prod.at(index).customer,
                _prod.at(index).totalCounters.Ok, _prod.at(index).totalCounters.Nok, _prod.at(index).partialCounters.Ok, _prod.at(index).partialCounters.Nok);
        _dbg(_buffer, 0);
    }
}

String Production::_counterIndexToStr(uint8_t indexCounter)
{
    switch (indexCounter)
    {
    case T_OK:
        return "T_OK";
        break;
    case T_NOK:
        return "T_NOK";
        break;
    case P_OK:
        return "P_OK";
        break;
    case P_NOK:
        return "P_NOK";
        break;

    default:
        return "Counter not found";
        break;
    }
}

DynamicJsonDocument Production::vectToJson()
{
    DynamicJsonDocument json(CONFIG_PROD_JSON_SIZE);
    String str;

    json[K_PROD_NO] = _prod.size();
    JsonArray userArr = json.createNestedArray(K_PROD_VECT);

    for (uint8_t index = 0; index < _prod.size(); index++)
    {
        JsonObject userObj = userArr.createNestedObject();

        userObj[K_PROD_CODE] = _prod.at(index).code;
        userObj[K_PROD_NAME] = _prod.at(index).name;
        userObj[K_PROD_CSTR] = _prod.at(index).customer;
        userObj[K_PROD_T_OK] = _prod.at(index).totalCounters.Ok;
        userObj[K_PROD_T_NOK] = _prod.at(index).totalCounters.Nok;
        userObj[K_PROD_P_OK] = _prod.at(index).partialCounters.Ok;
        userObj[K_PROD_P_NOK] = _prod.at(index).partialCounters.Nok;
    }

    serializeJsonPretty(json, str);
    return json;
}

void Production::_dbg(String str, bool header)
{
    if (_dbgOn)
    {
        if(header)
        Serial.println("[PRODUCTION]\t" + str);
        else
        Serial.println(str);
    }
}
