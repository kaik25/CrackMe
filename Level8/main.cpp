#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>


#define PROCMON_FILENAME "procmon.exe"
#define PROCEXP_FILENAME "procexp.exe"
#define NOTEPAD_FILENAME "notepad++.exe"
#define CMD_FILENAME "cmd.exe"

int isBadProcessRunning(DWORD processId){

	TCHAR szProcessName[MAX_PATH] = { 0 };
	char* imageName = NULL;
	HANDLE hProcess = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
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
		if (!strcmp(imageName, PROCEXP_FILENAME)){
			return TRUE;
		}
		if (!strcmp(imageName, CMD_FILENAME)){
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

void main(int argc, TCHAR *argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	if (isProcMonRunning()){
		printf("Failed!\n");
		return;
	}

	if (argc == 1){
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// Start the child process. 
		if (!CreateProcess(argv[0],   // No module name (use command line)
			" abc",        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			CREATE_NO_WINDOW, // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			printf("Failed!\n");
			return;
		}

		// Wait until child process exits.
		WaitForSingleObject(pi.hProcess, INFINITE);

		printf("OK!\n");
		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	} else if (argc == 2){
		system("pause");
		return;
	}
}