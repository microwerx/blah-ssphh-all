#include "pch.hpp"
#include <ssphhapp.hpp>
#include <scene_editor_window.hpp>

SceneEditorWindow::SceneEditorWindow(const std::string& name)
	: Vf::Window(name) {}

SceneEditorWindow::~SceneEditorWindow() {}

void SceneEditorWindow::OnUpdate(double timestamp) {
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

void SceneEditorWindow::OnRenderDearImGui() {
	if (!ssg) return;
	if (!beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	endWindow();
}
