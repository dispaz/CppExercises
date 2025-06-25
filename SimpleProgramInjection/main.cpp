#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include "injection.h"
using namespace std;

void testFunction() {
	int counter = 0;
	while(counter < 100) {
		cout << "Counter: " << counter << endl;
		counter++;
		Sleep(100); // Sleep for 1 second
	}
}

int main() {
	HANDLE hProcess = GetCurrentProcess();
	if (hProcess == NULL) {
		cout << "Failed to get handle for process with PID: " << endl;
		return 1;
	}
	cout << "Opened process: " << hProcess << endl;
	Injector injector = Injector();
	injector.injectCode(hProcess, (LPVOID)&testFunction);
	while (true) {
		cout << "Main thread is running..." << endl;
		Sleep(100);
	}
	cin.get();
	return 0;
}