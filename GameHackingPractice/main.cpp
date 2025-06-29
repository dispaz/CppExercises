#include <iostream>
#include "GameHackingCh5.h"
#include "main.h"
#include "GameHackingCh6.h"

using namespace std;

int main() {
	cout << "Current process base address: " << hex << (uintptr_t)getModuleBaseAddress() << endl;

	wstring moduleName = L"SimpleProgram.exe";
	HANDLE anotherProcess = getHandleByName(moduleName);
	if (anotherProcess == NULL) {
		cout << "Process not found!" << endl;
		return 1;
	}
	printf("Handle to %S: %p\n", moduleName.c_str(), anotherProcess);

	cout << "Retrieving module base address..." << endl;
	DWORD remoteBaseAddress = getRemoteBaseAddress(anotherProcess);

	if (remoteBaseAddress == 0) {
		cout << "Failed to retrieve base address." << endl;
		return 1;
	}

	wcout << "Base address of " << moduleName << ": " << hex << uppercase << (uintptr_t)remoteBaseAddress << endl;

	cin.get();
	return 0;
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
