#include "pch.hpp"
#include <fluxions_renderer_utilities.hpp>
#include <ssphhapp.hpp>

namespace SSPHH {

	bool operator == (Fluxions::Matrix4f a, Fluxions::Matrix4f b) {
		float *am = &a.m11;
		float *bm = &b.m11;
		for (int i = 0; i < 16; i++) {
			if (am[i] != bm[i]) return false;
		}
		return true;
	}

	void SSPHH_Application::RenderGLES30_RectShadows() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		auto pbrRC = rendererContext->getRendererConfig("pbr_rc");
		if (!pbrRC) return;

		auto shadowRenderer = rendererContext->getRenderer("rect_shadow_renderer");
		if (!shadowRenderer) return;
		if (!shadowRenderer->validate()) {
			shadowRenderer->setSceneGraph(ssg);
		}

		auto shadowProgram = rendererContext->getProgram("rect_shadow_program");
		if (shadowProgram.expired()) return;

		auto rectShadowRC = shadowRenderer->getRenderConfig();
		if (!rectShadowRC) return;

		int dirtoLightIndex = 0;
		for (auto [_, dtl] : ssg->dirToLights) {
			// make framebuffer if it doesn't exist
			if (dtl->notrenderable()) {
				auto fbo = MakeRendererFramebuffer(dtl->name_str(), rendererContext);
				rendererContext->fbos[dtl->name_str()] = fbo;
				fbo->setDimensions(1024, 1024);
				fbo->addTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_DEPTH_COMPONENT32F, false);
				fbo->setMapName("DirToLightDepthMaps[" + std::to_string(dirtoLightIndex) + "]");
				fbo->addTexture2D(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_RGBA8, false);
				fbo->setMapName("DirToLightColorMaps[" + std::to_string(dirtoLightIndex) + "]");
				fbo->makeRenderable();
				dtl->setrenderable(true);

				pbrRC->readFBOs.push_back({ dtl->name_str(), fbo });
			}

			auto wptr = rendererContext->getFramebuffer(dtl->name_str());
			if (wptr.expired()) continue;

			rectShadowRC->writeFBO = wptr.lock();
			// Render shadow map for dirto light
			rectShadowRC->cameraMatrix = dtl->worldMatrix().AsInverse();
			rectShadowRC->useSceneCamera = false;
			rectShadowRC->useSceneProjection = false;
			rectShadowRC->useZOnly = true;
			rectShadowRC->enableDepthTest = true;
			rectShadowRC->clearColor.reset(0.2f, 0.2f, 0.2f);
			rectShadowRC->clearColorBuffer = true;
			rectShadowRC->clearDepthBuffer = true;
			rectShadowRC->viewportRect.w = 1024;
			rectShadowRC->viewportRect.h = 1024;
			dtl->calcOptimalMatrices(
				ssg->getBoundingBox(),
				rectShadowRC->viewportProjectionMatrix,
				rectShadowRC->viewportCameraMatrix);

			if (rectShadowRC->viewportProjectionMatrix == Matrix4f::MakeIdentity()) {
				HFLOGWARN("Viewport Projection error")
			}
			if (rectShadowRC->viewportCameraMatrix == Matrix4f::MakeIdentity()) {
				HFLOGWARN("Viewport Camera error")
			}

