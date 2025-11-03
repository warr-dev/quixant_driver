#include "io_quixant.h"
#include "memory/memory_manager.h"
#include "aux/logger_proxy.h"
#include "aux/utils.h"

#include <iostream>
#include <cstring>

#include <unistd.h>

extern "C" {
	#include <libqxt.h>
	#include <libsram.h>
	#include <libsecmeter.h>
}

void *IOQuixantThread(void *c) {
    usleep(100000);

    IOQuixant *ioqxt = static_cast <IOQuixant *> (c);

    usleep(ioqxt->usleeptime);

    while (!ioqxt->quitThread) {
        ioqxt->Process();
        usleep(ioqxt->usleeptime);
    }
    return 0;
}

IOQuixant *IOQuixant::GetInstance() {
    static IOQuixant instance{};
    return &instance;
}

IOQuixant::IOQuixant() {
    batteryStatus = 0;
    quitThread = false;
    usleeptime = 50000;    //poll every 50 ms BUG 5628
    lastOutputs = 0;

    CallBack = nullptr;

    pthread_mutex_init(&changeOutputMutex, NULL);

    startByte = 0x55;
    stopByte = 0xAA;
    SPIpauseMS = 5;
    readOneByte = 0;

    FPGAVersion = 0;

    batLowLevel = 2700;
    batCriticalLevel = 2400;
}

int IOQuixant::InitOutputDriver(std::string const &path) {
    //Init is done on InitInputDriver
    return LIB_DRIVERS_OPERATION_SUCCESS;
}

int IOQuixant::InitInputDriver(std::string const &path, std::string const &workingDir) {
    int result = LIB_DRIVERS_OPERATION_SUCCESS;

    qxt_device_init();

    IO_PLATFORM_TYPE io_temp = GetQuixantType();

    lastInputs = 0xFFFFFFFF;

    struct sBatSetup batLimits;
    batLimits.bat0_lvlw = batLowLevel;
    batLimits.bat1_lvlw = batLowLevel;
    batLimits.bat2_lvlw = batLowLevel;
    batLimits.bat0_lvla = batCriticalLevel;
    batLimits.bat1_lvla = batCriticalLevel;
    batLimits.bat2_lvla = batCriticalLevel;

    qxt_std_setbatlimits(&batLimits);
    SetBatteryCheckFrequency(4);

    constexpr uint16_t intrusionBitMask = 0x80; // Just CPU door selected
    SetIntrusionCheck(intrusionBitMask);

    if (!CheckIntrusion(intrusionBitMask)) {
        ReportCpuDoorStatus(true);
    }

    lastInputs = GetInputMask ();

    pthread_create(&m_thread, NULL, IOQuixantThread, this);

    switch (io_temp) {
        case IO_NONE:
            result = LIB_DRIVERS_ERROR_CONF_UNABLE_TO_ACCESS_DEVICE;
            break;

        case IO_QUIXANT_QX7000:
            triggerBatteryLevels();
            break;

        case IO_QUIXANT_QX200:
            break;

        default:
            result = LIB_DRIVERS_ERROR_UNKNOWN;
            break;
    }

    //result = PrintQuixantHardwareInformation();
    return result;
}

void IOQuixant::SetOutputCallback(void *callbackFunction) {}

void IOQuixant::SetCallBack(void *callbackFunction) {
    *(void **) (&CallBack) = callbackFunction;
}

void IOQuixant::SendCallBack(IO_DRIVER_CALLBACK *apiCall) {
    if (CallBack)
        (*CallBack)(apiCall);
    else
        LOG_DEBUG_DRIVERS << "IOQUIXANT NO CALLBACK";
}

IOQuixant::~IOQuixant() {
    quitThread = true;
}

uint32_t IOQuixant::GetInputMask () {
    return ~qxt_dio_readdword(0);
}

void IOQuixant::Process() {
    uint32_t newInputs = GetInputMask ();
    if (lastInputs != newInputs) {
        lastInputs = newInputs;
        ReportNewInputMask();
    }

}

IO_PLATFORM_TYPE IOQuixant::GetQuixantType() {
    unsigned int result;
    struct hw_inventory inventory_old;

    result = qxt_hw_inventory(&inventory_old);
    if (result) {
        LOG_ERROR_DRIVERS << "IOQuixant: Ioctl failed. Error reported is " << result;;
        return IO_NONE;
    }

    std::string target_id_str = std::string(inventory_old.target_id);
    size_t found = target_id_str.find("06.00");

    if (found == std::string::npos && (inventory_old.log_fw_version[0] - '0') >= 4) {
        return IO_QUIXANT_QX7000;
    } else {
        return IO_QUIXANT_QX200;
    }

}

