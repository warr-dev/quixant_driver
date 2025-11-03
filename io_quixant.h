#ifndef IO_QUIXANT_H
#define IO_QUIXANT_H

#include "libDrivers.h"
#include <bitset>
#include "io_interface.h"
#include "led_strips/ledstrip_driver_gamesman.h"
#include "led_strips/ledstrip_driver_dingo.h"


#define MAX_MATHOFFSET 1000000
#define QX_INPUT_DOOR_START  18
#define QX_INPUT_DOOR_END  21

void IOQuixantBatteryStatusCallback(struct intHandler *intHand);

void IOQuixantCPUDoorOpenCallback(struct intHandler *intHand);

void IOQuixantCPUDoorClosedCallback(struct intHandler *intHand);

class IOQuixant : public IInputDriver, public IOutputDriver, public IWatchdog, public ISPIDriver {
private:
    friend class LedStripDriverGAMESMAN;

    friend class LedStripDriverDINGO;

    friend void IOQuixantCPUDoorOpenCallback(struct intHandler *);

    friend void IOQuixantCPUDoorClosedCallback(struct intHandler *);

    IOQuixant();

public:

    ~IOQuixant();

    static IOQuixant *GetInstance();

    int InitInputDriver(std::string const &path = "", std::string const &workingDir = "") override;

    int InitOutputDriver(std::string const &path) override;

    void SetCallBack(void *callbackFunction) override;

    void SetOutputCallback(void *callbackFunction) override;

    int SetOutputs(uint32_t outputBitMask) override;

    int SetStateForASpecificOutput(int output) override;

    int ClearStateForASpecificOutput(int output) override;

    uint32_t GetInputMask () override;

    uint32_t GetOutputMask () override;

    int PrintQuixantHardwareInformation();

    IO_PLATFORM_TYPE GetQuixantType();

    std::string driverInfo;

    void Process();

    bool quitThread;
    int usleeptime;
    int batteryStatus;

    void DEBUGGetBatteriesVoltageLevels();

    uint32_t lastInputs;
    uint32_t lastOutputs;
    unsigned long lastdiff;


    void ReportAllBatteryStatus(uint32_t bitMask);

    void ReportCpuDoorStatus(bool isOpen);

private:
    pthread_t m_thread;
    pthread_mutex_t changeOutputMutex;

    double (*CallBack)(IO_DRIVER_CALLBACK *apiCall);

    int InitSPI() override;

    int SendDataToSPIBus(unsigned char *data, int size) override;

    virtual void SendCallBack(IO_DRIVER_CALLBACK *apiCall);

    void SetBatteryCheckFrequency(unsigned char frequency);

    void SetIntrusionCheck(uint16_t bitMask);

    unsigned int ReadQuixantIntrusions();

    bool CheckIntrusion(uint16_t bitMask);

    IO_BATTERY_STATUS GetIOBAtteryStatusFromDriverData(uint32_t driverData);

    void ReportNewInputMask();

    unsigned char SPIpauseMS;
    unsigned char readOneByte;
    unsigned char startByte;
    unsigned char stopByte;

    unsigned int FPGAVersion;

public:
    char SetWatchdog(unsigned char timeInSeconds) override;

    char RestartWatchdog() override;

    void triggerBatteryLevels();

    int checkBatteryLevel(int batn, unsigned int bat0, unsigned int bat1, unsigned int bat2);

    int batLowLevel;
    int batCriticalLevel;
};

#endif // IO_QUIXANT6000_H

