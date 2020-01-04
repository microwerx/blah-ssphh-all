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

	ImGui::TextColored(Colors::Cyan, "POST");
	ImGui::SliderFloat("Simple Exposure", &ssg->environment.toneMap.x, -12.0f, 12.0f);
	ImGui::SliderFloat("Gamma Compression", &ssg->environment.toneMap.y, 0.0f, 2.2f);
	ImGui::SliderFloat("Filmic Highlights", &ssg->environment.toneMap.z, 0.0f, 1.0f);
	ImGui::SliderFloat("Filmic Shadows", &ssg->environment.toneMap.w, 0.0f, 1.0f);
	ImGui::SliderFloat("Fade Image", &ssg->environment.CompareBlend.r, 0.0f, 1.0f);
	ImGui::SliderFloat("Fade Color", &ssg->environment.CompareBlend.g, 0.0f, 1.0f);
	ImGui::SliderFloat("Fade Reference", &ssg->environment.CompareBlend.b, 0.0f, 1.0f);
	ImGui::SliderFloat("Compare Reference", &ssg->environment.CompareBlend.a, 0.0f, 1.0f);
	ImGui::ColorEdit3("Fade Color", ssg->environment.FadeColor.ptr());
	ImGui::SliderFloat("Fade Dissolve", &ssg->environment.FadeColor.a, 0.0f, 1.0f);

	endWindow();
}
