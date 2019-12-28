#include "pch.hpp"
#include <ssphhapp.hpp>
#include <ssphh_menu.hpp>

SsphhMenu::SsphhMenu(const std::string& name)
	: Vf::Window(name) {}

SsphhMenu::~SsphhMenu() {}

void SsphhMenu::OnUpdate(double timestamp) {
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

void SsphhMenu::OnRenderDearImGui() {
	if (!ssg) return;
	if (!beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	endWindow();
}
