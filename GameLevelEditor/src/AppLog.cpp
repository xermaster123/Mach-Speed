#include "AppLog.h"
#include <iostream>

AppLog::AppLog(): ScrollToBottom(false)
{
}

void AppLog::Clear()
{
	Buf.clear();
	LineOffsets.clear();
	LineLogLevel.clear();
}

void AppLog::AddLog(LogLevel log, std::string msg)
{
	std::cout << msg << std::endl;

	msg.append("\n");
	char* fmt = new char[msg.length() + 1];
	strcpy_s(fmt, msg.length() + 1, msg.c_str());

	int old_size = Buf.size();
	//va_list args;
	//va_start(args, fmt);
	Buf.appendfv(fmt, nullptr);
	//va_end(args);

	for (int new_size = Buf.size(); old_size < new_size; old_size++)
	{
		if (Buf[old_size] == '\n')
		{
			LineOffsets.push_back(old_size);
			LineLogLevel.push_back(log);
		}
	}
	ScrollToBottom = true;
	delete[] fmt;
}

void AppLog::Draw(const char* title, bool* p_open)
{

	if (ImGui::Button("Clear")) Clear();
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (copy) ImGui::LogToClipboard();

	const char* buf_begin = Buf.begin();
	const char* line = buf_begin;

	for (int line_no = 0; line != NULL; line_no++)
	{
		ImVec4 logColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

		if (LineLogLevel.size() != 0)
		{
			if (LineLogLevel[line_no] == LogLevel::Warning)
			{
				logColour = ImVec4(1.0f, 0.65f, 0.0f, 1.0f);
			}
			else if (LineLogLevel[line_no] == LogLevel::Error)
			{
				logColour = ImVec4(1.0f, 0.f, 0.0f, 1.0f);
			}
			else
			{
				logColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		ImGui::PushStyleColor(ImGuiCol_Text, logColour);

		const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;

		if (Filter.IsActive())
		{
			if (Filter.PassFilter(line, line_end))
			{
				ImGui::TextUnformatted(line, line_end);
			}

		}
		else
		{
			ImGui::TextUnformatted(line, line_end);
			//ImGui::TextUnformatted(Buf.begin());
		}

		line = line_end && line_end[1] ? line_end + 1 : NULL;
		ImGui::PopStyleColor();
	}

	if (ScrollToBottom)
	{
		ImGui::SetScrollHere(0.0f);

		//if (ImGui::GetScrollY() == ImGui::GetScrollMaxY())
		//{
			ScrollToBottom = false;
		//}
	}

	ImGui::EndChild();
}
