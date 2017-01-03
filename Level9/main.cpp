#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <winreg.h>\


char LICENSE_KEY_NAME[] = "Tpguxbsf]Lpej";
char LICENSE_VALUE_NAME[] = "Je";
char OK_MSG[] = "PL\"";
char FAILED_MSG[] = "Gbjmfe\"";

void deobfuscateString(char* obfuscatedString, int stringLength)
{
	int i = 0;
	for (i = 0; i < stringLength; i++){
		obfuscatedString[i] -= 1;
	}
}

void obfuscateString(char* obfuscatedString, int stringLength)
{
	int i = 0;
	for (i = 0; i < stringLength; i++){
		obfuscatedString[i] += 1;
	}
}


int doesLicenseKeyExist()
{
	HKEY licenseKey = NULL;
	LONG valueSize = 0;

	deobfuscateString(LICENSE_KEY_NAME, strlen(LICENSE_KEY_NAME));
	if (RegOpenKeyEx(HKEY_CURRENT_USER, LICENSE_KEY_NAME, 0, KEY_READ, &licenseKey) != ERROR_SUCCESS)
	{
		obfuscateString(LICENSE_KEY_NAME, strlen(LICENSE_KEY_NAME));
		return FALSE;
	} 
	obfuscateString(LICENSE_KEY_NAME, strlen(LICENSE_KEY_NAME));
	

	deobfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));
	if (RegQueryValueEx(licenseKey, LICENSE_VALUE_NAME, NULL, NULL, NULL, NULL)){
		obfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));
		return FALSE;
	}
	obfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));

	RegCloseKey(licenseKey);
	return TRUE;

}

int createLicenseKey()
{
	HKEY licenseKey = NULL;
	DWORD InvalidLicense = 0;

	deobfuscateString(LICENSE_KEY_NAME, strlen(LICENSE_KEY_NAME));
	if (RegOpenKeyEx(HKEY_CURRENT_USER, LICENSE_KEY_NAME, 0, KEY_SET_VALUE, &licenseKey) != ERROR_SUCCESS)
	{
		
		if (RegCreateKey(HKEY_CURRENT_USER, LICENSE_KEY_NAME, &licenseKey) != ERROR_SUCCESS){
			obfuscateString(LICENSE_KEY_NAME, strlen(LICENSE_KEY_NAME));
			return FALSE;
		}
	}
	obfuscateString(LICENSE_KEY_NAME, strlen(LICENSE_KEY_NAME));

	deobfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));
	if (RegSetValueEx(
		licenseKey,
		LICENSE_VALUE_NAME,
		NULL,
		REG_BINARY,
		(BYTE*)&InvalidLicense,
		sizeof(DWORD) != ERROR_SUCCESS))
	{
		obfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));
		return FALSE;
	}
	obfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));
	
	RegCloseKey(licenseKey);
	return TRUE;
}

int isKeyValid(){
	HKEY licenseKey = NULL;
	DWORD valueSize = 0;
	DWORD value = 0;
	DWORD dataType = sizeof(DWORD);

	deobfuscateString(LICENSE_KEY_NAME, strlen(LICENSE_KEY_NAME));
	if (RegOpenKeyEx(HKEY_CURRENT_USER, LICENSE_KEY_NAME, 0, KEY_READ, &licenseKey) != ERROR_SUCCESS)
	{
		obfuscateString(LICENSE_KEY_NAME, strlen(LICENSE_KEY_NAME));
		return FALSE;
	}
	obfuscateString(LICENSE_KEY_NAME, strlen(LICENSE_KEY_NAME));

	deobfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));
	if (RegQueryValueEx(licenseKey, LICENSE_VALUE_NAME, NULL, &dataType, NULL, &valueSize)){
		obfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));
		return FALSE;
	}
	obfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));

	if (dataType != REG_BINARY || valueSize > sizeof(DWORD)){
		return FALSE;
	}

	deobfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));
	if (RegQueryValueEx(licenseKey, LICENSE_VALUE_NAME, NULL, NULL, (BYTE*)&value, &valueSize)){
		obfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));
		return FALSE;
	}
	obfuscateString(LICENSE_VALUE_NAME, strlen(LICENSE_VALUE_NAME));

	if (value != 0x1337){
		return FALSE;
	}

	RegCloseKey(licenseKey);
	return TRUE;
}

void main()
{
	int retVal = 0;

	if (!doesLicenseKeyExist()){
		if (!createLicenseKey()){
			retVal = 1;
			goto lblEnd;;
		}
	}

	if (!isKeyValid()){
		retVal = 1;
		goto lblEnd;;
	}


lblEnd:
	
	if (retVal){
		deobfuscateString(FAILED_MSG, strlen(FAILED_MSG));
		printf(FAILED_MSG);
		printf("\n");
		obfuscateString(FAILED_MSG, strlen(FAILED_MSG));
	}
	else {
		deobfuscateString(OK_MSG, strlen(OK_MSG));
		printf(OK_MSG);
		printf("\n");
		obfuscateString(OK_MSG, strlen(OK_MSG));
	}
	
	return;	
}