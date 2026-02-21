#include "pch.h"
#include "defs.h"
#include "msg.h"
#include "io.h"
#include "mem.h"

static BOOLEAN driver_demo() {

	//test copy virtual memory from this process to this process
	PVOID pool_a = ExAllocatePool2(POOL_FLAG_NON_PAGED, sizeof(int), 'tag1');
	PVOID pool_b = ExAllocatePool2(POOL_FLAG_NON_PAGED, sizeof(int), 'tag2');

	if (!pool_a || !pool_b)
		return FALSE;

	RtlZeroMemory(pool_a, sizeof(int));
	RtlZeroMemory(pool_b, sizeof(int));

	// write 123 to pool_a
	*(int*)pool_a = 123;

	SIZE_T size = sizeof(int);

	__try {
		NTSTATUS result = write_process_memory(PsGetCurrentProcessId(), pool_a, pool_b, size);

		if (!NT_SUCCESS(result)) {
			debug_msg("write_process_memory failed with status: 0x%X\n", result);
		}
		else {
			debug_msg("Copied %d bytes from %p to %p, value: %d\n", (int)size, pool_a, pool_b, *(int*)pool_b);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		ULONG exceptionCode = GetExceptionCode();
		debug_msg("Exception occurred during write_process_memory: 0x%X\n", exceptionCode);
	}

	//Free the allocated memory
	ExFreePoolWithTag(pool_a, 'tag1');
	ExFreePoolWithTag(pool_b, 'tag2');

	return TRUE;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT p_driver, PUNICODE_STRING p_registry_path) {
	// Declare pRegistry Path unused
	UNREFERENCED_PARAMETER(p_registry_path);

	//Im testing the driver using kdmapper, kdmapper doesnt pass valid pDriver and pRegistryPath
	UNREFERENCED_PARAMETER(p_driver);

	debug_msg("DriverEntry called\n");

	if (!driver_demo())
		return STATUS_NO_MEMORY;

	//p_driver->DriverUnload = UnloadDriver;
	
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT p_driver) {
	UNREFERENCED_PARAMETER(p_driver);

	debug_msg("UnloadDriver called\n");

	return STATUS_SUCCESS;
}