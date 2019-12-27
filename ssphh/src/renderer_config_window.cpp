#include "pch.hpp"
#include <renderer_config_window.hpp>
#include <ssphhapp.hpp>

RendererConfigWindow::RendererConfigWindow(const std::string& name)
	: Vf::Window(name) {}

RendererConfigWindow::~RendererConfigWindow() {}

void RendererConfigWindow::OnUpdate(double timestamp) {
	if (!ssphh_widget_ptr) {
		context = nullptr;
		return;
	}

	Vf::Window::OnUpdate(timestamp);

	if (context != &ssphh_widget_ptr->rendererContext)
		context = &ssphh_widget_ptr->rendererContext;

	if (!context) return;

	if (renderConfigList.size() != context->rendererConfigs.size()) {
		rc = nullptr;
		renderConfigList.clear();
	}
	renderConfigList.resize(context->rendererConfigs.size());
	int i = 0;
	for (const auto& [k, v] : context->rendererConfigs) {
		renderConfigList[i++] = v.name();
	}
	if (curRendererConfigIndex >= renderConfigList.size()) {
		rc = nullptr;
		curRendererConfigIndex = 0;
	}
	else {
		rc = context->getRendererConfig(renderConfigList[curRendererConfigIndex]);
	}
}

void RendererConfigWindow::OnRenderDearImGui() {
	if (!context) return;
	if (renderConfigList.size() != context->rendererConfigs.size()) {
		return;
	}
	if (!beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	if (defaultParameterWidth == 100.0f) {
		auto r = ImGui::CalcTextSize("GL_TEXTURE_CUBE_MAP_POSITIVE_X");
		defaultParameterWidth = r.x;
	}

	ImGui::ListBox("Renderer Configs",
				   &curRendererConfigIndex,
				   renderConfigList.data(),
				   (int)renderConfigList.size(), 8);

	if (!rc) { endWindow(); return; }
	if (rc && !rc->parent()) {
		ImGui::TextColored(Colors::Red, "RC '%s' is invalid!", rc->name());
		endWindow(); return;
	}
	if (!rc->rc_program_ptr) {
		ImGui::TextColored(Colors::Red, "RC '%s' rc_program_ptr is invalid!", rc->name());
		endWindow(); return;
	}

	// FBO information ///////////////////////////////////////////////////

	ImGui::TextColored(Colors::White, "RC '%s' [%s]", rc->name(), rc->parent()->name());
	if (ImGui::TreeNode("writefbos")) {
		for (const auto& [k, v] : rc->writeFBOs) {
			ImGui::Text("writefbo: %s %s", (v ? v->name() : "NULL"), (v ? v->status() : "no status"));
			if (v) {
				for (const auto& fbo : v->renderTargets) {
					ImGui::Text("target: %s/%s/%s",
								Fluxions::glNameTranslator.getString(fbo.first),
								Fluxions::glNameTranslator.getString(fbo.second.attachment),
								Fluxions::glNameTranslator.getString(fbo.second.target));
				}
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("readfbos")) {
		for (const auto& [k, v] : rc->readFBOs) {
			ImGui::Text("readfbo: %s %s", (v ? v->name() : "NULL"), (v ? v->status() : "no status"));
			if (v) {
				for (const auto& fbo : v->renderTargets) {
					ImGui::Text("target: %s/%s/%s",
								Fluxions::glNameTranslator.getString(fbo.second.target),
								Fluxions::glNameTranslator.getString(fbo.first),
								fbo.second.mapName.c_str());
				}
			}
		}
		ImGui::TreePop();
	}

	// PROGRAM information ///////////////////////////////////////////////
	ImGui::Text("Program %s", rc->rc_program_ptr->name());
	if (!rc->rc_program_ptr->isLinked()) {
		ImGui::TextColored(Colors::Red, "RC '%s' program is not linked!", rc->name());
	}
	if (rc->rc_program_ptr->getInfoLog().size()) {
		ImGui::TextColored(Colors::Yellow, rc->rc_program_ptr->getInfoLog().c_str());
	}
	if (ImGui::TreeNode("Attached Shaders")) {
		for (const auto& v : rc->rc_program_ptr->attachedShaders) {
			if (!v) continue;
			ImGui::Text("%s %s %s",
						Fluxions::glNameTranslator.getString(v->shaderType),
						v->name(),
						v->status());
			if (v->infoLog.size()) {
				ImGui::TextColored(Colors::Yellow, v->infoLog.c_str());
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Active Attributes")) {
		for (const auto& [k, v] : rc->rc_program_ptr->activeAttributes) {
			ImGui::Text("%02i %s", v.index, v.GetNameOfType());
			ImGui::SameLine(defaultParameterWidth);
			ImGui::Text("%s", k.c_str());
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Active Uniforms")) {
		for (const auto& [k, v] : rc->rc_program_ptr->activeUniforms) {
			auto r = ImGui::CalcTextSize("GL_TEXTURE_CUBE_MAP_POSITIVE_X");
			ImGui::SetNextItemWidth(r.x);
			ImGui::Text("%02i %s", v.index, v.GetNameOfType());
			ImGui::SameLine(defaultParameterWidth);
			ImGui::Text("%s", k.c_str());
		}
		ImGui::TreePop();
	}

	if (rc->renderPost) {
		ImGui::SliderFloat("Exposure", &rc->renderPostToneMapExposure, -12.0f, 12.0f);
		ImGui::SliderFloat("Gamma", &rc->renderPostToneMapGamma, 0.0f, 2.2f);
		ImGui::SliderFloat("Filmic Highlights", &rc->renderPostFilmicHighlights, 0.0f, 1.0f);
		ImGui::SliderFloat("Filmic Shadows", &rc->renderPostFilmicShadows, 0.0f, 1.0f);
	}

	if (ImGui::TreeNode("Metrics")) {
		ImGui::Value("Render Time: ", rc->metrics_total_ms);
		ImGui::Value("Apply Time: ", rc->metrics_apply_ms);
		ImGui::Value("Skybox Time: ", rc->metrics_skybox_ms);
		ImGui::Value("Scene Graph Time: ", rc->metrics_scene_ms);
		ImGui::Value("VIZ Time: ", rc->metrics_viz_ms);
		ImGui::Value("Post Time: ", rc->metrics_posttime_ms);
		ImGui::TreePop();
	}

	endWindow();
}
