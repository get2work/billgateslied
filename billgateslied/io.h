#pragma once
#include <ntifs.h>

/*
For input output operations, we have many options:
1. We use IOCTL to send data from user mode to kernel mode, and vice versa.
	- We define IOCTL codes in a header file, and implement the corresponding IOCTL handlers in the driver.
	- We use DeviceIoControl in user mode to send data to the driver, and the driver can use the output buffer to send data back to user mode.
	- IOCTL requires the driver to "exist" in the system, less stealthy, but easier to implement and use.

2. We use a shared memory section to share data between user mode and kernel mode
3. We use a named pipe to send data from user mode to kernel mode
4. We use a file to write data from user mode and read it in kernel mode
5. We use a registry key to write data from user mode and read it in kernel mode
6. We use a network socket to send data from user mode to kernel mode
7. We hook a system call that has a pointer argument and use it to send data from user mode to kernel mode
*/

#define IOCTL_CHANGE_PROTECTION CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WRITE_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_READ_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)