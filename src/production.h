
#ifndef PRODUCTION_H
#define PRODUCTION_H

#include "def.h"
#include "XViewerStruct.h"

#define PROD_STRUCT_CODE_NOT_FOUND "CODE NOT FOUND"
#define PROD_STRUCT_NAME_NOT_FOUND "NAME NOT FOUND"
#define PROD_STRUCT_CSTR_NOT_FOUND "CUSTOMER NOT FOUND"
#define PROD_STRUCT_COUNTER_NOT_FOUND 0

enum countersEnum
{
    T_OK = 1,
    T_NOK,
    P_OK,
    P_NOK,
};

class Production
{
public:
    void init();

    std::vector<prodStruct_t> &getProdVect() { return _prod; };

    bool addProd(char code[PROD_CODE_SIZE], char name[PROD_NAME_SIZE], char customer[PROD_CSTR_SIZE]);
    bool removeProd(char code[PROD_CODE_SIZE]);
    bool updateName(char code[PROD_CODE_SIZE], char newName[PROD_NAME_SIZE]);
    bool updateCustomer(char code[PROD_CODE_SIZE], char newCstr[PROD_CSTR_SIZE]);
    bool updateCounter(char code[PROD_CODE_SIZE], uint8_t counter);
    bool updateCounter(char code[PROD_CODE_SIZE], uint8_t counter, uint32_t newCounterVal);

    void print();

    prodStruct_t getCurrentProd();
    bool setCurrentProd(char code[PROD_CODE_SIZE]);

    DynamicJsonDocument vectToJson();

    void setDbg(bool dbgOn) { _dbgOn = dbgOn; }

private:
    std::vector<prodStruct_t> _prod;
    int8_t _indexCurrentProd = -1;

    char _buffer[256];

    bool _dbgOn = false;

    void _dbg(String str = "", bool header = true);

    String _counterIndexToStr(uint8_t indexCounter);
};

#endif