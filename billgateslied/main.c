#include "pch.h"
#include "defs.h"
#include "msg.h"
#include "io.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pRegistryPath) {
	// Declare pRegistry Path unused
	UNREFERENCED_PARAMETER(pRegistryPath);

	DebugMessage("DriverEntry called\n");

	//test copy virtual memory from this process to this process

	int a = 123;
	int b = 0;

	SIZE_T bytesCopied = 0;
	
	MmCopyVirtualMemory(
		PsGetCurrentProcess(),
		&a,
		PsGetCurrentProcess(),
		&b,
		sizeof(a),
		KernelMode,
		&bytesCopied
	);

	DebugMessage("Copied %d bytes from %p to %p, value: %d\n", (int)bytesCopied, &a, &b, b);

	pDriver->DriverUnload = UnloadDriver;
	
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriver) {
	UNREFERENCED_PARAMETER(pDriver);
	
	DebugMessage("UnloadDriver called\n");

	return STATUS_SUCCESS;
}