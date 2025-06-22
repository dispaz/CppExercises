// MemoryExercise.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>
#include "TestClassMemory.cpp"

using namespace std;

//int main() {
//	TestClass testObj = TestClass();
//	int value = 0;
//	do {
//		cout << "Process ID: " << GetCurrentProcessId() << endl;
//		cout << "testObj: 0x" << hex << uppercase << (uintptr_t)&testObj << endl;
//		testObj.printStatus();
//		cout << "Enter value to modify health: " << endl;
//		cin >> value;
//		cout << "Modifying health by " << value << endl;
//		testObj.modifyHealth(-value);
//
//		cout << "-----------------------------" << endl;
//	} while (true);
//
//	return 0;
//}

int main()
{
    int varInt = 12345;
    string varStr = "def str";
	char varChar[128] = "Long char array right there ->";
	int* ptr2Int = &varInt;
	
	// a pointer to pointer to int named ptr2ptr pointing to ptr2int
	int** ptr2ptr(&ptr2Int);
	int*** ptr2ptr2ptr(&ptr2ptr);
	
	do {
		cout << "Process ID: " << GetCurrentProcessId() << endl;
		cout << "varInt:		0x" << hex << uppercase << (uintptr_t)&varInt << " = " << dec << varInt << endl;
		cout << "varStr:		0x" << hex << uppercase << (uintptr_t)&varStr << " = \"" << varStr << "\"" << endl;
		cout << "varChar:		0x" << hex << uppercase << (uintptr_t)&varChar << " = \"" << varChar << "\"" << endl;
		cout << "ptr2Int:		0x" << hex << uppercase << (uintptr_t)&ptr2Int << " = 0x" << ptr2Int << endl;
		cout << "ptr2ptr:		0x" << hex << uppercase << (uintptr_t)&ptr2ptr << " = 0x" << ptr2ptr << endl;
		cout << "ptr2ptr2ptr:	0x" << hex << uppercase << (uintptr_t)&ptr2ptr2ptr  << " = 0x" <<ptr2ptr2ptr << endl;

		cout << "Press Enter to continue..." << endl;
		cin.get(); // Wait for user input to continue
		cout << "--------------------------------------------------------" << endl;

	} while (true);
	return 0;
}

