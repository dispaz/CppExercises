#include <iostream>
#include "GameHackingCh5.h"
#include "main.h"
#include "GameHackingCh6.cpp"

using namespace std;

int main() {
	temp();
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
