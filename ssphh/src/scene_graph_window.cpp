#include "pch.hpp"
#include <scene_graph_window.hpp>
#include <ssphhapp.hpp>

SceneGraphWindow::SceneGraphWindow(const std::string& name) : Vf::Window(name) {}


SceneGraphWindow::~SceneGraphWindow() {}


void SceneGraphWindow::OnUpdate(double timestamp) {
	if (!ssphh_widget_ptr) {
		ssg = nullptr;
		return;
	} else if (ssg != ssphh_widget_ptr->ssg) {
		ssg = ssphh_widget_ptr->ssg;
	}
	if (!ssg)
		return;
	Vf::Window::OnUpdate(timestamp);
}


void SceneGraphWindow::OnRenderDearImGui() {
	if (!ssg)
		return;
	if (!beginWindow())
		return;
	Vf::Window::OnRenderDearImGui();

	if (ImGui::TreeNode("Nodes")) {
		for (const auto& [k, n] : ssg->nodes) {
			if (n->keyword() == std::string("node")) {
				ImGui::TextColored(
					Colors::Azure,
					"%d %s \'%s\' [%s]: [%s]",
					n->instance(),
					n->keyword(),
					n->name(),
					n->type(),
					n->status());
				continue;
			}
			if (ImGui::TreeNode(n->name())) {
				ImGui::TextColored(
					Colors::Azure,
					"%d %s \'%s\' [%s]: [%s]",
					n->instance(),
					n->keyword(),
					n->name(),
					n->type(),
					n->status());
				if (ImGui::SmallButton("load")) {
					n->flipbit(Fluxions::IBaseObject::BIT_LOADED);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("usable")) {
					n->flipbit(Fluxions::IBaseObject::BIT_USABLE);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("renderable")) {
					n->flipbit(Fluxions::IBaseObject::BIT_RENDERABLE);
				}
				ImGui::SameLine();
				ImGui::Text("%s", n->bitstring());
				
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

	// if (ImGui::TreeNode("Geometry")) {
	//	ImGui::TreePop();
	//}

	if (ImGui::TreeNode("Materials")) {
		for (auto& mtl : ssg->materials.mtls) {
			if (ImGui::TreeNode(mtl->name())) {
				ImGui::ColorEdit4("Kd", mtl->base.Kd.ptr());
				ImGui::ColorEdit4("Ks", mtl->base.Ks.ptr());
				ImGui::ColorEdit4("Ke", mtl->base.Ke.ptr());
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Maps")) {
		for (auto& map : ssg->materials.maps) {
			ImGui::Text("%s", map.first.c_str());
		}
		ImGui::TreePop();
	}


	endWindow();
}
