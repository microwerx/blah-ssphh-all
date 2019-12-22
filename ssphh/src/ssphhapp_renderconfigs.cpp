#include <ssphhapp.hpp>
#include <hatchetfish_stopwatch.hpp>
#include <ssphhapp_renderconfigs.hpp>

namespace SSPHH
{
	const char* renderconfig_filename = "resources/config/pb_monolithic_2020.renderconfig";
}

RendererWindow::RendererWindow(const std::string& name)
	: Vf::Window(name) {}

RendererWindow::~RendererWindow() {}

void RendererWindow::OnUpdate(double timeStamp) {
	if (!ssphh_widget_ptr) return;
	Vf::Window::OnUpdate(timeStamp);

	if (context != &ssphh_widget_ptr->rendererContext)
		context = &ssphh_widget_ptr->rendererContext;
}

void RendererWindow::OnRenderDearImGui() {
	if (!context || !beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	if (ImGui::Button("Load Configs")) {
		Hf::StopWatch stopwatch;
		context->loadConfig(SSPHH::renderconfig_filename);
		lastConfigsLoadTime = stopwatch.Stop_sf();
	}
	ImGui::Value("configs load", lastConfigsLoadTime);

	if (ImGui::Button("Load Shaders")) {
		Hf::StopWatch stopwatch;
		context->loadShaders();
		lastShadersLoadTime = stopwatch.Stop_sf();
	}
	ImGui::Value("shaders load", lastShadersLoadTime);

	if (ImGui::Button("Load Textures")) {
		Hf::StopWatch stopwatch;
		context->loadTextures();
		lastTextureLoadTime = stopwatch.Stop_sf();
	}
	ImGui::Value("texture load", lastTextureLoadTime);

	if (ImGui::TreeNode("programs")) {
		for (auto& [k, ro] : context->programs) {
			ImGui::Text(ro.name());
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("renderers")) {
		for (auto& [k, ro] : context->renderers) {
			ImGui::Text(ro.name());
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("rendererconfigs")) {
		for (auto& [k, ro] : context->rendererConfigs) {
			ImGui::Text(ro.name());
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("texture2Ds")) {
		for (auto& [k, t] : context->texture2Ds) {
			ImGui::Text(t.name());
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("textureCubes")) {
		for (auto& [k, t] : context->textureCubes) {
			ImGui::Text(t.name());
		}
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
		rendererContext.resize(
			(int)screenWidth, (int)screenHeight,
			(int)screenWidth, (int)screenHeight);

		if (!rendererContext.loadConfig(renderconfig_filename)) {
			HFLOGERROR("%s file not found.", renderconfig_filename);
		}

		rendererContext.loadShaders();
		rendererContext.loadTextures();

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