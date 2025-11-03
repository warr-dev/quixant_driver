#########################################################################################################
# In order to compile the driver for your Kernel use the following command:                             #
#                                                                                                       #
# ./driverscomp                                                                                         #
#                                                                                                       #
# if everything is fine you should get the ko file (Kernel Module)                                      #
#                                                                                                       #
# To build the ko file for a specific kernel version, please use                                        #
# the -s switch on the command line, as in the following example:                                       #
#                                                                                                       #
# ./driverscomp -s /lib/modules/3.5.0-22-generic/build                                                  #
#                                                                                                       #
# in order to install the kernel modules run as root the following command:                             #
# ./qxt                                                                                                 #
#########################################################################################################

//
// Quixant Core Module Device Drivers for Linux OS
//
//
// Release Notes
// 
-----------------------------------------------------
Version: 0.7.0.1
Date: 2024/05/31
Description:
* Added support for Linux kernel 6.8

Version: 0.7.0.0
Date: 2024/02/13
Description:
* Added support for Linux kernel 6.6

Version: 0.6.1.1
Date: 2023/01/11
Description:
* Added support for main board ID in legacy platforms

Version: 0.6.1.0
Date: 2022/12/05
Description:
* Added support for main board ID

Version: 0.6.0.2
Date: 2022/01/28
Description:
* Update for Linux Kernel >= 5.12

Version: 0.6.0.1
Date: 2021/04/08
Description:
* Added QXT_I2C_SET_BUS selecting the second bus i2c

Version: 0.5.1.0
Date: 2020/12/10
Description:
* Added QXT_CFG_REG_SET and QXT_CFG_REG_GET

Version: 0.5.0.0
Date: 2020/11/20
Description:
* Added QXT_I2C_LOCK and QXT_I2C_UNLOCK

Version: 0.4.0.6
Date: 2020/08/12
Description:
* The relase 0.4.0.6 provides new information about LP, Intrusions, DIN and DOUT in QSys

Version: 0.4.0.5
Date: 2020/07/17
Description: 
* The release 0.4.0.5 fixes some kernel log messages in i2c

Version: 0.4.0.4
Date: 2020/06/30
Description: 
* The release 0.4.0.4 omits to log some i2c errors

Version: 0.4.0.3
Date: 2020/05/19
Description: 
* The release 0.4.0.3 fixes an issue of AMD linux kernel v.5.4.2-1138-amd+

Version: 0.4.0.2
Date: 2020/05/14
Description: 
* The release 0.4.0.2 fixes an issue of linux kernel v.5.x

Version: 0.4.0.1
Date: 2020/03/06
Description: 
* The release 0.4.0.1 fixes an issue regarding multithread access

Version: 0.4.0.0
Date: 2020/03/01
Description: 
* The release 0.4.0.0 implements the new ioctl QXT_I2C_RW

Version: 0.3.1.5
Date: 2019/11/27
Description: 
* The release 0.3.1.5 generates the QSys information

Version: 0.3.1.4
Date: 2019/09/20
Description: 
* The release 0.3.1.4 fixes some compatibility issues in linux kernels >= 5.0

-----------------------------------------------------
Version: 0.3.1.3
Date: 2018/12/10
Description: 
* The release 0.3.1.3 fixes some compatibility issues with vanilla and AMD custom kernels

-----------------------------------------------------
Version: 0.3.1.2
Date: 2018/09/10
Description: 
* The release 0.3.1.2 improves the compatibility with AMD custom kernels

-----------------------------------------------------
Version: 0.3.1.1
Date: 2018/08/31
Description: 
* The release 0.3.1.1 accelerates the LP detection on SPI 1

-----------------------------------------------------
Version: 0.3.1.0
Date: 2018/08/29
Description: 
* The release 0.3.1.0 enhances the capability to recover Logging Processor communication faults

-----------------------------------------------------
Version: 0.3.0.0
Date: 2017/12/11
Description: 
* The release 0.3.0.0 implements the SPI2 interrupt handling for led strip management

-----------------------------------------------------
Version: 0.2.4.1
Date: 2017/07/20
Description: 
* The release 0.2.4.1 fixes some issues related to the AMD-Intel compatibility

-----------------------------------------------------
Version: 0.2.3.5
Date: 2017/03/23
Description: 
* The release 0.2.3.5 fixes an old bug that prevents the user to get the hash code of BIOS and LP.
* The actual distribution name is now R_DRV_CORE instead of R_DRV_QXT

-----------------------------------------------------
Version: 0.2.2.0
Date: 2014/09/16    
Description: 
* Implemented new I/O Controls for new library functionality
* Added interoperability support for 32bit applications and 64bit device drivers
* Added support for latest Quixant Logic Controller device id, which is found in Quixant boards released after the first half of year 2015 

-----------------------------------------------------
Version: 0.1.5.0
Date: 2014/03/17    
Description: 
* Corrected a build error caused by an incorrect Makefile.

-----------------------------------------------------
Version: 0.1.4.0
Date: 2014/02/21    
Description: 
* Added support for more external SPI frequencies on newer Quixant boards.

-----------------------------------------------------
Version: 0.1.3.0
Date: 2014/02/12    
Description: 
* Made internal changes for HW inventory compatibility between Quixant legacy and current platforms.

-----------------------------------------------------
Version: 0.1.2.0
Date: 2013/02/03    
Description: 
* Added src path in the installation script.
