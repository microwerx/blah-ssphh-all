#include <ssphhapp.hpp>
#include <hatchetfish.hpp>
#include <hatchetfish_stopwatch.hpp>
#include <ssphhapp_renderconfigs.hpp>

namespace Colors
{
	const ImVec4 Black{ 0.000f, 0.000f, 0.000f, 1.000f };
	const ImVec4 Gray33{ 0.333f, 0.333f, 0.333f, 1.000f };
	const ImVec4 Gray67{ 0.667f, 0.667f, 0.667f, 1.000f };
	const ImVec4 White{ 1.000f, 1.000f, 1.000f, 1.000f };
	const ImVec4 Red{ 1.000f, 0.000f, 0.000f, 1.000f };
	const ImVec4 Orange{ 0.894f, 0.447f, 0.000f, 1.000f };
	const ImVec4 Yellow{ 0.894f, 0.894f, 0.000f, 1.000f };
	const ImVec4 Green{ 0.000f, 1.000f, 0.000f, 1.000f };
	const ImVec4 Cyan{ 0.000f, 0.707f, 0.707f, 1.000f };
	const ImVec4 Azure{ 0.000f, 0.447f, 0.894f, 1.000f };
	const ImVec4 Blue{ 0.000f, 0.000f, 1.000f, 1.000f };
	const ImVec4 Violet{ 0.447f, 0.000f, 0.894f, 1.000f };
	const ImVec4 Rose{ 0.894f, 0.000f, 0.447f, 1.000f };
	const ImVec4 Brown{ 0.500f, 0.250f, 0.000f, 1.000f };
	const ImVec4 Gold{ 0.830f, 0.670f, 0.220f, 1.000f };
	const ImVec4 ForestGreen{ 0.250f, 0.500f, 0.250f, 1.000f };
}

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
		ImGui::Value("Skybox Time: ", rc->metrics_skybox_ms);
		ImGui::Value("Scene Graph Time: ", rc->metrics_scene_ms);
		ImGui::Value("Post Time: ", rc->metrics_posttime_ms);
		ImGui::TreePop();
	}

	endWindow();
}

namespace SSPHH
{
	void SSPHH_Application::InitRenderConfigs() {
		HFLOGINFO("Initializing render configs");

		//TODO: Set these up in the renderconfig file

		//RendererConfig& rectShadowRenderConfig = rendererContext.rendererConfigs["rectShadow"];
		//rectShadowRenderConfig.clearColorBuffer = true;
		//rectShadowRenderConfig.clearDepthBuffer = true;
		//rectShadowRenderConfig.viewportRect.x = 0;
		//rectShadowRenderConfig.viewportRect.y = 0;
		//rectShadowRenderConfig.viewportRect.w = Interface.renderconfig.sunShadowMapSize;
		//rectShadowRenderConfig.viewportRect.h = Interface.renderconfig.sunShadowMapSize;
		//rectShadowRenderConfig.fov = 90.0;
		//rectShadowRenderConfig.isCubeMap = false;
		//rectShadowRenderConfig.useSceneCamera = false;
		//rectShadowRenderConfig.useZOnly = true;
		//rectShadowRenderConfig.recomputeProjectionMatrix = false;
		////rectShadowRenderConfig.fbo = "rectShadowFBO";
		//rectShadowRenderConfig.writeFBOs.push_back({ "rectShadowFBO", nullptr });

		//RendererFramebuffer& rectShadowFBO = rendererContext.fbos["rectShadowFBO"];
		//rectShadowFBO.init("rectShadowFBO");
		//rectShadowFBO.setDimensions(rectShadowRenderConfig.viewportRect.w, rectShadowRenderConfig.viewportRect.h);
		//rectShadowFBO.addTexture2D(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_RGBA8, true);
		//rectShadowFBO.addTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_DEPTH_COMPONENT32F, false);
		//if (!rectShadowFBO.make()) {
		//	HFLOGERROR("Could not make rect shadow map FBO.");
		//}

		//RendererConfig& cubeShadowRenderConfig = rendererContext.rendererConfigs["cubeShadow"];
		//cubeShadowRenderConfig.clearColorBuffer = true;
		//cubeShadowRenderConfig.clearDepthBuffer = true;
		//cubeShadowRenderConfig.viewportRect.x = 0;
		//cubeShadowRenderConfig.viewportRect.y = 0;
		//cubeShadowRenderConfig.viewportRect.w = 64;
		//cubeShadowRenderConfig.viewportRect.h = 64;
		//cubeShadowRenderConfig.isCubeMap = true;
		//cubeShadowRenderConfig.useSceneCamera = false;
		//cubeShadowRenderConfig.useZOnly = true;
		//cubeShadowRenderConfig.fov = 90.0;
		//cubeShadowRenderConfig.recomputeProjectionMatrix = false;
		//cubeShadowRenderConfig.writeFBOs.push_back({ "cubeShadowFBO", nullptr });

		//// new method with the Simple GLES 30 Renderer
		//int cubeMapSize = 128;
		//RendererConfig& defaultRenderConfig = rendererContext.rendererConfigs["default"];
		//defaultRenderConfig.viewportRect.w = cubeMapSize;
		//defaultRenderConfig.viewportRect.h = cubeMapSize;
		//defaultRenderConfig.postCameraMatrix = ssg.spheres[1].transform;
		//defaultRenderConfig.useSceneCamera = false;
		//defaultRenderConfig.isCubeMap = true;

		//rendererContext.renderers["gles30CubeMap"].setRenderConfig(&defaultRenderConfig);

		FxSetDefaultErrorMessage();
	}

