#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>

#include <thread>
#include <iostream>
#include <chrono>
#include <string>


bool isProcessRunning1(const std::string& process_name)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe32.dwSize = sizeof(PROCESSENTRY32);
	
	while (Process32Next(hProcessSnap, &pe32))
	{
		std::cout << "process: " << pe32.szExeFile << " is running. process id: " << pe32.th32ProcessID << std::endl;
		if (std::string(pe32.szExeFile) == process_name)
		{
			std::cout << "process: " << process_name << " is running. process id: " << pe32.th32ProcessID << std::endl;
			CloseHandle(hProcessSnap);
			return true;
		}
	}

	CloseHandle(hProcessSnap);

	return false;
}

bool isProcessRunning2(const std::string& process_name)
{
	DWORD dwProcessID[1000];
	DWORD dwCBNeed;
	EnumProcesses(dwProcessID, sizeof(dwProcessID), &dwCBNeed);
	DWORD dwProcessCnt = dwCBNeed / sizeof(DWORD);
	char path[MAX_PATH];
  
	for (DWORD i = 0; i < dwProcessCnt; i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID[i]);
		ZeroMemory(path, MAX_PATH);
		
		GetProcessImageFileNameA(hProcess, path, MAX_PATH);
		printf("Process ID [ %d ] - [ %s ]\n", dwProcessID[i], path);
		
		if (strstr(path, process_name.c_str()) != NULL)
		{
			//TerminateProcess(hProcess, 0);
			return true;
		}
	}

	return false;
}

int main()
{
	int milliseconds = 2000;
	const std::string process_name = "wechat.exe";
	auto thread = std::thread([&]() {
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
			if (isProcessRunning1(process_name))
			{
				continue;
			}

			break;
		}
		
	});

	thread.join();

	return 0; 
}
