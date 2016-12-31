#include <Windows.h>
#include <stdio.h>

int main(){
	HANDLE hFile;
	int retVal = 0;

	hFile = CreateFile(TEXT("c:\\temp\\31_12_16.txt"), // open One.txt
		GENERIC_READ,             // open for reading
		0,                        // do not share
		NULL,                     // no security
		OPEN_EXISTING,            // existing file only
		FILE_ATTRIBUTE_NORMAL,    // normal file
		NULL);                    // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND){
			printf("Could not find c:\\temp\\31_12_16.txt.\n");
		} 
		else {
			printf("Could not open c:\\temp\\31_12_16.txt.\n");
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