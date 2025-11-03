# Release notes - DRVTRACER

## Description:
---------------
Driver interface to the TRACER device. Aims at being very minimal.

## Compilation instructions:
----------------------------

In order to compile the driver for your Kernel use the following command:

`./driverscomp`

if everything is fine you should get the ko file (Kernel Module)

To build the ko file for a specific kernel version, please use
the -s switch on the command line, as in the following example:

`./driverscomp -s /lib/modules/3.5.0-22-generic/build`

## Install instructions:
------------------------

In order to install the kernel modules run the following command as Root:

`./qxt`

## About qtracer.h
------------------

Instead of providing a full library, ATS comes with a set of inline function usable to define your own traces.
`qtracer.h` is the file needed to use these inline functions in C/C++ programs.
For more information, please see ATS documentation/howto/application notes.

## Change log:
--------------
* **version 2.3.1.0:**
    * Fixed compatibility with linux kernel > 6.7.11
* **version 2.3.0.1:**
    * Fixed compatibility with linux kernel 6.6
* **version 2.3.0.0:**
    * Fixed compatibility with linux kernel 5.18
* __version 2.2.0.1:__
	*  Fixed compatibility with linux kernel 5.12
* __version 2.2.0.0:__
	*  Fixed compatibility with linux kernel 5.10
* __version 2.1.0.2:__
	*  Fixed compatibility with linux kernel >= 5.x
* __version 2.1.0.1:__
	*  Fixed warning in Linux compilation
* __version 2.1.0.0:__
	*  Configuration moved to BIOS
* __version 2.0.0.1:__
	*  Added configuration file
	*  Added maintenance IOCTLs
* __version 1.0.0.3:__ Added explicit ATS activation on driver init
* __version 1.0.0.0:__ First release :baby:



