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

	WCHAR message[2] = { 0 };
	ULONG messageLength = 0;

	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		if (DeviceIoControl(deviceHandle, DEVICE_READ, NULL, 0, message, 2, &messageLength, 0))
		{
			POINT cursorPoint;
			GetCursorPos(&cursorPoint);

			const LONG pixels = 10;
			long int code = wcstol(message, NULL, 10);
			printf("%ld\n", code);

			switch (code) {
			case 1:		// UP
				printf("up\n");
				cursorPoint.y += pixels;
				break;

			case 2:		// DOWN
				cursorPoint.y -= pixels;
				break;

			case 3:		// LEFT
				cursorPoint.x -= pixels;
				break;

			case 4:		// RIGHT
				cursorPoint.x += pixels;
				break;

			default:
				break;
			}

			// Update cursor
			ShowCursor(FALSE);
			SetCursorPos(cursorPoint.x, cursorPoint.y);

			ShowCursor(TRUE);
		}

		Sleep(100);
	}

	CloseHandle(deviceHandle);
	return 0;
}