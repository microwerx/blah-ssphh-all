#include "pch.hpp"
#include <ssphhapp.hpp>
#include <scene_graph_window.hpp>

namespace Vf
{
	void ImGuiMatrix4f(const Matrix4f& m) {
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m11, m.m12, m.m13, m.m14);
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m21, m.m22, m.m23, m.m24);
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m31, m.m32, m.m33, m.m34);
		ImGui::Text("[ % 1.3f % 1.3f % 1.3f % 3.2f ]", m.m41, m.m42, m.m43, m.m44);
	}

	void ImGuiBoundingBox(const BoundingBoxf& bbox) {
		ImGui::Text("Min:  [ % 3.2f % 3.2f % 3.2f ]", bbox.minBounds.x, bbox.minBounds.y, bbox.minBounds.z);
		ImGui::Text("Max:  [ % 3.2f % 3.2f % 3.2f ]", bbox.maxBounds.x, bbox.maxBounds.y, bbox.maxBounds.z);
		ImGui::Text("Size: [ %3.2f %3.2f %3.2f ]", bbox.Size().x, bbox.Size().y, bbox.Size().z);
	}
}

const char* makeNodeName(SimpleSceneGraphNode* n) {
	static std::ostringstream ostr;
	ostr.clear();
	ostr << n->name() << "[" << n->type() << "]: [" << n->status() << "]";
	return ostr.str().c_str();
}

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
