#pragma once

class Program {
public:
	Program() = default;
	void run();
	void printHello() const;
	void printArgument(int arg) const;
	void printStringWithArg(const char* str, int arg) const;
};