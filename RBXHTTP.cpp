#include "windows.h"
#include "RBXHTTP.h"
#include "wininet.h"
#include <algorithm>
#include "ContentProvider.h"
#include "util.h"

_HttpGet HttpGet = (_HttpGet)0;
void* PrintObj[2]; //shared_ptr crap

bool parseHTTPArgument(std::string& path, std::string arg, std::string& output)
{
	enum {searchforArgs, searchForArgs2, parseArgName, doneParse, parseArgValue} state = searchforArgs;
	unsigned int charCount = 0;
	unsigned int savedPos = 0;
	for(unsigned int i=0; i < path.length(); i++)
	{
		char currChar = path.at(i);

		//funny state machine
		switch(state)
		{
			case searchforArgs:
				if (currChar == '?') {
					charCount = 0;
					savedPos = i+1;
					state = parseArgName;
				}
				break;
			case searchForArgs2:
				if (currChar == '&') {
					charCount = 0;
					savedPos = i+1;
					state = parseArgName;
				}
				break;
			case parseArgName:
				if (currChar == '=') {
					if (path.substr(savedPos, charCount).compare(arg) == 0){
						charCount = 0;
						savedPos = i+1;
						state = parseArgValue;
					}else{
						state = searchForArgs2;
					}
					break;
				}
				charCount++;
				break;
			case parseArgValue:
				if (currChar == '&') {
					state = doneParse;
					break;
				}else if (i == path.length()-1) { //dumb end of string workaround
					charCount++;
					state = doneParse;
					break;
				}
				charCount++;
				break;
		}
		if (state == doneParse) {
			output = path.substr(savedPos, charCount);
			return true;
			break;
		}
	}
	return false;
}

void __fastcall HttpGetHook(HttpClass *AThis, void* EDX, void* a1, void* a2, void* a3, void* a4, std::string& result) //0x008624B0
{
	URL_COMPONENTSA crackedUrl;
	memset(&crackedUrl, 0x0, sizeof(URL_COMPONENTSA));
	crackedUrl.dwStructSize = sizeof(URL_COMPONENTSA);
	char HostName[30]; crackedUrl.lpszHostName = HostName; crackedUrl.dwHostNameLength = sizeof(HostName);
	char UrlPath[256]; crackedUrl.lpszUrlPath = UrlPath; crackedUrl.dwUrlPathLength = sizeof(UrlPath);
	std::string stdHostName, stdUrlPath, idValue, fileRead;
	std::string contentFolder = getContentFolder();

	RBXPrintObj(&PrintObj);
	std::string URLc = AThis->url;

	if(!InternetCrackUrlA(URLc.c_str(), URLc.size(), NULL, &crackedUrl)){
		RBXPrint(PrintObj[0], 2, "InternetCrackUrlA() failed!");
		goto endOfFunc;
	}
	//Convert the cracked components into std::string
	stdHostName = HostName;
	stdUrlPath = UrlPath;
	
	//Convert the hostname and path to lowercase
	std::transform(stdHostName.begin(), stdHostName.end(), stdHostName.begin(), ::tolower);
	std::transform(stdUrlPath.begin(), stdUrlPath.end(), stdUrlPath.begin(), ::tolower);

	if (stdHostName.compare("roblox.com") == 0 || stdHostName.compare("www.roblox.com") == 0) {
		//check for /asset and /asset/ (ugly but faster than two compares for /asset and /asset/)
		if (stdUrlPath.compare(0, sizeof("/asset")-1,"/asset") == 0
			&& (stdUrlPath.at(sizeof("/asset")-1) == '?' || (stdUrlPath.at(sizeof("/asset")-1) == '/' && stdUrlPath.at(sizeof("/asset")) == '?')))
		{
			if (parseHTTPArgument(stdUrlPath, "id", idValue)) {
				if(isNumeric(idValue) && WinReadFile(getContentFolder() + "\\localassets\\" + idValue, fileRead)) {
					result = fileRead;
					return;
				}
			}
			URLc = "https://assetdelivery.roblox.com/v1" + stdUrlPath;
		}
		//studio.ashx
		if (stdUrlPath.compare("/game/studio.ashx") == 0) {
			if (WinReadFile(getContentFolder() + "\\localstatic\\studio.ashx", fileRead)){
				result = fileRead;
				return;
			}else{
				RBXPrint(PrintObj[0], 2, "Studio.ashx is missing from localstatic folder, corescripts will not function.");
			}
		}
		//visit.ashx
		if (stdUrlPath.compare("/game/visit.ashx") == 0) {
			if (WinReadFile(getContentFolder() + "\\localstatic\\visit.ashx", fileRead)){
				result = fileRead;
				return;
			}
		}
		//getscriptstate.ashx
		if (stdUrlPath.compare(0, sizeof("//asset/getscriptstate.ashx")-1, "//asset/getscriptstate.ashx") == 0) {
			result = "0 0 0 0";
			return;
		}
		//keepalivepinger.ashx
		if (stdUrlPath.compare("/game/keepalivepinger.ashx") == 0) {
			result = "8";
			return;
		}
	}
	AThis->url = URLc;
endOfFunc:
	HttpGet(AThis, a1, a2, a3, a4, result);
} 