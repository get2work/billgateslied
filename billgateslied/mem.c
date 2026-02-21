#include "pch.h"
#include "mem.h"
#include "defs.h"
#include "msg.h"

NTSTATUS read_process_memory(HANDLE pid, PVOID source, PVOID dest, SIZE_T size)
{
	// Validate input parameters
    if (!pid || !source || !dest || size < 1)
        return STATUS_INVALID_PARAMETER;

	// Look up the target process by its PID, easy for usermode
    PEPROCESS process;
    NTSTATUS status = PsLookupProcessByProcessId(pid, &process);

    if (!NT_SUCCESS(status))
        return status;

	// Attach to the target process's address space using KeStackAttachProcess
	// increments object reference count. Can be detected by advanced anticheat.
    KAPC_STATE apc_state;
    KeStackAttachProcess(process, &apc_state);

	// Use RtlCopyMemory to copy memory from the target process to our buffer
    __try {
        RtlCopyMemory(dest, source, size);
        status = STATUS_SUCCESS;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        status = GetExceptionCode();
    }

	// Detach from the target process's address space
    KeUnstackDetachProcess(&apc_state);
	// Dereference the process object, reduces the reference count.
    ObDereferenceObject(process);
    return status;
}

NTSTATUS write_process_memory(HANDLE pid, PVOID source, PVOID dest, SIZE_T size)
{
    if (!pid || !source || !dest || size < 1)
        return STATUS_INVALID_PARAMETER;

    PEPROCESS process;
    NTSTATUS status = PsLookupProcessByProcessId(pid, &process);

    if (!NT_SUCCESS(status))
        return status;

    KAPC_STATE apc_state;
    KeStackAttachProcess(process, &apc_state);

    __try {
        RtlCopyMemory(dest, source, size);
        status = STATUS_SUCCESS;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        status = GetExceptionCode();
    }

    KeUnstackDetachProcess(&apc_state);
    ObDereferenceObject(process);

    return status;
}

NTSTATUS protect_memory(HANDLE pid, PVOID address, PSIZE_T size, ULONG protect, PULONG oldprotect)
{
    if (!address || !size || !oldprotect)
        return STATUS_INVALID_PARAMETER;

    if (*size == 0)
        return STATUS_INVALID_PARAMETER;

    PEPROCESS process;
    NTSTATUS status = PsLookupProcessByProcessId(pid, &process);

    if (!NT_SUCCESS(status))
        return status;

    KAPC_STATE apc_state;
    KeStackAttachProcess(process, &apc_state);

    __try {
        PVOID base_address = address;
        SIZE_T region_size = *size;
        
        // Call from target process context
        status = ZwProtectVirtualMemory(
            ZwCurrentProcess(),  // Now in target process context
            &base_address,
            &region_size,
            protect,
            oldprotect
        );
        
        *size = region_size;  // Update with actual protected size
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        status = GetExceptionCode();
    }

    KeUnstackDetachProcess(&apc_state);
    ObDereferenceObject(process);

    return status;
}