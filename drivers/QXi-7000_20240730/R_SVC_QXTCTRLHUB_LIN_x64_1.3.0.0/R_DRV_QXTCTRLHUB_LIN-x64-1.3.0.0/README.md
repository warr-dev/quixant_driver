# Release notes - drvqxt\_ctrlhub

**Description**:
------------
Kernel-mode service to access the Quixant Intel Controller Hub

**Requirements**: 
-------------
* CPU compatibility: INTEL and AMD processors
* Operating system: Linux kernel v.4.0 or greater, Windows 10 or greater

**License**:
--------
`drvqxt_ctrlhub` is released under Quixant NDA agreement

**Instructions**
-------------

### Linux

In order to compile the driver for your Kernel, use the following command:

~~~
./driverscomp                                                                     
~~~

if everything is fine, you should get the ko file (Kernel Module).

To build the .ko file for a specific kernel version, please use the "-s" switch on the command line, as in the following example:

~~~
./driverscomp -s /lib/modules/3.5.0-22-generic/build
~~~

In order to install the kernel modules, run as root the following command:

~~~
./qxt
~~~


### Windows

The driver must be installed as a kernel service. Moreover, we strongly suggest to configure the system to load the driver at boot.

To install, unzip all content in a folder and use the system tool `sc.exe`; open a Command Window or a PowerShell Window as Administrator and type:

`sc.exe create QxtCtrlHub binpath= "/absolute/path/to/drvqxt_ctrlhub.sys" type= kernel start= auto`

Substitute the `/absolute/path/to/drvqxt_ctrlhub.qsys` with the appropriate path to the driver on your system. The parameter `start=auto` instructs the system to load the driver during the system services startup.

To load the driver the first time, without rebooting, type:

`sc.exe start QxtCtrlHub`

To check the status of the service, type:

`sc.exe query QxtCtrlHub type=kernel`

To unload the driver, use the command:

`sc.exe stop QxtCtrlHub`

To uninstall the service:

`sc.exe delete QxtCtrlHub`


**Change log**:
-----------
* __version  1.3.0.0__: Rebuilt for Linux kernel v.6.8

* __version  1.2.1.0__: Rebuilt for Linux kernel v.6.5

* __version  1.2.0.1__: Rebuilt Linux release with glibc 2.27

* __version  1.2.0.0__: Added Linux compatibility

* __version  1.1.1.0__: Fixed compatibility with AMD, enabled shared access mode

* __version  1.1.0.0__: Added IOCTL_QXT_SPI_READ_BIOS_SIZE, SMBUS IOCTLs, LPC IOCTLs

* __version  1.0.0.1__: Enhancement in flash reading times

* __version  1.0.0.0__: Initial release
