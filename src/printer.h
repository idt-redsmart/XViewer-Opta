#ifndef PRINTER_H
#define PRINTER_H

#include "def.h"
#include "XViewerStruct.h"

class Printer
{
public:
    printerStruct_t &getPrinter() { return _printer; }
    void clearStruct();
    void enable(bool enableDate = false, bool enableCustomer = false, bool enableUser = false);
    void set(bool enableDate, bool enableCustomer, bool enableUser);
    void disable();
    void setProdCode(char code[PROD_CODE_SIZE]) { memcpy(_printer.productCode, code, PROD_CODE_SIZE); }
    void setProdName(char name[PROD_NAME_SIZE]) { memcpy(_printer.productName, name, PROD_NAME_SIZE); }
    void setProdCstr(char cstr[PROD_CSTR_SIZE]) { memcpy(_printer.customerStr, cstr, PROD_CSTR_SIZE); }
    void setUserStr(char user[USER_USERNAME_SIZE]) { memcpy(_printer.customerStr, user, USER_USERNAME_SIZE); }

    void setDbg(bool dbgOn) { _dbgOn = dbgOn; }

private:
    printerStruct_t _printer;

    char _buffer[128];

    bool _dbgOn = false;

    void _dbg(String str = "");
    String _boolToString(bool data);
};

#endif