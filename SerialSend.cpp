//
// SerialSend.cpp - This program sends text via serial port
// Written by Ted Burke - last updated 5-12-2011
//
// The specified text is sent via the specified serial port at 38400 baud.
// The text cannot contain any spaces.
//
// 		argv[1] = device name
//		argv[2] = text to send
//
// To compile:
//
//		g++ -o SerialSend.exe SerialSend.cpp
//
// To run (this example sends the characters "S365" via COM1):
//
//		SerialSend COM1 S356
//
// References:
//
//	Robertson Bayer, "Windows Serial Port Programming", March 30, 2008
//		(I used Bayer's serial port code as my starting point for this)
//

#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[])
{	
	// Parse command line arguments
	if (argc < 2)
	{
		fprintf(stderr, "Usage:\n\n\tSerialSend DEVICE_NAME TEXT_TO_SEND\n");
		return 1;
	}
	
	// Open the specified serial port (first command line argument)
	HANDLE hSerial;
	hSerial = CreateFile(argv[1], GENERIC_READ|GENERIC_WRITE, 0, 0,
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hSerial==INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Error: serial port %s could not be opened\n", argv[1]);
		return 1;
	}
	
	// Set device parameters (38400 baud, 1 start bit, 1 stop bit, no parity)
	DCB dcbSerialParams = {0};
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error getting device state\n");
		CloseHandle(hSerial);
		return 1;
	}		
	dcbSerialParams.BaudRate = CBR_38400;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if(SetCommState(hSerial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error setting device parameters\n");
		CloseHandle(hSerial);
		return 1;
	}
	
	// Set COM port timeout settings
	COMMTIMEOUTS timeouts = {0};
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	if(SetCommTimeouts(hSerial, &timeouts) == 0)
	{
		fprintf(stderr, "Error setting timeouts\n");
		CloseHandle(hSerial);
		return 1;
	}
	
	// Send specified text (second command line argument; cannot contain spaces)
	DWORD dwBytesWritten = 0;
	if(WriteFile(hSerial, argv[2], strlen(argv[1]), &dwBytesWritten, NULL) == 0)
	{
		fprintf(stderr, "Error writing text to %s\n", argv[1]);
	}
	
	// Close serial port
	if (CloseHandle(hSerial) == 0);
	{
		fprintf(stderr, "Error closing serial device %s\n", argv[1]);
		return 1;
	}
	
	// exit normally
	return 0;
}