			FxCheckErrorsWarn();
			shadowRenderer->saveGLState();
			shadowRenderer->applyRenderConfig();
			FxCheckErrorsWarn();
			for (const auto& [geomid, geom] : ssg->geometryGroups) {
				// render each mesh
				Matrix4f worldMatrix = geom->worldMatrix();
				shadowRenderer->renderMesh(*geom->mesh, true, worldMatrix);
			}
			FxCheckErrorsWarn();
			shadowRenderer->restoreGLState();
			dirtoLightIndex++;
		}
	}

	void SSPHH_Application::RenderGLES30_CubeShadows() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		auto pbrRC = rendererContext->getRendererConfig("pbr_rc");
		if (!pbrRC) return;

		auto cubeShadowRC = rendererContext->getRendererConfig("cubeShadow");
		if (!cubeShadowRC) return;
	}

	void SSPHH_Application::RenderGLES30Shadows() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		if (Interface->drawRectShadows) RenderGLES30_RectShadows();
		if (Interface->drawCubeShadows) RenderGLES30_CubeShadows();


		// RendererConfig& cubeShadow = rendererContext.rendererConfigs["cubeShadow"];
		// RendererConfig& rectShadow = rendererContext.rendererConfigs["rectShadow"];

		// double sunShadowT0 = HFLOG_SECS_ELAPSED();
		// rectShadow.renderSkyBox = false;
		// rectShadow.viewportRect.w = Interface->renderconfig.sunShadowMapSize;
		// rectShadow.viewportRect.h = Interface->renderconfig.sunShadowMapSize;
		// rectShadow.preCameraMatrix.LoadIdentity();
		// rectShadow.postCameraMatrix.LoadIdentity();
		// rectShadow.projectionMatrix = ssg.environment.sunShadowProjectionMatrix;
		// rectShadow.cameraMatrix_ = ssg.environment.sunShadowViewMatrix;
		// rectShadow.cameraPosition = ssg.environment.sunShadowMapOrigin;
		// rectShadow.viewportZNear = ssg.environment.sunShadowMapNearZ;
		// rectShadow.viewportZFar = ssg.environment.sunShadowMapFarZ;
		// rectShadow.clearColor.clearCoefs(1.0f, 0.0f, 1.0f, 1.0f);
		// rectShadow.clearColorBuffer = true;
		// rectShadow.renderToFBO = true;

		// RenderImage(rendererContext, ssg, "gles30Shadow", "rectShadow");
		////ssg.AdvancedRender(rectShadow);
		////rectShadow.fbo.generateMipmaps();

		//// Sun Shadow Map is rendered, now let's bind it to a texture_ map...

		//// FIXME: we shouldn't be using the SSG for rendering
		////for (auto& rt : rectShadow.fbo.renderTargets) {
		////	if (rt.second.attachment == GL_COLOR_ATTACHMENT0)
		////		ssg.environment.sunColorMapId = rt.second.object;
		////	if (rt.second.attachment == GL_DEPTH_ATTACHMENT)
		////		ssg.environment.sunDepthMapId = rt.second.object;
		////}

		////for (int i = 0; i < (int)rectShadowRenderConfig.fbo.renderTargets.size(); i++)
		////{
		////	rts[rectShadow.fbo.renderTargets[i].first] = &(rectShadow.fbo.renderTargets[i].second);
		////}
		////for (auto rtIt = rts.begin(); rtIt != rts.end(); rtIt++)
		////{
		////	RenderTarget &rt = *(rtIt->second);
		////	if (rt.attachment == GL_COLOR_ATTACHMENT0)
		////	{
		////		ssg.environment.sunColorMapId = rt.object;

		////		rt.unit = ssg.environment.sunColorMapUnit;
		////		rt.sampler = ssg.environment.sunColorMapSamplerId;
		////	}
		////	else if (rt.attachment == GL_DEPTH_ATTACHMENT)
		////	{
		////		ssg.environment.sunDepthMapId = rt.object;

		////		rt.unit = ssg.environment.sunDepthMapUnit;
		////		rt.sampler = ssg.environment.sunDepthMapSamplerId;
		////	}
		////	else
		////	{
		////		rt.unit = 0;
		////		rt.sampler = 0;
		////	}

		////	FxBindTextureAndSampler(rt.unit, rt.target, rt.object, rt.sampler);

		////	int w, h;
		////	int mipLevel = 0;
		////	glGetTexLevelParameteriv(rt.target, mipLevel, GL_TEXTURE_WIDTH, &w);
		////	glGetTexLevelParameteriv(rt.target, mipLevel, GL_TEXTURE_HEIGHT, &h);

		////	FxBindTextureAndSampler(rt.unit, rt.target, 0, 0);
		////	rt.unit = 0;
		////}
		// ssg.environment.sunShadowMapTime = (float)(1000.0f * (HFLOG_SECS_ELAPSED() - sunShadowT0));

		// if (Interface->captureShadows) {
		//	SaveTextureMap(GL_TEXTURE_2D, ssg.environment.sunColorMapId, "sun_color.ppm");
		//	SaveTextureMap(GL_TEXTURE_2D, ssg.environment.sunDepthMapId, "sun_depth.ppm");
		//}

		//// Render cube shadow map for light 0
		// for (int i = 0; i < ssg.pointLights.size(); i++) {
		//	auto& spl = ssg.pointLights[i];
		//	auto& scs = ssg.pointLights[i].scs;
		//
		//	scs.zfar = cubeShadow.viewportZFar;
		//	cubeShadow.fbo_gen_color = false;

		//	cubeShadow.renderToFBO = false;
		//	cubeShadow.useSceneCamera = false;
		//	cubeShadow.cameraMatrix_.LoadIdentity();
		//	cubeShadow.cameraPosition = Vector4f(spl.position, 1.0f);

		//	//RenderCubeShadowMap(ssg, scs, cubeShadow);
		//	RenderCube(rendererContext, ssg, "gles30Shadow", "cubeShadow");
		//	FxSetErrorMessage("ssphh.cpp", __LINE__, __FUNCTION__);
		//}

		// for (int i = 0; i < ssgUserData->ssphhLights.size(); i++) {
		//	auto& sphl = ssgUserData->ssphhLights[i];
		//	auto& scs = sphl.depthSphlMap;

		//	scs.zfar = cubeShadow.viewportZFar;
		//	if (Interface->ssphh.enableShadowColorMap) {
		//		sphl.colorSphlMap.createTextureCube();
		//		cubeShadow.clearColor.clearCoefs(0.2f, 0.4f, 0.6f, 1.0f);
		//		cubeShadow.fbo_gen_color = true;
		//		cubeShadow.fbo_color_map = sphl.colorSphlMap.getTexture();
		//	}
		//	else {
		//		cubeShadow.fbo_gen_color = false;
		//		cubeShadow.fbo_color_map = 0;
		//	}
		//	cubeShadow.renderToFBO = false;
		//	cubeShadow.useSceneCamera = false;
		//	cubeShadow.cameraMatrix_.LoadIdentity();
		//	cubeShadow.cameraPosition = sphl.position;

		//	//RenderCubeShadowMap(ssg, sphl.depthSphlMap, cubeShadow);
		//	RenderCube(rendererContext, ssg, "gles30Shadow", "cubeShadow");
		//	FxSetErrorMessage("ssphh.cpp", __LINE__, __FUNCTION__);

		//	if (Interface->captureShadows) {
		//		std::ostringstream ostr;
		//		ostr << "sphl" << std::setw(2) << std::setfill('0') << i;
		//		SaveTextureMap(GL_TEXTURE_CUBE_MAP, sphl.colorSphlMap.getTexture(), ostr.str() + "_color.ppm");
		//		SaveTextureMap(GL_TEXTURE_CUBE_MAP, sphl.depthSphlMap.getTexture(), ostr.str() + "_depth.ppm");
		//	}
		//}

		Interface->captureShadows = false;
	}
} // namespace SSPHH
