#include <iostream>
#include <Windows.h>
#include <string>

using std::string;

const int BUFFER_SIZE = 128; // Define a buffer size for reading memory

void readValuesFromOtherProcess();
void writeValuesToOtherProcess();
int main() {
	writeValuesToOtherProcess();
	return 0;
}

void readValuesFromOtherProcess() {
	const int targetProcessId = 32120;
	const LPCVOID varAddr = (LPCVOID)0xB1C28FFC98;
	HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, targetProcessId);
	if (hProcess == NULL) {
		std::cerr << "Failed to open process with ID " << targetProcessId << ". Error: " << GetLastError() << std::endl;
		return;
	}
	std::cout << "Successfully opened process with ID " << targetProcessId << "." << std::endl;

	std::cout << "-----------------------------------------" << std::endl;
	intptr_t readInt = 0;
	do {
		if (!ReadProcessMemory(hProcess, (LPCVOID)varAddr, &readInt, sizeof(readInt), NULL)) {
			std::cerr << "Failed to read memory at address " << varAddr << ". Error: " << GetLastError() << std::endl;
			CloseHandle(hProcess);
			return;
		}
		std::cout << "Read integer pointer: 0x" << std::hex << std::uppercase << readInt << std::endl;

		int valuePtr;
		if(!ReadProcessMemory(hProcess, (LPCVOID)readInt, &valuePtr, sizeof(valuePtr), NULL)) {
			std::cerr << "Failed to read integer value at address " << valuePtr << ". Error: " << GetLastError() << std::endl;
			CloseHandle(hProcess);
			return;
		}
		std::cout << "Read integer value: " << std::dec << valuePtr << std::endl;
		std::cin.get();
	} while (true);
}

void readObjectFromOtherProcess() {
	const int targetProcessId = 25316;
	const LPCVOID varAddr = (LPCVOID)0x7A508FF7F8;
	do {

		std::cin.get();
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
		if(hProcess == NULL) {
			std::cerr << "Failed to open process with ID " << targetProcessId << ". Error: " << GetLastError() << std::endl;
			return;
		}
		LPCVOID testObj;
		std::cout << "Successfully opened process with ID " << targetProcessId << "." << std::endl;
		if(ReadProcessMemory(hProcess, varAddr, &testObj, sizeof(varAddr), NULL)) {
			std::cout << "Successfully read memory at address " << varAddr << "." << std::endl;
		} else {
			std::cerr << "Failed to read memory at address " << varAddr << ". Error: " << GetLastError() << std::endl;
			CloseHandle(hProcess);
			return;
		}
	} while (true);
}

void writeValuesToOtherProcess() {
	int targetProcessId = 1576;
	/*std::cout << "Write PID: " << std::endl;
	std::cin >> targetProcessId;*/
	HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_VM_OPERATION, FALSE, targetProcessId);
	if (hProcess == NULL) {
		std::cerr << "Failed to open process with ID " << targetProcessId << ". Error: " << GetLastError() << std::endl;
		return;
	}
	std::cout << "Successfully opened process with ID " << targetProcessId << "." << std::endl;

	//std::cout << "Enter Address to write to: " << std::endl;
	intptr_t memoryAddress = 0x72510FF650;
	/*std::cin >> std::hex >> memoryAddress;*/

	do {
		std::cout << "Reading memory of PID: " << targetProcessId << " at address: 0x";
		std::cout << std::hex << memoryAddress << std::endl;

		char buffer[BUFFER_SIZE];
		BOOL rpmResult = ReadProcessMemory(hProcess, (LPCVOID)memoryAddress, &buffer, sizeof(buffer), NULL);
		if (!rpmResult) {
			std::cerr << "Failed to read memory at address 0x" << std::hex << memoryAddress << ". Error: " << GetLastError() << std::endl;
			CloseHandle(hProcess);
			return;
		}
		std::cout << "Read memory: " << buffer << std::endl;
		std::cout << "Enter value to write (or 'exit' to quit): \n";
		string inputValue;
		std::getline(std::cin, inputValue);
		if (inputValue == "exit") {
			break;
		}
		if (inputValue.length() >= BUFFER_SIZE) {
			inputValue = inputValue.erase(BUFFER_SIZE);
			std::cout << "Input value too long, truncating to: \n" << inputValue << std::endl;
		}
		char bufferToWrite[BUFFER_SIZE] = {};

		strncpy_s(bufferToWrite, inputValue.c_str(), sizeof(bufferToWrite));
		std::cout << bufferToWrite << std::endl;
		BOOL wpmResult = WriteProcessMemory(hProcess, (LPVOID)memoryAddress, &bufferToWrite, sizeof(bufferToWrite), NULL);
		if(!wpmResult) {
			std::cerr << "Failed to write memory at address 0x" << std::hex << memoryAddress << ". Error: " << GetLastError() << std::endl;
			CloseHandle(hProcess);
			return;
		}
	} while (true);
	CloseHandle(hProcess);
}