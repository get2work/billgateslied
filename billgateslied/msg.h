#pragma once

/*
* To handle driver debugging, we can use the DbgPrintEx function to print debug messages to the kernel debugger.
* We can use vmware to host a virtual machine, use a serial port to connect the virtual machine to the host machine, and use a kernel debugger (like WinDbg) to debug the driver in the virtual machine.
* We can also use a second computer to test our driver, and connect the two computers using a serial cable or a network connection, and use a kernel debugger to debug the driver on the second computer.
*/

// Cant see messages?
// HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Debug Print Filter
// Set the value of the "Default" key to 0xFFFFFFFF to enable all debug messages, or set it to a specific component ID and level to filter messages.

#define debug_msg(...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, __VA_ARGS__)