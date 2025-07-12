#include <iostream>
#include <Windows.h>
#include "main.h"

using namespace std;

void injectionTest();
void ch8Test();
void printRunningN(int n);

int main() {
	ch8Test();
	std::cin.get();
	return 0;
}

void ch8Test() {
	cout << "####### Chapter 8 Test #######" << endl;
	cout << "Func addr: " << &printRunningN << endl; // 007E1483
	cout << "Sleep() addr: " << &Sleep << endl; // 774AD720
	cin.get();
	cout << "Running printRunningN(5)." << endl;
	printRunningN(5);
}

void injectionTest()
{
	while (true) {
		cout << "running ..." << endl;
		Sleep(1000);
	}
}

void printRunningN(int n) {
	while (n > 0) {
		cout << "Running: " << n << endl;
		Sleep(1000);
		n--;
	}
}
