#include "XViewer.h"

printerStruct_t XViewer::getCurrentPrinter(bool printDbg)
{
    char _buffer[128];
    int8_t indexWorkingUser = _users.getIndexWorkingUser();

    _setPrinterProdData();

    printerStruct_t tmp(_printer.getPrinter());

    if ((strcmp(tmp.userStr, "") == 0 || strcmp(tmp.userStr, "null") == 0) && indexWorkingUser >= 0)
    {
        memcpy(tmp.userStr, _users.getUserVect().at(indexWorkingUser).username, USER_USERNAME_SIZE);
    }

    if (!printDbg)
        return tmp;

    _dbgP();
    sprintf(_buffer, "%-20s  %-5s", "Printer enabled?", _boolToString(tmp.enable).c_str());
    _dbgP(_buffer);

    sprintf(_buffer, "%-20s  %-5s", "Date enabled?", _boolToString(tmp.printDate).c_str());
    _dbgP(_buffer);

    sprintf(_buffer, "%-20s  %-5s", "Customer enabled?", _boolToString(tmp.printCstr).c_str());
    _dbgP(_buffer);

    sprintf(_buffer, "%-20s  %-5s", "User enabled?", _boolToString(tmp.printUser).c_str());
    _dbgP(_buffer);

    sprintf(_buffer, "%-20s  %-32s", "Customer str:", tmp.customerStr);
    _dbgP(_buffer);

    sprintf(_buffer, "%-20s  %-32s", "User str:", tmp.userStr);
    _dbgP(_buffer);

    sprintf(_buffer, "%-20s  %-32s", "Product code:", tmp.productCode);
    _dbgP(_buffer);

    sprintf(_buffer, "%-20s  %-64s", "Product name:", tmp.productName);
    _dbgP(_buffer);

    return tmp;
}

void Printer::enable(bool enableDate, bool enableCustomer, bool enableUser)
{
    _printer.enable = true;
    (enableDate) ? _printer.printDate = true : true;
    (enableCustomer) ? _printer.printCstr = true : true;
    (enableUser) ? _printer.printUser = true : true;

    sprintf(_buffer, "Printer enabled\t\tDate: %6s | Cstr: %6s | User: %6s",
            _boolToString(_printer.printDate).c_str(), _boolToString(_printer.printCstr).c_str(),
            _boolToString(_printer.printUser).c_str());
    _dbg(_buffer);
}

void Printer::set(bool enableDate, bool enableCustomer, bool enableUser)
{
    _dbg("Setting printer");

    // setto e salvo dati stampante
    _printer.printDate = enableDate;
    _printer.printUser = enableUser;
    _printer.printCstr = enableCustomer;
}

void Printer::clearStruct()
{
    _printer.enable = false;
    _printer.printDate = false;
    _printer.printCstr = false;
    _printer.printUser = false;
    memcpy(_printer.customerStr, "", 1);
    memcpy(_printer.userStr, "", 1);
    memcpy(_printer.productCode, "", 1);
    memcpy(_printer.productName, "", 1);
}

void Printer::disable()
{
    _printer.enable = false;
}

void XViewer::_setPrinterProdData()
{
    prodStruct_t currentProd = _prods.getCurrentProd();
    _printer.setProdCode(currentProd.code);
    _printer.setProdName(currentProd.name);
    _printer.setProdCstr(currentProd.customer);
}

String XViewer::_boolToString(bool data)
{
    if (data)
        return "TRUE";
    else
        return "FALSE";
}

String Printer::_boolToString(bool data)
{
    if (data)
        return "TRUE";
    else
        return "FALSE";
}

void Printer::_dbg(String str)
{
    if (_dbgOn)
        Serial.println("[PRINTER]\t" + str);
}

void XViewer::_dbgP(String str)
{
    if (_dbgPOn)
        Serial.println("[PRINTER]\t" + str);
}
