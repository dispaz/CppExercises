#include <iostream>
#include "GameHackingCh5.h"
#include "GameHackingCh6.h"
#include "main.h"
#include "GameHackingCh7.h"
#include "tools.h"

using namespace std;

void chapter8Work();

HANDLE retrieveSimpleProgramHandle();

int main() {
	chapter8Work();
	return 0;
}

void chapter8Work() {
	cout << "Chapter 8 work in progress..." << endl;

	DWORD targetNOPAdd = 0x00412550;

	const int SIZE = 5; // Size of the NOP instruction to write
	
	HANDLE proc = retrieveSimpleProgramHandle();
	
	if (proc == NULL) {
		cout << "Process not found!" << endl;
		return;
	}

	auto oldProtect = protectMemoryAPI<BYTE[SIZE]>(proc, targetNOPAdd, PAGE_EXECUTE_READ);
	LPVOID targetNOP = (LPVOID)targetNOPAdd;
	
	writeNOP<SIZE>(proc, targetNOPAdd);
	
	cout << "NOPs written successfully." << endl;
}

HANDLE retrieveSimpleProgramHandle()
{
	wstring moduleName = L"SimpleProgram.exe";
	HANDLE anotherProcess = getHandleByName(moduleName);

	return anotherProcess;
}

void chapter7Work() {
	//runThreads();
	wstring processName = L"SimpleProgram.exe";
	HANDLE anotherProcess = getHandleByName(processName);
	if(anotherProcess == NULL) {
		cout << "Process not found" << endl;
	}

	const wchar_t* dllName = L"D:\\VV\\Cheat Dev\\CheatDevelopmentExercises\\Debug\\SimpleInjectDLL.dll";

	loadDll(anotherProcess, dllName);
}

void chapter6Work() {
	cout << "Current process base address: " << hex << (uintptr_t)getModuleBaseAddress() << endl;

	wstring moduleName = L"SimpleProgram.exe";
	HANDLE anotherProcess = getHandleByName(moduleName);
	if (anotherProcess == NULL) {
		cout << "Process not found!" << endl;
		return;
	}
	printf("Handle to %S: %p\n", moduleName.c_str(), anotherProcess);

	cout << "Retrieving module base address..." << endl;
	DWORD remoteBaseAddress = getRemoteBaseAddress(anotherProcess);

	if (remoteBaseAddress == 0) {
		cout << "Failed to retrieve base address." << endl;
		return;
	}

	wcout << "Base address of " << moduleName << ": " << hex << uppercase << (uintptr_t)remoteBaseAddress << endl;

	cin.get();
}

void playerInfo()
{
	Player* player = new Player();
	player->health = 100;
	player->isAlive = true;
	player->inventory.push_back(1);
	player->inventory.push_back(2);
	player->linkedList.push_back(0);
	player->linkedList.push_back(1);
	player->linkedList.push_back(2);

	do {
		player->displayInfo();
		player->showAddresses();
		int choice;
		cin >> choice;
		if (choice == 0) {
			cout << "0 chosen" << endl;
		}
		else {
			player->addItem(choice);
		}
		cin.get();
	} while (true);
}
