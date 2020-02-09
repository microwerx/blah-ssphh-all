#include <fluxions_renderer_utilities.hpp>
#include <ssphhapp.hpp>

namespace SSPHH
{
	void SSPHH_Application::SetupRenderGLES30() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);
		SSG_OptimizeClippingPlanes();
		UpdateSPHLs();
		UploadSPHLs();
	}


	void SSPHH_Application::RenderGLES30() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		// Implementation Plan
		// -------------------
		// Render Sky Box into FBO
		// Render PBR into FBO
		// Render VIZ into FBO
		// Render POST with FBO to screen

		//SetupRenderGLES30();
		//RenderGLES30Shadows();
		//RenderGLES30Scene();

		//RenderGLES30SPHLs();
		//RenderGL11Hierarchies();

		//if (counter == 1)
		//	RenderTest1SunShadows();
		//if (counter == 2)
		//	RenderTest2SphereCubeMap();
		//if (counter == 3)
		//	RenderTest3EnviroCubeMap();

		if (Interface.drawSkyBox) {
			RenderGLES30_SkyBox();
		}
		
		if (Interface.drawPBR) {
			RenderGLES30_SceneGraph();
		}

		if (Interface.drawVIZ) {
			RenderGLES30_VIZ();
		}
		
		if (Interface.drawPOST) {
			RenderGLES30_PostProcess();
		}

		FxSetDefaultErrorMessage();
	}

	void SSPHH_Application::RenderGLES30_SkyBox() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		const std::string renderername{ "skybox" };
		if (rendererContext.renderers.count(renderername)) {
			const std::string& renderconfigname = rendererContext.renderers[renderername].renderconfigname;
			Fluxions::RenderImage(rendererContext, ssg, renderername, renderconfigname);
		}

		while (glGetError() != GL_NO_ERROR) HFLOGWARN("OpenGL Error");
	}

	void SSPHH_Application::RenderGLES30_SceneGraph() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		const std::string renderername{ "pbr" };
		if (rendererContext.renderers.count(renderername)) {
			const std::string& renderconfigname = rendererContext.renderers[renderername].renderconfigname;
			Fluxions::RenderImage(rendererContext, ssg, renderername, renderconfigname);
		}

		while (glGetError() != GL_NO_ERROR) HFLOGWARN("OpenGL Error");
	}

	void SSPHH_Application::RenderGLES30_VIZ() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		static double viz_lag{ 0 };
		static constexpr double MS_PER_VIZ_UPDATE = 1.0 / 30.0;
		viz_lag += GetFrameTime();
		bool updateViz{ false };
		while (viz_lag >= MS_PER_VIZ_UPDATE) {
			viz_lag -= MS_PER_VIZ_UPDATE;
			updateViz = true;
		}

		const std::string renderername{ "viz" };
		if (rendererContext.renderers.count(renderername)) {
			const std::string& renderconfigname = rendererContext.renderers[renderername].renderconfigname;
			if (updateViz) {
				rendererContext.renderers[renderername].update(RendererGLES30::UPDATE_VIZ);
			}
			Fluxions::RenderImage(rendererContext, ssg, renderername, renderconfigname);
		}

		while (glGetError() != GL_NO_ERROR) HFLOGWARN("OpenGL Error");
	}

	void SSPHH_Application::RenderGLES30_PostProcess() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		const std::string renderername{ "postprocess" };
		if (rendererContext.renderers.count(renderername)) {
			const std::string& renderconfigname = rendererContext.renderers[renderername].renderconfigname;
			Fluxions::RenderImage(rendererContext, ssg, renderername, renderconfigname);
		}

		while (glGetError() != GL_NO_ERROR) HFLOGWARN("OpenGL Error");
	}


	void SSPHH_Application::RenderGLES30Scene() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		//RendererConfig& defaultRenderConfig = rendererContext.rendererConfigs["pbr_shader"];
		//defaultRenderConfig.clearDepthBuffer = true;
		//defaultRenderConfig.clearColorBuffer = false;
		//defaultRenderConfig.viewportRect.x = 0;
		//defaultRenderConfig.viewportRect.y = 0;
		//defaultRenderConfig.viewportRect.w = (size_t)screenWidth;
		//defaultRenderConfig.viewportRect.h = (size_t)screenHeight;
		//defaultRenderConfig.preCameraMatrix = Interface.inversePreCameraMatrix;
		//defaultRenderConfig.postCameraMatrix = Interface.inversePostCameraMatrix;
		//defaultRenderConfig.fov = Interface.ssg.cameraFOV;
		//defaultRenderConfig.isCubeMap = false;
		//defaultRenderConfig.useSceneCamera = true;
		//defaultRenderConfig.useZOnly = false;
		//defaultRenderConfig.useMaterials = true;
		//defaultRenderConfig.shaderDebugChoice = Interface.tools.shaderDebugChoice;
		//defaultRenderConfig.shaderDebugLight = Interface.tools.shaderDebugLight;
		//defaultRenderConfig.shaderDebugSphl = Interface.tools.shaderDebugSphl;

		while (glGetError());
		const std::string renderername{ "skybox" };
		if (rendererContext.renderers.count(renderername)) {
			const std::string& renderconfigname = rendererContext.renderers[renderername].renderconfigname;
			Fluxions::RenderImage(rendererContext, ssg, renderername, renderconfigname);
		}
		//ssg.AdvancedRender(defaultRenderConfig);
		//ssg.camera.actualViewMatrix = defaultRenderConfig.cameraMatrix_;
		while (glGetError()) HFLOGWARN("AdvancedRender() ERROR!");

		if (Interface.drawSkyBox) {
			FxSetErrorMessage(__FILE__, __LINE__, "skybox");
			glEnable(GL_DEPTH_TEST);
			Sky_Render();
			glDisable(GL_DEPTH_TEST);
			FxSetErrorMessage("ssphh.cpp", __LINE__, __FUNCTION__);
			while (glGetError()) HFLOGWARN("Draw SkyBox ERROR!");
		}

		glUseProgram(0);
		glDisable(GL_DEPTH_TEST);
	}
}
