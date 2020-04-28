#include "pch.hpp"
#include <ssphhapp.hpp>
#include <hatchetfish.hpp>
#include <hatchetfish_stopwatch.hpp>

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
		//rectShadowRenderConfig.viewportRect.w = Interface->renderconfig.sunShadowMapSize;
		//rectShadowRenderConfig.viewportRect.h = Interface->renderconfig.sunShadowMapSize;
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
		rendererContext->reinit();
		rendererContext->resize(getWidthi(), getHeighti());

		if (!rendererContext->loadConfig(default_renderconfig_path)) {
			HFLOGERROR("%s file not found.", default_renderconfig_path);
		}
		
		for (auto& [_, rdr] : rendererContext->renderers) {
			rdr->setSceneGraph(ssg);
		}

		rendererContext->loadShaders();
		rendererContext->loadTextures();
		rendererContext->loadMaps(ssg->materials.maps);
		rendererContext->makeFramebuffers();

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
		Interface->lastRenderConfigLoadTime = stopwatch.GetMillisecondsElapsed();
	}


	void SSPHH_Application::LoadShaders() {
		for (auto& [_, p] : rendererContext->programs) {
			p->setloaded(false);
			p->setusable(false);
		}
		rendererContext->loadShaders();
	}
} // namespace SSPHH
