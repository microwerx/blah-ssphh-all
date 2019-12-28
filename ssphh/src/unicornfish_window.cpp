#include "pch.hpp"
#include <ssphhapp.hpp>
#include <unicornfish_window.hpp>

UnicornfishWindow::UnicornfishWindow(const std::string& name)
	: Vf::Window(name) {}

UnicornfishWindow::~UnicornfishWindow() {}

void UnicornfishWindow::OnUpdate(double timestamp) {
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

void UnicornfishWindow::OnRenderDearImGui() {
	if (!ssg) return;
	if (!beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	endWindow();
}
