#pragma once

class IChapterRunner {
public:
	virtual void run() = 0;
	virtual ~IChapterRunner() = default;
};