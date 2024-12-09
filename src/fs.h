#ifndef FS_H
#define FS_H

#include "def.h"
#include "XViewerStruct.h"
#include "BlockDevice.h"
#include "QSPIFBlockDevice.h"
#include "MBRBlockDevice.h"
#include "FATFileSystem.h"

class FsClass
{
public:
    FsClass() : _blockDevice(QSPI_FLASH1_IO0, QSPI_FLASH1_IO1, QSPI_FLASH1_IO2, QSPI_FLASH1_IO3,
                             QSPI_FLASH1_SCK, QSPI_FLASH1_CSN, QSPIF_POLARITY_MODE_0, MBED_CONF_QSPIF_QSPI_FREQ) {}

    bool init();

    bool storeConfig(const char *path, DynamicJsonDocument json);

    bool deleteConfig(const char *path);

    bool readSensConfig(std::vector<sensorStruct_t> &sensVect);
    bool readUserConfig(std::vector<userStruct_t> &userVect);
    bool readAlarmConfig(std::vector<alarmStruct_t> &alarmVect);
    bool readProdConfig(std::vector<prodStruct_t> &prodVect);
    bool readPrinterConfig(printerStruct_t &printerConfig);

    void setDbg(bool dbgOn) { _dbgOn = dbgOn; }

private:
    QSPIFBlockDevice _blockDevice;
    mbed::bd_size_t _erasableSize;

    bool _dbgOn = false;

    void _dbg(String str = "");
};

#endif