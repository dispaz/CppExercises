#include <vector>
#include <iostream>
#include <windows.h>
#include "GameHackingCh5.h"

using namespace std;


void Player::displayInfo() const {
	printf("Health: %d\n", health);
	printf("Alive: %s\n", isAlive ? "Yes" : "No");
	printf("Inventory: ");
	for (int item : inventory) {
		printf("%d ", item);
	}
	printf("\nLinked List: ");
	for (const auto& node : linkedList) {
		printf("%d ", node);
	}
	printf("\n");
}

void Player::showAddresses() {
	cout << "Player object address: " << hex << uppercase << (uintptr_t)this << endl;
	cout << "Health address: " << hex << uppercase << (uintptr_t) & (this->health) << endl;
	cout << "IsAlive address: " << hex << uppercase << (uintptr_t) & (this->isAlive) << endl;
	cout << "Inventory address: " << hex << uppercase << (uintptr_t) & (this->inventory) << endl;
	cout << "Linked list address: " << hex << uppercase << (uintptr_t) & (this->linkedList) << endl;
}

void Player::addItem(int item) {
	this->inventory.push_back(item);
}