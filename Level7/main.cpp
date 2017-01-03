#include <Windows.h>
#include <stdio.h>
#include <psapi.h>
#include <string.h>

#define PROCMON_FILENAME "procmon.exe"
#define NOTEPAD_FILENAME "notepad++.exe"


int isBadProcessRunning(DWORD processId){

	TCHAR szProcessName[MAX_PATH] = { 0 };
	char* imageName = NULL;
	HANDLE hProcess = OpenProcess(
									PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,
									FALSE,
									processId);

	// Get the process name.

	if (NULL != hProcess)
	{
		GetProcessImageFileName(hProcess, szProcessName, MAX_PATH);
		char* imageName = strrchr(szProcessName, '\\') + sizeof('\\');

		if (!strcmp(imageName, PROCMON_FILENAME)){
			return TRUE;
		}
		if (!strcmp(imageName, NOTEPAD_FILENAME)){
			return TRUE;
		}

	}

	CloseHandle(hProcess);
	return FALSE;
}


int isProcMonRunning()
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	DWORD processId = 0;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return 1;
	}

	cProcesses = cbNeeded / sizeof(DWORD);
	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			if (isBadProcessRunning(aProcesses[i])){
				return TRUE;
			}
		}
	}
	return FALSE;
}


int main(){
	HANDLE hFile = NULL;
	int retVal = 0;
	
	if (isProcMonRunning()){
		printf("Bad process is running, BYE!\n");
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