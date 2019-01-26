#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

using namespace std;

VOID GetProcessInfo(DWORD CONST dwProcessID) {

	HMODULE hMods[1024];
	DWORD cbNeeded;

	HANDLE CONST hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (hProcess == NULL) {
		cout << "OpenProcess --- " << "LastErrorCode: " << GetLastError() << endl;
		return;
	}
	else {
		cout << "HOREY" << endl;
	}
	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
		for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
				cout << szModName << "   " << hMods[i] << endl;
			}
		}
	}
	else {
		cout  << "EnumProcessModules --- " << "LastErrorCode: " << GetLastError() << endl;
		return;
	}
}

VOID PrintModuleList(HANDLE CONST hStdOut, DWORD CONST dwProcessID) {
	MODULEENTRY32 meModuleEntry;
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		cout << "LastErrorCode: " << GetLastError() << endl;
		return;
	}
	meModuleEntry.dwSize = sizeof(MODULEENTRY32);
	Module32First(hSnapshot, &meModuleEntry);
	cout << meModuleEntry.szModule << endl;
	CloseHandle(hSnapshot);
}

VOID PrintProcessList(HANDLE CONST hStdOut) {
	PROCESSENTRY32 peProcessEntry;
	HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return;
	}
	peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &peProcessEntry);
	cout << peProcessEntry.szExeFile << endl;
	int i = 0;
	DWORD check;
	do {
		cout << i << "   " << peProcessEntry.th32ProcessID << "  " << peProcessEntry.szExeFile << endl;
		if (i == 40) { 
			check = peProcessEntry.th32ProcessID;
		}
		i++;
	} while (Process32Next(hSnapshot, &peProcessEntry));

//	DWORD choosingPID;
//	cout << "Choose PID" << endl;
//	cin >> choosingPID;
	//PrintModuleList(hStdOut, choosingPID);
	//PrintModuleList(hStdOut, check);
	cout << "PID: " << check << endl;
	GetProcessInfo(check);
	CloseHandle(hSnapshot);
}

int main() {
	HANDLE CONST hStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	PrintProcessList(hStdHandle);
	system("pause");
	return 0;
}
