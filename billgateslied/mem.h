#pragma once
#include <ntifs.h>


//this is essentially what MmCopyVirtualMemory does internaly
//note you can just cast pid to handle
extern NTSTATUS read_process_memory(HANDLE pid, PVOID source, PVOID dest, SIZE_T size);
extern NTSTATUS write_process_memory(HANDLE pid, PVOID source, PVOID dest, SIZE_T size);

//uses ZwProtectVirtualMemory to change the protection of a region of memory in a specific process
extern NTSTATUS protect_memory(HANDLE pid, PVOID address, PSIZE_T size, ULONG protect, PULONG oldprotect);