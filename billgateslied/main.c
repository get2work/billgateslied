#include "pch.h"
#include "defs.h"
#include "msg.h"
#include "io.h"
#include "mem.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegistryPath) {
	// Declare pRegistry Path unused
	UNREFERENCED_PARAMETER(pRegistryPath);
	
	//Im testing the driver using kdmapper, kdmapper doesnt pass valid pDriver and pRegistryPath
	UNREFERENCED_PARAMETER(pDriver);

	DebugMessage("DriverEntry called\n");

	//test copy virtual memory from this process to this process

	PVOID poolA = ExAllocatePool2(POOL_FLAG_NON_PAGED, sizeof(int), 'Tag1');
	PVOID poolB = ExAllocatePool2(POOL_FLAG_NON_PAGED, sizeof(int), 'Tag2');
	
	if (!poolA || !poolB) 
		return STATUS_NO_MEMORY;
	
	RtlZeroMemory(poolA, sizeof(int));
	RtlZeroMemory(poolB, sizeof(int));

	// write 123 to poolA
	*(int*)poolA = 123;

	SIZE_T bytesCopied = 0;

	__try {
		NTSTATUS result = write_process_memory(PsGetCurrentProcessId(), (PULONG)poolA, poolB, sizeof(int));

		if (!NT_SUCCESS(result)) {
			DebugMessage("write_process_memory failed with status: 0x%X\n", result);
		}
		else {
			DebugMessage("Copied %d bytes from %p to %p, value: %d\n", (int)bytesCopied, poolA, poolB, *(int*)poolB);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		ULONG exceptionCode = GetExceptionCode();
		DebugMessage("Exception occurred during write_process_memory: 0x%X\n", exceptionCode);
	}

	//Free the allocated memory
	ExFreePoolWithTag(poolA, 'Tag1');
	ExFreePoolWithTag(poolB, 'Tag2');
	
	//Im testing the driver using kdmapper, kdmapper doesnt pass valid pDriver and pRegistryPath
	//
	//pDriver->DriverUnload = UnloadDriver;
	
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriver) {
	UNREFERENCED_PARAMETER(pDriver);
	
	DebugMessage("UnloadDriver called\n");

	return STATUS_SUCCESS;
}