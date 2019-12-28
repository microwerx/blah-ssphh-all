#include "pch.hpp"
#include <ssphhapp.hpp>
#include <scene_graph_window.hpp>

SceneGraphWindow::SceneGraphWindow(const std::string& name)
	: Vf::Window(name) {}

SceneGraphWindow::~SceneGraphWindow() {}

void SceneGraphWindow::OnUpdate(double timestamp) {
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

void SceneGraphWindow::OnRenderDearImGui() {
	if (!ssg) return;
	if (!beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	if (ImGui::TreeNode("Nodes")) {
		for (const auto& [k, n] : ssg->nodes) {
			if (n->keyword() == std::string("node")) {
				ImGui::TextColored(Colors::Azure, "%s \'%s\' [%s]: [%s]", n->keyword(), n->name(), n->type(), n->status());
				continue;
			}
			if (ImGui::TreeNode(n->name())) {
				ImGui::TextColored(Colors::Azure, "%s \'%s\' [%s]: [%s]", n->keyword(), n->name(), n->type(), n->status());
				if (ImGui::TreeNode("bbox")) {
					Vf::ImGuiBoundingBox(n->bbox);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("transform")) {
					Vf::ImGuiMatrix4f(n->transform);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("addlTransform")) {
					Vf::ImGuiMatrix4f(n->addlTransform);
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	endWindow();
}
