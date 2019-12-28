#include "pch.hpp"
#include <ssphhapp.hpp>
#include <ssphh_window.hpp>

SsphhWindow::SsphhWindow(const std::string& name)
	: Vf::Window(name) {}

SsphhWindow::~SsphhWindow() {}

void SsphhWindow::OnUpdate(double timestamp) {
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

void SsphhWindow::OnRenderDearImGui() {
	if (!ssg) return;
	if (!beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	endWindow();
}
