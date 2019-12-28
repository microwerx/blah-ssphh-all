#include "pch.hpp"
#include <ssphhapp.hpp>
#include <tool_window.hpp>

ToolWindow::ToolWindow(const std::string& name)
	: Vf::Window(name) {}

ToolWindow::~ToolWindow() {}

void ToolWindow::OnUpdate(double timestamp) {
	if (!ssphh_widget_ptr) {
		ssg = nullptr;
		return;
	}
	else if (ssg != &ssphh_widget_ptr->ssg) {
		ssg = &ssphh_widget_ptr->ssg;
	}
	if (!ssg) return;
	Vf::Window::OnUpdate(timestamp);
}

void ToolWindow::OnRenderDearImGui() {
	if (!ssg) return;
	if (!beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	endWindow();
}
