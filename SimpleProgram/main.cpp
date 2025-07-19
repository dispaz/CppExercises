#include <iostream>
#include <Windows.h>
#include "main.h"

using namespace std;

void injectionTest();
void ch8Test();
void printRunningN(int n);

int main() {
	cout << "Here would be call hook" << endl;
	ch8Test();
	std::cin.get();
	return 0;
}

void ch8Test() {
	MessageBoxW(NULL, L"MessageBoxA text", L"MessageBoxA title", MB_OK);
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
