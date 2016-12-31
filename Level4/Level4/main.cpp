#include <Windows.h>
#include <stdio.h>
#define BUFFER_SIZE 5

int main(){
	HANDLE hFile;
	int retVal = 0;
	char buffer[BUFFER_SIZE] = { 0 };
	DWORD bytesRead = 0;

	hFile = CreateFile(TEXT("c:\\temp\\crackme4.txt"), // open One.txt
		GENERIC_READ,             // open for reading
		0,                        // do not share
		NULL,                     // no security
		OPEN_EXISTING,            // existing file only
		FILE_ATTRIBUTE_NORMAL,    // normal file
		NULL);                    // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		retVal = 1;
		goto lblEnd;
	}	

	if (FALSE == ReadFile(hFile,
							buffer,
							BUFFER_SIZE - 1,
							&bytesRead,
							NULL)
							)
	{
		retVal = 1;
		goto lblEnd;
	} 

	if (bytesRead != BUFFER_SIZE - 1){
		retVal = 1;
		goto lblEnd;
	}
lblEnd:
	CloseHandle(hFile);
	if (retVal){
		printf("Failed!\n");
	}
	else {
		printf("OK!\n");
	}
	return retVal;
}