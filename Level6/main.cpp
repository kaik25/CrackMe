#include <Windows.h>
#include <stdio.h>
#define BUFFER_SIZE 6
#define FILENAME_SIZE 12
#define MAX_PATH_SIZE 256
#define CODE_SIZE 6


int main(){
	HANDLE hFile;
	int retVal = 0;
	char buffer[BUFFER_SIZE] = { 0 };
	char fileName[FILENAME_SIZE] = { 0 };
	char fileNameFullPath[MAX_PATH_SIZE] = { 0 };
	char code[CODE_SIZE] = { 0 };
	DWORD bytesRead = 0;
	SYSTEMTIME st = { 0 };
	char drive[2] = "c";
	char folder[3] = { 0 };

	GetLocalTime(&st);
	sprintf_s(fileName, FILENAME_SIZE, "%d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
	sprintf_s(folder, 3, "%d", st.wHour + 1);
	sprintf_s(fileNameFullPath, MAX_PATH_SIZE, "%s:\\%s\\%s.txt", drive, folder, fileName);
	sprintf_s(code, CODE_SIZE, "%d:%d", st.wHour, st.wMinute);
	
	hFile = CreateFile(fileNameFullPath, // open
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

	if (strcmp(buffer, code)){
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