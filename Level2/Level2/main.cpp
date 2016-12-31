#include <Windows.h>
#include <stdio.h>

int main(){
	HANDLE hFile;
	int retVal = 0;

	hFile = CreateFile(TEXT("c:\\temp\\1d6asdf6a1sd8f1a6sd8f4as6df1a6sd.txt"), // open One.txt
		GENERIC_READ,             // open for reading
		0,                        // do not share
		NULL,                     // no security
		OPEN_EXISTING,            // existing file only
		FILE_ATTRIBUTE_NORMAL,    // normal file
		NULL);                    // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND){
			printf("Failed!\n");
		} 
		else {
			printf("Failed!\n");
		}
		retVal = 1;
	}	
	else {
		printf("OK!\n");
		CloseHandle(hFile);
	}
	system("pause");
	return retVal;
}