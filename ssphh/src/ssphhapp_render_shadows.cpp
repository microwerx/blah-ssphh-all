#include <ssphhapp.hpp>
#include <fluxions_renderer_utilities.hpp>

namespace SSPHH
{
	void SSPHH_Application::RenderGLES30Shadows() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		RendererConfig& cubeShadow = rendererContext.rendererConfigs["cubeShadow"];
		RendererConfig& rectShadow = rendererContext.rendererConfigs["rectShadow"];

		double sunShadowT0 = Hf::Log.getSecondsElapsed();
		rectShadow.renderSkyBox = false;
		rectShadow.viewportRect.w = Interface.renderconfig.sunShadowMapSize;
		rectShadow.viewportRect.h = Interface.renderconfig.sunShadowMapSize;
		rectShadow.preCameraMatrix.LoadIdentity();
		rectShadow.postCameraMatrix.LoadIdentity();
		rectShadow.projectionMatrix = ssg.environment.sunShadowProjectionMatrix;
		rectShadow.cameraMatrix = ssg.environment.sunShadowViewMatrix;
		rectShadow.cameraPosition = ssg.environment.sunShadowMapOrigin;
		rectShadow.znear = ssg.environment.sunShadowMapNearZ;
		rectShadow.zfar = ssg.environment.sunShadowMapFarZ;
		rectShadow.clearColor.reset(1.0f, 0.0f, 1.0f, 1.0f);
		rectShadow.clearColorBuffer = true;
		rectShadow.renderToFBO = true;

		RenderImage(rendererContext, ssg, "gles30Shadow", "rectShadow");
		//ssg.AdvancedRender(rectShadow);
		//rectShadow.fbo.generateMipmaps();

		// Sun Shadow Map is rendered, now let's bind it to a texture_ map...

		// FIXME: we shouldn't be using the SSG for rendering
		//for (auto& rt : rectShadow.fbo.renderTargets) {
		//	if (rt.second.attachment == GL_COLOR_ATTACHMENT0)
		//		ssg.environment.sunColorMapId = rt.second.object;
		//	if (rt.second.attachment == GL_DEPTH_ATTACHMENT)
		//		ssg.environment.sunDepthMapId = rt.second.object;
		//}

		//for (int i = 0; i < (int)rectShadowRenderConfig.fbo.renderTargets.size(); i++)
		//{
		//	rts[rectShadow.fbo.renderTargets[i].first] = &(rectShadow.fbo.renderTargets[i].second);
		//}
		//for (auto rtIt = rts.begin(); rtIt != rts.end(); rtIt++)
		//{
		//	RenderTarget &rt = *(rtIt->second);
		//	if (rt.attachment == GL_COLOR_ATTACHMENT0)
		//	{
		//		ssg.environment.sunColorMapId = rt.object;

		//		rt.unit = ssg.environment.sunColorMapUnit;
		//		rt.sampler = ssg.environment.sunColorMapSamplerId;
		//	}
		//	else if (rt.attachment == GL_DEPTH_ATTACHMENT)
		//	{
		//		ssg.environment.sunDepthMapId = rt.object;

		//		rt.unit = ssg.environment.sunDepthMapUnit;
		//		rt.sampler = ssg.environment.sunDepthMapSamplerId;
		//	}
		//	else
		//	{
		//		rt.unit = 0;
		//		rt.sampler = 0;
		//	}

		//	FxBindTextureAndSampler(rt.unit, rt.target, rt.object, rt.sampler);

		//	int w, h;
		//	int mipLevel = 0;
		//	glGetTexLevelParameteriv(rt.target, mipLevel, GL_TEXTURE_WIDTH, &w);
		//	glGetTexLevelParameteriv(rt.target, mipLevel, GL_TEXTURE_HEIGHT, &h);

		//	FxBindTextureAndSampler(rt.unit, rt.target, 0, 0);
		//	rt.unit = 0;
		//}
		ssg.environment.sunShadowMapTime = (float)(1000.0f * (Hf::Log.getSecondsElapsed() - sunShadowT0));

		if (Interface.captureShadows) {
			SaveTextureMap(GL_TEXTURE_2D, ssg.environment.sunColorMapId, "sun_color.ppm");
			SaveTextureMap(GL_TEXTURE_2D, ssg.environment.sunDepthMapId, "sun_depth.ppm");
		}

		// Render cube shadow map for light 0
		for (int i = 0; i < ssg.pointLights.size(); i++) {
			auto& spl = ssg.pointLights[i];
			auto& scs = ssg.pointLights[i].scs;
			
			scs.zfar = cubeShadow.zfar;
			cubeShadow.fbo_gen_color = false;

			cubeShadow.renderToFBO = false;
			cubeShadow.useSceneCamera = false;
			cubeShadow.cameraMatrix.LoadIdentity();
			cubeShadow.cameraPosition = Vector4f(spl.position, 1.0f);

			//RenderCubeShadowMap(ssg, scs, cubeShadow);
			RenderCube(rendererContext, ssg, "gles30Shadow", "cubeShadow");
			FxSetErrorMessage("ssphh.cpp", __LINE__, __FUNCTION__);
		}

		for (int i = 0; i < ssgUserData->ssphhLights.size(); i++) {
			auto& sphl = ssgUserData->ssphhLights[i];
			auto& scs = sphl.depthSphlMap;

			scs.zfar = cubeShadow.zfar;
			if (Interface.ssphh.enableShadowColorMap) {
				sphl.colorSphlMap.createTextureCube();
				cubeShadow.clearColor.reset(0.2f, 0.4f, 0.6f, 1.0f);
				cubeShadow.fbo_gen_color = true;
				cubeShadow.fbo_color_map = sphl.colorSphlMap.getTexture();
			}
			else {
				cubeShadow.fbo_gen_color = false;
				cubeShadow.fbo_color_map = 0;
			}
			cubeShadow.renderToFBO = false;
			cubeShadow.useSceneCamera = false;
			cubeShadow.cameraMatrix.LoadIdentity();
			cubeShadow.cameraPosition = sphl.position;

			//RenderCubeShadowMap(ssg, sphl.depthSphlMap, cubeShadow);
			RenderCube(rendererContext, ssg, "gles30Shadow", "cubeShadow");
			FxSetErrorMessage("ssphh.cpp", __LINE__, __FUNCTION__);

			if (Interface.captureShadows) {
				std::ostringstream ostr;
				ostr << "sphl" << std::setw(2) << std::setfill('0') << i;
				SaveTextureMap(GL_TEXTURE_CUBE_MAP, sphl.colorSphlMap.getTexture(), ostr.str() + "_color.ppm");
				SaveTextureMap(GL_TEXTURE_CUBE_MAP, sphl.depthSphlMap.getTexture(), ostr.str() + "_depth.ppm");
			}
		}

		Interface.captureShadows = false;
	}
} // namespace SSPHH