int IOQuixant::PrintQuixantHardwareInformation() {
    using namespace std;
    //u32 aux_counter;
    unsigned int aux_counter;
    unsigned char serial_number[6];
    struct hw_inventory inventory;
    std::stringstream ss;
    std::stringstream header;

    int result = LIB_DRIVERS_OPERATION_SUCCESS;

    if (qxt_readSN(serial_number) == 0)
        ss << (unsigned int) serial_number[0] << (unsigned int) serial_number[1] << (unsigned int) serial_number[2]
           << (unsigned int) serial_number[3] << (unsigned int) serial_number[4] << (unsigned int) serial_number[5];
    else
        return -1;

    header << "----------------------------------------------------------------" << endl;
    header << "------------- QUIXANT WHICH?? PLATFORM HW REPORT -------------------" << endl;
    header << "----------------------------------------------------------------" << endl;
    header << endl;

    aux_counter = qxt_hw_inventory(&inventory);
    if (aux_counter) {
        cout << " Ioctl failed. Error reported is " << aux_counter << endl;
        return -1;
    }

    auto * nvDriver = MemoryManager::GetInstance().GetDriver();
    unsigned int NVRAMsize = 0;

    if (nvDriver) {
        NVRAMsize = nvDriver->GetSize();
    }

    FPGAVersion = (unsigned int) qxt_get_fpga_version();

    header << "Serial Number 	= " << ss.str() << endl;
    header << "Log FW Version	= " << inventory.log_fw_version << endl;
    header << "Driver Version	= " << inventory.driver_version << endl;
    header << "Driver Product	= " << inventory.driver_product << endl;
    header << "Library Version	= " << inventory.library_version << endl;
    header << "Library Product	= " << inventory.library_product << endl;
    header << "FPGA Version	= " << FPGAVersion << endl;
    header << "NVRAM size = " << hex << NVRAMsize << endl;
    header << "----------------------------------------------------------------" << endl;
    header << endl;

    cout << header.str() << endl;

    return result;

}

void IOQuixant::DEBUGGetBatteriesVoltageLevels() {
    /*unsigned short batStatus[1];

    if(0 != qxt_std_readbat(QXT_BATTERY0, batStatus))
        cout << "Error - Unable to get battery 1 voltage" <<  endl;
    else
        cout << "Battery 1 voltage is " << batStatus[0] << "mV" << endl;

    if(0 != qxt_std_readbat(QXT_BATTERY1, batStatus))
        cout << "Error - Unable to get battery 2 voltage" <<  endl;
    else
        cout << "Battery 2 voltage is " << batStatus[0] << "mV" << endl;

    if(0 != qxt_std_readbat(QXT_BATTERY2, batStatus))
        cout << "Error - Unable to value battery 3 voltage\n" <<  endl;
    else
        cout << "Battery 3 voltage is " << batStatus[0] << "mV\n" << endl;	*/

    unsigned int bat0, bat1, bat2;

    bat0 = 0;
    bat1 = 0;
    bat2 = 0;

    qxt_std_readbatteries(&bat0, &bat1, &bat2, QXT_FORCE_BATTERY_READING);
    LOG_DEBUG_DRIVERS << "Battery0: " << bat0 << "\n" << "Battery1: " << bat1 << "\n" << "Battery2: " << bat2 << "\n";
}

void IOQuixant::SetBatteryCheckFrequency(unsigned char frequency) {
    //qxt_battery_check(frequency, IOQuixantBatteryStatusCallback);
    char teste_baterias = qxt_battery_check(frequency, IOQuixantBatteryStatusCallback);
    std::cout << "Battery freq: " << (int) teste_baterias << std::endl;
}

