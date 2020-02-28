#include "pch.hpp"
#include <ssphhapp.hpp>
#include <help_window.hpp>

HelpWindow::HelpWindow(const std::string& name)
	: Vf::Window(name) {}

HelpWindow::~HelpWindow() {}

void HelpWindow::OnInit(const std::vector<std::string>& args) {
	Vf::Window::OnInit(args);
	constexpr int H1 = 0;
	constexpr int H2 = 1;
	constexpr int P = 2;
	lines.push_back({ H1, "ESC", "Quit Program" });
	lines.push_back({ H1, "F1", "Help Window" });
	lines.push_back({ P,    "", "Displays a summary of help for hot keys used in this program"});
	lines.push_back({ H1, "F2", "Statistics Window" });
	lines.push_back({ H1, "F3", "Scene Graph Window" });
	lines.push_back({ H1, "F4", "Scene Editor Window" });
	lines.push_back({ H1, "F5", "SSPHH Window" });
	lines.push_back({ H1, "F6", "Animation Path Window" });
	lines.push_back({ H1, "F7", "Animation Window" });
	lines.push_back({ H1, "F8", "Renderer Config Window" });
	lines.push_back({ H1, "F9", "Renderer Window" });
	lines.push_back({ H1, "F10","Menu" });
	lines.push_back({ H1, "F11","Tool Window" });
	lines.push_back({ H1, "F12","Unicornfish Window" });
}

void HelpWindow::OnRenderDearImGui() {
	if (!beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	static ImVec4 colors[4] = {
		Colors::Azure,
		Colors::ForestGreen,
		Colors::Gray67,
		Colors::White
	};

	auto ts = ImGui::CalcTextSize("ABCD");
	float gap = ts.x;

	for (auto& [heading, key, desc] : lines) {
		ImGui::PushStyleColor(ImGuiCol_Text, colors[heading]);
		ImGui::Text("%s", key.c_str());
		ImGui::SameLine(gap);
		ImGui::TextWrapped("%s", desc.c_str());
		ImGui::PopStyleColor();
	}

	endWindow();
}
