#include <iostream>
#include <Windows.h>

using namespace std;

int main() {
	while (true) {
		cout << "running ..." << endl;
		Sleep(1000);
	}
	std::cin.get();
	return 0;
}