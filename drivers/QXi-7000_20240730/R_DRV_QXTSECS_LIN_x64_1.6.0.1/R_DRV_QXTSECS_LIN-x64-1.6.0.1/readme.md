# Security Subsystem Device Drivers for Linux OS


## Installation

In order to compile the driver for your Kernel use the following command:

~~~
./driverscomp
~~~

if everything is fine you should get the ko file (Kernel Module).

To build the .ko file for a specific kernel version, please use the "-s" switch on the command line, as in the following example:

~~~
./driverscomp -s /lib/modules/3.5.0-22-generic/build
~~~

In order to install the kernel modules, run as root the following command:

~~~
./qxt
~~~

## Release Notes
-----------------------------------------------------
**Version: 1.6.0.1 - 2024/05/31**
* Added support for Linux kernel 6.8

-----------------------------------------------------
**Version: 1.6.0.0 - 2024/02/13**
* Added support for Linux kernel 6.6

-----------------------------------------------------
**Version: 1.5.1.0 - 2023/07/11**
* Added support for kernel 5.19
* Linux release: added return code -EPERM from IOCTL\_SEC\_DMAALLOC when the DMA memory allocated by the kernel cannot be mapped in user-space

-----------------------------------------------------
**Version: 1.5.0.0 - 2023/02/21**
* Added support for kernel 5.18

-----------------------------------------------------

**Version: 1.4.0.7 - 2023/01/03**
* Added new ioctl IOCTL\_SEC\_GET\_STATUS

-----------------------------------------------------
**Version: 1.4.0.6 - 2022/07/07**
* Added support for Ubuntu 22.04 and kernel 5.15

-----------------------------------------------------
**Version: 1.4.0.5 - 2022/02/03**
* Fixed issue in memory copy during encryption

-----------------------------------------------------
**Version: 1.4.0.4 - 2022/01/28**
* Added support for linux kernel 5.12

-----------------------------------------------------
**Version: 1.4.0.3 - 2021/11/25**
* Added support for Delayed Authentication

-----------------------------------------------------
**Version: 1.4.0.0 - 2021/10/28**
* Added support for Authorization and Master Key ready in AES status register

-----------------------------------------------------
**Version: 1.3.1.1 - 2021/10/12**
* Added support for Master key injection

-----------------------------------------------------
**Version: 1.3.1.0 - 2021/09/22**
* Added support for user defined keys

-----------------------------------------------------
**Version: 1.3.0.0 - 2021/06/16**
* Fix for Linux kernel >= 5.11

-----------------------------------------------------
**Version: 1.2.2.0 - 2021/03/23**
* Fix for Linux kernel >= 5.10

-----------------------------------------------------
**Version: 1.2.1.4 - 2020/10/20**
* Fix for Linux kernel >= 5.9

-----------------------------------------------------
**Version: 1.2.1.3 - 2020/10/14**
* Fix in encryption/decryption procedure when memory allocated buffer is <= 4Kb

-----------------------------------------------------
**Version: 1.2.1.2 - 2020/08/21**
* Fix for OneWay memory management for any kernel version

-----------------------------------------------------
**Version: 1.2.1.1 - 2020/08/18**
* Fix for OneWay memory management in Linux kernel >= 5.4

-----------------------------------------------------
**Version: 1.2.1.0 - 2020/06/13**
* Fix for DMA allocation on any new AMD cpu

-----------------------------------------------------
**Version: 1.2.0.2 - 2019/09/25**
* Fix for any Linux kernel up to 5.0

-----------------------------------------------------
**Version: 1.2.0.1 - 2019/01/23**
* Fix for Linux kernel from AMD

-----------------------------------------------------
**Version: 1.2.0.0 - 2017/09/05**
* Fix for Linux kernel version >= 4.11

-----------------------------------------------------
**Version: 1.1.2.6 - 2016/10/10**
* Set 4MB limit in dma memory allocation

-----------------------------------------------------
**Version: 1.1.2.0 - 2016/02/12**
* First public release
