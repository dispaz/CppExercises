#pragma once
#include <vector>
#include <windows.h>
#include <list>

struct Player {
	int health;
	bool isAlive;
	std::vector<int> inventory;
	std::list<WORD> linkedList;
	void displayInfo() const;
	void showAddresses();
	void addItem(int itemID);
};