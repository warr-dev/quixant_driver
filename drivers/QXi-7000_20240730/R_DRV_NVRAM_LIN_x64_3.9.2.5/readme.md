# NVRAM Module Device Drivers for Linux OS


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

In order to install the kernel modules run as root the following command:

~~~
./qxt
~~~

## Release Notes
-----------------------------------------------------
**Version: 3.9.2.5, 2024/07/10**

* Updated version 3.9.2.5.
  Fixed issue in RW\_SYNC\_MODE + AUTOVERIFY

-----------------------------------------------------
**Version: 3.9.2.4, 2024/06/03**

* Updated version 3.9.2.4.
  Fixed issue in RW\_SYNC\_MODE

-----------------------------------------------------
**Version: 3.9.2.3, 2024/05/31**

* Updated version 3.9.2.3.
  Compatible with Linux kernel v.6.8

-----------------------------------------------------
**Version: 3.9.2.2, 2024/03/11**

* Updated version 3.9.2.2.
  Compatible with Linux kernel v.6.6

-----------------------------------------------------
**Version: 3.9.2.0, 2023/11/23**

* Updated version 3.9.2.0.
  Fixed wrong memory size report in QSYS

-----------------------------------------------------
**Version: 3.9.1.0, 2023/11/09**

* Updated version 3.9.1.0.
  Compatible with obsoleted Quixant board QXi300 
  
-----------------------------------------------------
**Version: 3.9.0.0, 2023/09/14**

* Updated version 3.9.0.0.
  Compatible with new boards IQON-2 and IQ-2 
  
-----------------------------------------------------
**Version: 3.8.0.1, 2022/01/28**

* Updated version 3.8.0.1.
  Compatible with Linuk kernel v.5.12
  
-----------------------------------------------------
**Version: 3.8.0.0, 2021/06/16**

* Updated version 3.8.0.0.
  Compatible with Linuk kernel v.5.11
  
-----------------------------------------------------
**Version: 3.7.0.0, 2021/06/09**

* Updated version 3.7.0.0.
  Added compatibility with Intel architecture and support for memory mapping access

-----------------------------------------------------
**Version: 3.6.0.1, 2021/04/27**

* Updated version 3.6.0.1.
  Fixed issues in multithread memory access

-----------------------------------------------------
**Version: 3.6.0.0, 2021/03/27**

* Updated version 3.6.0.0.
  Compatible with Linuk kernel v.5.8 - 5.10

-----------------------------------------------------
**Version: 3.5.0.0, 2021/02/07**

* Updated version 3.5.0.0.
  Added compatibility with IQ-1

-----------------------------------------------------
**Version: 3.4.1.7, 2020/10/12**

* Fixed return code in driverscomp tool during the driver installation.
  If the installation fails, driverscomp returns -1, 0 otherwise.

-----------------------------------------------------
**Version: 3.4.1.6, 2020/10/01**

* New release that fixes an issue regarding the CRC 16 alignments

-----------------------------------------------------
**Version: 3.4.1.5, 2020/05/19**

* New release supporting the linux AMD kernel 5.4.2-1138

-----------------------------------------------------
**Version: 3.4.1.4, 2020/05/14**

* New release with linux kernel >= 5.6 support

-----------------------------------------------------
**Version: 3.4.1.3, 2020/02/18**

* New release with linux kernel >= 5.3 support

-----------------------------------------------------
**Version: 3.4.1.2, 2019/10/22**

* New release with linux kernel 5.x support

-----------------------------------------------------
**Version: 3.4.1.1, 2018/09/14**

* New release with q-tree support enabled and fix in installation procedure

-----------------------------------------------------
**Version: 3.4.1.0, 2018/08/30**

* Added q-tree support

-----------------------------------------------------
**Version: 3.4.0.1, 2018/08/28**

* Added the capability to handle different mapping cache modes through the insmod command line:

~~~
insmod ./qxtnvram.ko [cache=<mode>]
~~~

where _mode_ is:

- wc = write combine
- wb = write back
- wt = write through (only supported in kernel >= 4.2)
- uc = uncacheable (only supported in kernel >= 4.2)
- nc = no cache

If omitted or unsupported, the default mapping mode is kernel dependant (typically no cache)

-----------------------------------------------------
**Version: 3.4.0.0, 2018/07/20**

* Added support for new modes MIRROR\_32, NORMAL\_MODE\_64 and the new CRC\_16

-----------------------------------------------------
**Version: 3.3.0.1, 2018/07/12**

* Fixed issue in detecting errors during write operations in RW\_SYNC + AUTOVERIFICATION mode

-----------------------------------------------------
**Version: 3.3.0.0, 2017/09/29**

* Added support for Kernel version >= 4.12
* Fixed bug affecting the RW\_SYNC + MIRROR mode


-----------------------------------------------------
**Version: 3.2.0.0, 2017/09/22**

* Added support for the old QX40 custom boards


-----------------------------------------------------
**Version: 3.1.0.0, 2017/05/24**

* Added support for Intel platforms

-----------------------------------------------------
**Version: 2.3.0.0, 2015/09/18**

* Added support for latest Quixant Logic Controller device id, which is found in Quixant boards released after the first half of year 2015.

-----------------------------------------------------
**Version: 2.2.3.0, 2015/05/13**

* Minor upgrade for Linux Kernel 3.19 and above compatibility.

-----------------------------------------------------
**Version: 2.2.2.0, 2014/09/04**

* Fixed a bug when returning the chipsize of a board with a single memory chip.

-----------------------------------------------------
**Version: 2.2.1.0, 2014/05/27**

* Improved memory chip size calculation.
* Added support for library function qxt\_nvram\_full\_inventory.

-----------------------------------------------------
**Version: 2.1.2.0, 2014/01/30**	

* Build environment bug fixes.

-----------------------------------------------------
**Version: 2.1.1.0, 2013/09/10**

* Fixed a bug which produced wrong readings of memory size before setting the operation mode.

-----------------------------------------------------
**Version: 2.1.0.9, 2013/09/05**

* Fixed a bug which caused incorrect reading of Bank ID.

-----------------------------------------------------
**Version: 2.1.0.8, 2013/07/02**

* Bug fixes.

-----------------------------------------------------
**Version: 2.1.0.7, 2013/03/07**

* Added possibility to "lock" nvram access.

-----------------------------------------------------
**Version: 2.1.0.0, 2013/02/03**

* Minor clean up.