	void SSPHH_Application::LoadRenderConfigs() {
		HFLOGINFO("resetting and loading render configs...");
		rendererContext.reset();
		rendererContext.resize(getWidthi(), getHeighti());

		if (!rendererContext.loadConfig(default_renderconfig_path)) {
			HFLOGERROR("%s file not found.", default_renderconfig_path);
		}

		rendererContext.loadShaders();
		rendererContext.loadTextures();
		rendererContext.loadMaps(ssg.materials.GetMapPaths());

		//RendererConfig& defaultRenderConfig = rendererContext.rendererConfigs["default"];
		//defaultRenderConfig.zShaderProgram = rendererContext.findProgram("pb_monolithic", "DefaultZProgram");
		//defaultRenderConfig.shaderProgram = rendererContext.findProgram("pb_monolithic", "DefaultSlowProgram");

		//RendererConfig& rectShadowRenderConfig = rendererContext.rendererConfigs["rectShadow"];
		//rectShadowRenderConfig.zShaderProgram = rendererContext.findProgram("pb_monolithic", "DefaultZProgram");
		//rectShadowRenderConfig.shaderProgram = rendererContext.findProgram("pb_monolithic", "DefaultSlowProgram");

		//RendererConfig& cubeShadowRenderConfig = rendererContext.rendererConfigs["cubeShadow"];
		//cubeShadowRenderConfig.zShaderProgram = rendererContext.findProgram("cube_shadow", "cube_shadow");

		// TODO: setup renderers automatically in RendererContext
		//gles30CubeMap.GetRenderConfig().shaderProgram = rendererContext.findProgram("pb_monolithic", "DefaultSlowProgram");
		//gles30CubeMap.GetRenderConfig().zShaderProgram = rendererContext.findProgram("pb_monolithic", "DefaultZProgram");

		// New four split render config
		//RendererConfig& foursplitULRenderConfig = rendererContext.rendererConfigs["foursplitUL"];
		//RendererConfig& foursplitURRenderConfig = rendererContext.rendererConfigs["foursplitUR"];
		//RendererConfig& foursplitLLRenderConfig = rendererContext.rendererConfigs["foursplitLL"];
		//RendererConfig& foursplitLRRenderConfig = rendererContext.rendererConfigs["foursplitLR"];

		//foursplitULRenderConfig.shaderProgram = rendererContext.findProgram("foursplit", "upperleft");
		//foursplitURRenderConfig.shaderProgram = rendererContext.findProgram("foursplit", "upperright");
		//foursplitLLRenderConfig.shaderProgram = rendererContext.findProgram("foutsplit", "lowerleft");
		//foursplitLRRenderConfig.shaderProgram = rendererContext.findProgram("foursplit", "lowerright");
		//if (!foursplitLLRenderConfig.check()) HFLOGERROR("Four split renderconfig failure! --> Lower left");
		//if (!foursplitLRRenderConfig.check()) HFLOGERROR("Four split renderconfig failure! --> Lower right");
		//if (!foursplitULRenderConfig.check()) HFLOGERROR("Four split renderconfig failure! --> Upper left");
		//if (!foursplitURRenderConfig.check()) HFLOGERROR("Four split renderconfig failure! --> Lower right");

		FxSetDefaultErrorMessage();
	}

	void SSPHH_Application::ReloadRenderConfigs() {
		Hf::StopWatch stopwatch;
		InitRenderConfigs();
		LoadRenderConfigs();
		stopwatch.Stop();
		HFLOGINFO("reload took %4.2f milliseconds", stopwatch.GetMillisecondsElapsed());
		Interface.lastRenderConfigLoadTime = stopwatch.GetMillisecondsElapsed();
	}
} // namespace SSPHH