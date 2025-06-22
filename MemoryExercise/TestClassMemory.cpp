#include <iostream>

using namespace std;


class TestClass {
private:
	int health = 100;
	bool isAlive = true;
public:

	TestClass() {
		health = 100;
		isAlive = true;
	}

	void printStatus() {
		cout << "Health:	0x" << hex << uppercase << (uintptr_t)&health << " = " << dec << health << endl;
		cout << "Alive:		0x" << hex << uppercase << (uintptr_t)&isAlive << " = " << (isAlive ? "Yes" : "No") << endl;
	}

	void modifyHealth(int amount) {
		health += amount;
		if (health <= 0) {
			isAlive = false;
		}
	}

	void modifyAliveStatus(bool status) {
		isAlive = status;
		if (!isAlive) {
			health = 0;
		}
	}
};