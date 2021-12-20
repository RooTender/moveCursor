#include <Windows.h>
#include <stdio.h>

#define DEVICE_READ		CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_READ_DATA)

int main() {

	// Create file for communication with driver
	HANDLE deviceHandle = CreateFile(L"\\\\.\\keyboardscanner", GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
	if (deviceHandle == INVALID_HANDLE_VALUE) {
		printf("Error opening file. Code: %d", GetLastError());
		Sleep(3000);
		return 1;
	}

	printf("Connection estabilished!");

	WCHAR message[4] = { 0 };
	ULONG messageLength = 0;


	while (!GetAsyncKeyState(VK_ESCAPE)) {
		if (DeviceIoControl(deviceHandle, DEVICE_READ, NULL, 0, message, 4, &messageLength, 0)) {
			printf("%ls\n", message);
		}
	}

	CloseHandle(deviceHandle);
	return 0;
}