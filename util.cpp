#include "windows.h"
#include "util.h"

bool isNumeric(std::string& str)
{
	for(unsigned int i=0; i < str.length(); i++) {
		if (isdigit(str.at(i)) == 0)
			return false;
	}
	return true;
}

bool WinReadFile(std::string& path, std::string& result)
{
	char buffer[10000];
	DWORD bytesRead;
	HANDLE fileHandle;

	fileHandle = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE)
		return false;
	for(;;)
	{
		if (!ReadFile(fileHandle, &buffer, sizeof(buffer), &bytesRead, NULL)) {
			CloseHandle(fileHandle);
			return false;
		}
		for(unsigned int i=0; i < bytesRead; i++) { //std::string::append does not work because it treats the buffer as C string with a NULL terminator
			result += buffer[i];
		}
		if (bytesRead < sizeof(buffer)) {
			CloseHandle(fileHandle);
			return true;
		}
	}
	CloseHandle(fileHandle);
	return false; //this should never happen 
}