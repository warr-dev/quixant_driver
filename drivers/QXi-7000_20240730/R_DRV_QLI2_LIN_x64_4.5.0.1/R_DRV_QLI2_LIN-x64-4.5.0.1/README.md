# Release Notes - QLI-2 drivers

## Description
--------------
Drivers for Quixant QLI-2, the enhanced version of the integrated multi-port multi-protocol LED strip controller devices.

## Requirements
---------------
- Windows: Windows 7-SP1/8/8.1/10
- Linux: any kernel starting from 3.19

## License
----------
Provided under Quixant SLA. Redistribution prohibited, unless otherwise stated by Quixant NDA and/or other agreements.

## Change Log:
--------------
* __version 4.5.0.1:__
_2024/05/31 04:40:55; AM_
Fixed Linux compatibility with kernel v.6.8

--------------
* __version 4.5.0.0:__
_2024/02/13 10:19:20 AM_
Fixed Linux compatibility with kernel v.6.6

* __version 4.4.0.2:__
_2023/11/23 12:03:05 AM_
Added support for QLI2 with 9 channels PWM

* __version 4.3.1.0:__
_2023/3/28 04:50:11 AM_
Fixed Linux compatibility with kernel 5.18

* __version 4.3.0.1:__
_2022/1/28 03:43:00 AM_
Fixed Linux compatibility with kernel 5.12

* __version 4.3.0.0:__
_2021/8/18 08:45:47 AM_
Added configurable PWM frequency refresh

* __version 4.2.0.2:__
_2021/6/16 02:53:15 PM_
Fixed Linux compatibility with kernel 5.11

* __version 4.2.0.1:__
_2021/6/16 01:33:05 PM_
Fixed Linux compatibility with kernel 5.10

* __version 4.2.0.0:__
_2021/1/28 03:51:50 PM_
Added Linux and Windows compatibility with qli2 fpga module v.1.4
Fixed issue setting SPI frequency initialization

* __version 4.0.0.8:__
_2020/7/27 11:29:31 AM_
Added explicit qtree deletion in driver termination and new information in QTree

* __version 4.0.0.7:__
_2020/05/19 02:40:00 PM_
Fixed compatibility issue with Linux AMD kernel v.5.4.2-1138

* __version 4.0.0.6:__
_2020/03/19 12:04:00 PM_
Settings of default values for each channel during the driver loading

* __version 4.0.0.5:__
_2020/02/26 03:58:40 PM_
Windows fix in IOCTL_QLI_GET_CHANNEL_CFG

* __version 4.0.0.4:__
_2020/02/12 12:30:21 PM_
Fixed issue in One-Wire LED timeout

* __version 4.0.0.3:__
_2020/01/29 03:57:25 PM_
Fixed structure alignments for windows compatibility.
First public release for Windows and Linux

* __version 4.0.0.2:__ 
_2019/12/09 11:25:42 AM_
Added disable channels IOCTL

* __version 4.0.0.1:__ 
_2019/12/04 05:20:00 PM_
Optimized CPU-FPGA memory transfer

* __version 4.0.0.0:__ 
_2019/11/11 12:07:10 PM_
First release
