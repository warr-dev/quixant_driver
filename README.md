# Release Notes - Quixant Module Installer for Linux (QMI4L)
----------------------------------------------------

## Description
--------------

Quixant Module Installer for Linux (QMI4L) is a solution which performs a series of operations on a standard Ubuntu Linux installation (performed by the Customer), making it fully working on Quixant machines.

## How to use

- uncompress the archive (tar -xzvf R_TOOLS_QMI4L_x.y.z.t.tgz)
- cd into created directory
- sudo ./qmi4l
- follow on screen instructions

## Supported OS
- Ubuntu 18.04
- Ubuntu 20.04
- Ubuntu 22.04
- Ubuntu 24.04
 
## Scenario
--------------
QMI for your platform is downloaded from Quixant Software Hub, and is packaged together with a tgz file containing all required drivers and service for your Quixant model.

## SYNOPSIS
qmi4l [ -s | -S |-u | -U | -p | -P | -h | -H]

-s,-S Installs drivers and services (overwrites old drivers and services)

-u,-U Uninstalls drivers and Services

-p,-P Prints currently loaded modules and services status

-h,-H Display command help

## Operations performed by QMI4L:
--------------------------------------

* driver handling via system service

* installation of Quixant specific services

* serial port renaming via system service

* setup of rules related to Quixant USB devices


## Change log:
--------------
#### **_Version 2.0.2.3_** - _Date: 2024/12/12_ 
+ [_bugfix_]  The serial port scan is now performed using the "udevadm" utility. (ref. Mantis issue 632)
+ [_bugfix_]  Fixed unexpected printouts during the package check. (ref. Mantis issue 656)
+ [_improvement_]  Added support for "Iq Connect 1" system model

#### **_Version 2.0.1.9_** - _Date: 2023/10/11_ 
+ [_bugfix_]  improved handling of R_SVC_CTRLHUB driver file on platform that do not support them (ref. Mantis issue 567)

#### **_Version 2.0.1.8_** - _Date: 2023/09/22_ 
+ [_bugfix_]  improved detection of missing libusb-1.0-0-dev. Now 64bit version is checked for presence (ref. Mantis issue 565)

#### **_Version 2.0.1.7_** - _Date: 2023/09/14_ 
+ [_bugfix_]  added missing printout for qxtpch in qxtDrv service (sudo service qxtDrv info)

#### **_Version 2.0.1.6_** - _Date: 2023/08/31_ 
+ [_improvement_]  added support for QXTCTRLHUB 
+ [_improvement_]  added check for installation of libusb-1.0-0-dev
---------------

#### **_Version 2.0.1.5_** - _Date: 2023/04/20_ 
+ [_improvement_]  added PID 7514 in udev rules file 91-quixant.rules
+ [_improvement_]  removed -i from options and update documentation accordingly
+ [_bugfix_]  changed check on libusb-1.0-0 installation 
---------------

#### **_Version 2.0.1.4_** - _Date: 2023/01/05_ 
+ [_improvement_]  no more error messages when no SVC files are inside main .tgz
---------------

#### **_Version 2.0.1.3_** - _Date: 2023/01/04_ 
+ [_improvement_]  made tgz filename independent from model name

#### **_Version 2.0.1.2_** - _Date: 2022/10/21_ 
+ [_bugfix_]  QX-70 handled as QXc-70 in the .tgz file

#### **_Version 2.0.1.1_** - _Date: 2022/10/19_ 
+ [_bugfix_]  fixed detection of ttyS, due to change of dmesg output in ubuntu22

#### **_Version 2.0.1.0_** - _Date: 2022/09/30_ 
+ [_improvement_]  service installation feature added
+ [_improvement_]  syntax alignment between Windows and Linux versions

#### **_Version 1.0.0.3_** - _Date: 2022/08/01_ 
+ [_bugfix_]  code cleanup


#### **_Version 1.0.0.2_** - _Date: 2022/07/15_ 
+ [_bugfix_] serial port detection method improved


#### **_Version 1.0.0.1_** - _Date: 2022/06/06_ 
+ [_improvement_] removed platform check, kept concerned PCI device presence


#### **_Version 1.0.0.0_** - _Date: 2022/05/06_ 
+ First release package
