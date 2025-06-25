#include "Program.h"
#include <iostream>

using namespace std;

void Program::run() {
	cout << "Running Program..." << endl;
	while(true) {
		int choice;
		cin >> choice;
		switch(choice) {
			case 1:
				printHello();
				break;
			case 2:
				printArgument(10);
				break;
			case 3: {
				
				printStringWithArg("Hello world", 10);
				break;
			}
			case 0:
				cout << "Exiting Program." << endl;
				return;
			default:
				cout << "Invalid choice. Please try again." << endl;
		}
	}
}

void Program::printHello() const {
	cout << "Hello from Program!" << endl;
}

void Program::printArgument(int arg) const {
	cout << "Argument: " << arg << endl;
}

void Program::printStringWithArg(const char* str, int arg) const {
	cout << "String: " << str << ", Argument: " << arg << endl;
}