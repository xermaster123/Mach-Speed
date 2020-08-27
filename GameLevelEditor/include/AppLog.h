#pragma once
#include "imgui/imgui.h"
#include <string>

enum LogLevel
{
	Normal = 0,
	Warning = 1,
	Error = 2
};

class AppLog
{
	ImGuiTextBuffer Buf;
	ImGuiTextFilter Filter;
	ImVector<int> LineOffsets; // Index to lines offset
	ImVector<int> LineLogLevel; // Index to lines offset

	bool ScrollToBottom;

public:
	AppLog();

	void Clear();
	void AddLog(LogLevel log, std::string msg);
	//void FillBuffer(LogLevel log, const char* fmt, ...) IM_FMTARGS(2);
	void Draw(const char* title, bool* p_open = NULL);
};
