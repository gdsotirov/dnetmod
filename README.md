[![Build Status](https://travis-ci.com/gdsotirov/dnetmod.svg?branch=master)](https://travis-ci.com/gdsotirov/dnetmod)
[![Action Status](https://github.com/gdsotirov/dnetmod/workflows/C/C++%20CI/badge.svg)](https://github.com/gdsotirov/dnetmod/actions?query=workflow%3A%22C%2FC%2B%2B+CI%22)

# DNetMod (DeviceNet Module)

## Introduction
------------------------------------------------------------------------------

This document describes the DeviceNet(tm) Module. It's specially purposed for
C++ programmers (it's like a programmers' reference) but can be used by any
other person familiar with DeviceNet(tm) and C/C++ programming.

The DeviceNet(tm) Module is like a C++ shell over the C API of the
DeviceNet(tm) interfaces (master devices). The goal of this project is to
simplify the use of this interfaces by providing a universal way of
communication between your application, the interface and the devices in your
DeviceNet(tm) network.

DeviceNet(tm) Module has been designed to be expandable to support more and/or
new interfaces for DeviceNet(tm). This can be done by inheriting of the module
base classes to represent your interface and a device attached to it.

## Supported platforms
------------------------------------------------------------------------------

The module is designed to be used on Win32 and Linux platforms. The module
should work on Microsoft(r) Windows(r) 9x, NT, Me, 2000 and XP. Linux kernel
series 2.2 and 2.4 are supported. The primary target for the module is
Intel(r) ix86 platform, but should be compiled on other platforms with no or
small changes.

On Win32 platform supported compilers are Microsoft(r) Visual C++(r),
Borland(r) C++ Builder(tm), Watcom C/C++ and possibly others. On Linux
platform GNU GCC is supported and possibly Intel(r) C++ Compiler for Linux.

Note: If you wish to port the module to other platform/compiler be sure
structures in files cif_user.h, cifuser.h, rcs_user.h and dnm_user.h to be
packed and aligned on 1 byte boundary.

## Supported hardware
------------------------------------------------------------------------------

Currently the DeviceNet(tm) Module supports only two interfaces:

  * National Instruments(tm) NI-DNET(tm) Interface
  * Hilscher CIF-DNM Interfaces (Tested only with CIF-50DNM! But should work
    with all other Hilscher boards.)

## Features
------------------------------------------------------------------------------

DeviceNet(tm) Module provides general functionality to the programmer who uses
it. In the same way independent from the interface you can communicate with
your devices. With the DeviceNet(tm) Module you can do:

  * open, close and reset your interface;
  * allocate/deallocate a device on the network;
  * read data from a device;
  * write data to a device;
  * get device's attributes;
  * set device's attributes;
  * execute DeviceNet(tm) services.

## Module interface
------------------------------------------------------------------------------

Module API is provided thru functions and classes. To find more about them
please follow the links below. You can view whole class hierarchy presented
in a tree view (see hlp/).

* Functions
  - DevTypeToString
  - GetErrMsg
  - VendIdToString
* Classes
  - CIdentificator
  - CNode
  - CInterface
  - CDevice
  - CNIInterface
  - CNIDevice
  - CCIFInterface
  - CCIFDevice