void IOQuixant::SetIntrusionCheck(uint16_t bitMask) {

    INTRUSION_DEFINITIONS definitions{};

    // set intrusion definitions
    definitions.Intrusion7 = PowerOnNormallyClosed;
    if (!QXT_SUCCESS(qxtLpDefineIntrusions(definitions))) {
        std::cerr << "Failed setting cpu intrusion definitions" << std::endl;
        return;
    }

    auto interrupt_result = qxt_std_interrupts(
            QXT_INTRUSION_CLOSED,
            TRUE,
            bitMask,
            0,
            &IOQuixantCPUDoorOpenCallback);

    if (interrupt_result) {
        std::cerr << "Failed setting cpu door closed callback" << std::endl;
        return;
    }

    interrupt_result = qxt_std_interrupts(
            QXT_INTRUSION_OPEN,
            TRUE,
            bitMask,
            0,
            &IOQuixantCPUDoorClosedCallback);

    if (interrupt_result) {
        std::cerr << "Failed setting cpu door open callback" << std::endl;
        return;
    }
}

void IOQuixant::ReportCpuDoorStatus(bool isOpen) {
    IO_DRIVER_CALLBACK update{};

    update.header.type = isOpen ? IO_API_INPUT_UP : IO_API_INPUT_DOWN;
    update.singleInputUpdate.name = CPU_DOOR;

    SendCallBack(&update);
}

void IOQuixantCPUDoorOpenCallback(intHandler *) {
    IOQuixant::GetInstance()->ReportCpuDoorStatus(true);
}

void IOQuixantCPUDoorClosedCallback(intHandler *) {
    IOQuixant::GetInstance()->ReportCpuDoorStatus(false);
}

int IOQuixant::ClearStateForASpecificOutput(int output) {
    uint32_t bitmask = 0;
    uint32_t port = 0;

    bitmask = (1U << (output % 8U));
    port = (output / 8U);

	BitClear <uint32_t> (lastOutputs, output);

    pthread_mutex_lock(&changeOutputMutex);
    int result = qxt_dio_clearbit(port, bitmask);
    pthread_mutex_unlock(&changeOutputMutex);
    return result;
}

int IOQuixant::SetOutputs(uint32_t outputBitMask) {
    if (outputBitMask == lastOutputs)
        return LIB_DRIVERS_OPERATION_SUCCESS;

    lastOutputs = outputBitMask;

    return qxt_dio_writedword(0, outputBitMask);
}

int IOQuixant::SetStateForASpecificOutput(int output) {
    uint32_t bitmask = 0;
    uint32_t port = 0;
    //cout << "Setting State for " << output << endl;
    bitmask = (1U << (output % 8U));
    port = (output / 8U);

	BitSet <uint32_t> (lastOutputs, output);

    pthread_mutex_lock(&changeOutputMutex);
    int result = qxt_dio_setbit(port, bitmask);
    pthread_mutex_unlock(&changeOutputMutex);

    return result;
}

void IOQuixant::ReportAllBatteryStatus(uint32_t bitMask) {
    IO_DRIVER_CALLBACK update;
    memset((void *) &update, 0, sizeof(update));
    update.header.type = IO_API_BATTERY_STATUS_CHANGE;
    update.batteryStatusUpdate.bat0 = GetIOBAtteryStatusFromDriverData(bitMask & 0x03);
    update.batteryStatusUpdate.bat1 = GetIOBAtteryStatusFromDriverData((bitMask & 0x0C) >> 2);
    update.batteryStatusUpdate.bat2 = GetIOBAtteryStatusFromDriverData((bitMask & 0x30) >> 4);
    SendCallBack(&update);
}

void IOQuixantBatteryStatusCallback(struct intHandler *intHand) {
    IOQuixant::GetInstance()->ReportAllBatteryStatus(intHand->value2);
}

IO_BATTERY_STATUS IOQuixant::GetIOBAtteryStatusFromDriverData(uint32_t driverData) {
    IO_BATTERY_STATUS result = BATTERY_STATUS_UNKNOWN;

    switch (driverData) {
        case BATTERY_CHECK_GOOD:
            result = BATTERY_STATUS_NORMAL;
            break;

        case BATTERY_CHECK_WARNING:
            result = BATTERY_STATUS_LOW;
            break;

        case BATTERY_CHECK_ALARM:
            result = BATTERY_STATUS_CRITICAL;
            break;
        default:
            result = BATTERY_STATUS_UNKNOWN;
            break;
    }
    return result;
}

void IOQuixant::ReportNewInputMask() {
    IO_DRIVER_CALLBACK update;
    memset(&update, 0, sizeof(update));
    update.header.type = IO_API_INPUTS_STATUS_CHANGE;
    update.inputsUpdate.inputBitMask = lastInputs;
    SendCallBack(&update);
}

int IOQuixant::InitSPI() {
    // Mode 2 = 0x48;  // CPOL=1, CPHA=0;
    char input_mode = 2;

    // SPI clock frequency 4KHz
    unsigned char SPI_FREQ = 0x64;

    // Init SPI quixant

    qxt_spi_init(SPI_FREQ,
                 input_mode); // funcion DOES NOT behave as described in documentation. should return 0xFF if unable to access SPI. it does not.

    unsigned char test[9] = {startByte, 0, 0, 0, 0, 0, 0, 0, stopByte};
    /*test[0] = startByte;
    test[1] = 0;
    test[2] = 0;
    test[3] = 0;
    test[4] = 0;
    test[5] = 0;
    test[6] = 0;
    test[7] = 0;
    test[8] = stopByte;*/

    if (SendDataToSPIBus(test, 9) == 0xFF) {
        LOG_ERROR_DRIVERS << "IOQuixant: FAILURE TO OPEN SPI";
        return 1;
    } else {
        return 0;
    }

}

int IOQuixant::SendDataToSPIBus(unsigned char *data, int dataSize) {
    int result = 1058; //var to check SPI response. 0xFF => Command Sent; 0xFF => Unable to access SPI. Check Permissions and FPGA Firmware

    for (int i = 0; i < dataSize; i++) {
        result = qxt_spi_writeread(data[i], &readOneByte, SPIpauseMS);
        //cout << hex << (unsigned int) data[i] << " | " ;

        if (result == 255) {
            LOG_ERROR_DRIVERS << "IOQuixant: FAILURE TO OPEN SPI";
        }
    }

    return result;
}

uint32_t IOQuixant::GetOutputMask () {
	 return lastOutputs;
}

char IOQuixant::SetWatchdog(unsigned char timeInSeconds) {
    unsigned char units = QXT_WATCHDOG_SECONDS;
    unsigned char watchdog = QXT_RUNTIME_WATCHDOG;
    unsigned char action = 0;

    char result = qxt_wd_enable(timeInSeconds, units, watchdog, action);

    if (result == 0x00)
        LOG_INFO_DRIVERS << "[IOQuixant::SetWatchdog] Watchdog set to " << std::to_string(timeInSeconds) << " seconds.";
    else
        LOG_WARNING_DRIVERS << "[IOQuixant::SetWatchdog] Watchdog enable failed!";

    return result;
}

char IOQuixant::RestartWatchdog() {
    return LIB_DRIVERS_ERROR_NOT_AVAILABLE;
}

void IOQuixant::triggerBatteryLevels() {
    uint32_t result = 0x00;

    uint32_t bat0 = 0;
    uint32_t bat1 = 0;
    uint32_t bat2 = 0;

    qxt_std_readbatteries(&bat0, &bat1, &bat2, true);

    int i = 0;
    for (i = 2; i >= 0; i--) {
        uint32_t batnValue = checkBatteryLevel(i, bat0, bat1, bat2);
        result = result << 2U;
        result = (result & ~0x03) | (batnValue & 0x03);
    }

    ReportAllBatteryStatus(result);
}

int IOQuixant::checkBatteryLevel(int batn, uint32_t bat0, uint32_t bat1, uint32_t bat2) {
    int result = 0;
    int batnValue = 0;

    switch (batn) {
        case 0: {
            batnValue = bat0;
            break;
        }
        case 1: {
            batnValue = bat1;
            break;
        }
        case 2: {
            batnValue = bat2;
            break;
        }
    }

    if (batnValue > batLowLevel) {
        //Nothing to do every thing is ok
        result = BATTERY_CHECK_GOOD;
    } else if (batnValue >= batCriticalLevel) {
        result = BATTERY_CHECK_WARNING;
    } else {
        result = BATTERY_CHECK_ALARM;
    }

    return result;
}

unsigned int IOQuixant::ReadQuixantIntrusions() {
    unsigned int intrusionBitmask = 0;

    // pressed = 0, released = 1
    auto result = qxtLpReadIntrusionStatus(&intrusionBitmask);

    if (result != Q_SUCCESS) {
        LOG_WARNING_DRIVERS << "Failed reading intrustion status" ;
    }

    return intrusionBitmask;
}

bool IOQuixant::CheckIntrusion(uint16_t bitMask) {
    auto intrusionStatus = ReadQuixantIntrusions();
    return (intrusionStatus & bitMask);
}


