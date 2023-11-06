#include "WindowConsole.h"
#include "App.h"

WindowConsole::WindowConsole(string name, bool startEnabled, ImGuiWindowFlags flags) : GuiBase(name, startEnabled, flags) {

	//output will be on imgui console window
	//originalCout = std::cout.rdbuf();
	//std::cout.rdbuf(consoleOutput.rdbuf());
};

WindowConsole::~WindowConsole()
{
	//return to all console output to original console window not imgui
	//std::cout.rdbuf(originalCout);
}

void WindowConsole::Update() {
	//take text of original console


	//cout << "this text goes to window console\n";
	
	//Write on imgui window console text

	list<string> lines;
	size_t pos = 0, found;

	std::string bufferContent = app->consoleOutput.str();

	while ((found = bufferContent.find("\n", pos)) != std::string::npos) {
		lines.push_back(bufferContent.substr(pos, found - pos));
		pos = found + 1;
	}
	// Add the last line
	lines.push_back(bufferContent.substr(pos));


	//now lets print it to console window imgui
	for (const std::string& line : lines) {
		ImGui::Text(line.c_str());
	}

	//ImGui::Text(app->consoleOutput.str().c_str());


	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
	{
		ImGui::SetScrollHereY(1.0f);
	}
	//set original console as if nothing happened
	//std::cerr.rdbuf(originalCerr);
	//cout << "this text goes to console\n";
}