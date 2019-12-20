// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#include "pch.hpp"

#include <memory>
#include <ssphhapp.hpp>
#include <hatchetfish_stopwatch.hpp>

namespace Fluxions
{
	extern bool debugging;
} // namespace Fluxions

SSPHHPtr ssphh_widget_ptr;
extern std::vector<std::string> g_args;

#ifdef USE_GLUT
extern void PrintBitmapStringJustified(float x, float y, int justification, void* font, const char* format, ...);
extern void PrintString9x15(float x, float y, int justification, const char* format, ...);
#endif

extern double gt_Fps;
extern double g_distance;

//using namespace Fluxions;

void InitSSPHH() {
	ssphh_widget_ptr = std::make_shared<SSPHH::SSPHH_Application>();
}

void KillSSPHH() {
	//ssphh->Stop();
	ssphh_widget_ptr->Kill();
}

namespace SSPHH
{
	template <typename T>
	void deletenull(T** ptr) {
		if (*ptr == nullptr) {
			HFLOGWARN("Deleting nullptr");
		}
		delete *ptr;
		*ptr = nullptr;
	}

	namespace Fx = Fluxions;
	using namespace Fluxions;
	using namespace Vf;

	const char* default_coronaskyboxcubemap_path = "export_cubemap.png";
	const char* default_scene_file = "resources/scenes/maze_scene/maze.scn";
	//const char* default_scene_file = "resources/scenes/test_texture_scene/test_terrain_scene.scn";
	// The renderconfig default is in ssphhapp_renderconfigs.cpp

	SSPHH_Application::SSPHH_Application()
		: Widget("ssphhapplication") {
		sceneFilename = default_scene_file;
	}

	SSPHH_Application::SSPHH_Application(const std::string& name)
		: Vf::Widget(name) {
		sceneFilename = default_scene_file;
	}

	SSPHH_Application::~SSPHH_Application() {
		deletenull(&ssgUserData);
	}

	void SSPHH_Application::ParseCommandArguments(const std::vector<std::string>& cmdargs) {
		if (cmdargs.size() <= 1)
			return;

		size_t count = cmdargs.size();
		int i = 1;
		for (size_t j = 1; j < count; j++) {
			bool nextArgExists = j < count - 1;
			if ((cmdargs[j] == "-scene") && nextArgExists) {
				// next argument is the path
				FilePathInfo fpi(cmdargs[j + 1]);
				if (fpi.Exists()) {
					sceneFilename = cmdargs[j + 1];
					HFLOGINFO("loading scene file %s", sceneFilename.c_str());
				}
				else {
					HFLOGERROR("scene file %s does not exist.", sceneFilename.c_str());
				}
				j++;
			}

			if (cmdargs[j] == "-broker") {
				Interface.uf.uf_type = UfType::Broker;
				HFLOGINFO("Unicornfish: starting in broker mode");
			}
			if (cmdargs[j] == "-worker") {
				Interface.uf.uf_type = UfType::Worker;
				HFLOGINFO("Unicornfish: starting in client mode");
			}
			if (cmdargs[j] == "-client") {
				Interface.uf.uf_type = UfType::Client;
				HFLOGINFO("Unicornfish: starting in worker mode");
			}
			if ((cmdargs[j] == "-endpoint") && nextArgExists) {
				Interface.uf.endpoint = cmdargs[j + 1];
				j++;
				HFLOGINFO("Unicornfish: using endpoint %s", Interface.uf.endpoint.c_str());
			}
			if ((cmdargs[j] == "-service") && nextArgExists) {
				Interface.uf.service = cmdargs[j + 1];
				HFLOGINFO("Unicornfish: using service %s", Interface.uf.service.c_str());
				j++;
			}
		}
	}

	void SSPHH_Application::StartPython() {
		//python.init(string("ssphh"));
		//python.start();
		//python.run();
	}

	int init_count = 0;
	void SSPHH_Application::OnInit(const std::vector<std::string>& args) {
		Widget::OnInit(args);
		HFLOGINFO("Initializing SSPHH App");
		init_count++;

		//rendererContext.renderers["gles30"].init("gles30");
		//rendererContext.renderers["gles30CubeMap"].init("gles30CubeMap");
		//rendererContext.renderers["sph_renderer"].init("sph_renderer");

		//rendererContext.samplers["samplerCube"].init("samplerCube");
		//rendererContext.samplers["sampler2D"].init("sampler2D");
		//rendererContext.samplers["shadowCube"].init("shadowCube");
		//rendererContext.samplers["shadow2D"].init("shadow2D");

		//rendererContext.textures["enviroSkyBox"].init("enviroSkyBox");
		//rendererContext.textures["pbSkyBox"].init("pbSkyBox");

		//rendererContext.rendererConfigs["default"].init("default");
		//rendererContext.rendererConfigs["gbuffer"].init("gbuffer");
		//rendererContext.rendererConfigs["cubeShadow"].init("cubeShadow");
		//rendererContext.rendererConfigs["cubeEnvMap"].init("cubeEnvMap");
		//rendererContext.rendererConfigs["rectShadow"].init("rectShadow");
		//rendererContext.rendererConfigs["rectEnvMap"].init("rectEnvMap");
		//rendererContext.rendererConfigs["uberShader"].init("uberShader");
		//rendererContext.rendererConfigs["foursplitUL"].init("foursplitUL");
		//rendererContext.rendererConfigs["foursplitUR"].init("foursplitUR");
		//rendererContext.rendererConfigs["foursplitLL"].init("foursplitLL");
		//rendererContext.rendererConfigs["foursplitLR"].init("foursplitLR");


		// TODO: I would like to make the following code work:
		//
		// SceneGraph sg;
		// sg.Init();
		// sg.Load("myscene.scn");
		//
		// RendererContext r;
		// r.Init();
		// r.LoadRenderConfig("deferred.renderconfig")
		// r.LoadRenderConfig("pbr_monolithic.renderconfig")
		// r.LoadRenderConfig("pbr_deferred.renderconfig")
		// r.BuildBuffers(sg);

		ParseCommandArguments(args);

		InitUnicornfish();
		InitImGui();
		Sky_InitViewController();

		Hf::StopWatch stopwatch;

		// StartPython();

		FxSetErrorMessage(__FILE__, __LINE__, "inside OnInit()");

		Interface.preCameraMatrix.LoadIdentity();

		const char* glrenderer = (const char*)glGetString(GL_RENDERER);
		const char* glvendor = (const char*)glGetString(GL_VENDOR);
		const char* glversion = (const char*)glGetString(GL_VERSION);

		my_hud_info.glRendererString = glrenderer ? glrenderer : "Unknown Renderer";
		my_hud_info.glVendorString = glvendor ? glvendor : "Unknown Vendor";
		my_hud_info.glVersionString = glversion ? glversion : "Unknown Version";


		//// Initialize default sampler objects
		//RendererSamplerObject& samplerCube = rendererContext.samplers["samplerCube"];
		//samplerCube.init("samplerCube");
		//samplerCube.setMagFilter(GL_LINEAR);
		//samplerCube.setMinFilter(GL_LINEAR);
		//samplerCube.setWrapSTR(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		//RendererSamplerObject& sampler2D = rendererContext.samplers["sampler2D"];
		//sampler2D.init("sampler2D");
		//sampler2D.setMagFilter(GL_LINEAR);
		//sampler2D.setMinFilter(GL_LINEAR);
		//sampler2D.setWrapST(GL_REPEAT, GL_REPEAT);
		//RendererSamplerObject& shadowCube = rendererContext.samplers["shadowCube"];
		//shadowCube.init("shadowCube");
		//shadowCube.setMagFilter(GL_LINEAR);
		//shadowCube.setMinFilter(GL_LINEAR);
		////shadowCube.setCompareFunction(GL_LESS);
		////shadowCube.setCompareMode(GL_COMPARE_REF_TO_TEXTURE);
		//RendererSamplerObject& shadow2D = rendererContext.samplers["shadow2D"];
		//shadow2D.init("shadow2D");
		//shadow2D.setMagFilter(GL_LINEAR);
		//shadow2D.setMinFilter(GL_LINEAR);
		////shadow2D.setCompareFunction(GL_LESS);
		////shadow2D.setCompareMode(GL_COMPARE_REF_TO_TEXTURE);

		// Initialize Simple Scene Graph
		FxSetErrorMessage(__FILE__, __LINE__, "initializing scene graph");

		deletenull(&ssgUserData);
		ssgUserData = new SSG_SSPHHRendererPlugin(&ssg);
		SSG_LoadScene();
		cameraAnimation.create();

		// Initialize Rendering System
		FxSetErrorMessage(__FILE__, __LINE__, "initializing rendering system");
		rendererContext.init("SSHH RendererContext", &rendererContext);
		InitRenderConfigs();
		LoadRenderConfigs();

		FxSetDefaultErrorMessage();

		// Initialize Physically Based Sky
		FxSetErrorMessage(__FILE__, __LINE__, "initializing physically based sky system");
		Sun_ResetClock();

		stopwatch.Stop();
		HFLOGINFO("OnInit() took %3.2f seconds", stopwatch.GetSecondsElapsed());
	}

	void SSPHH_Application::OnKill() {
		// TODO: I would like to make the following code work:
		//
		// SceneGraph sg;
		// RendererContext r;
		// ...
		// sg.Kill();
		// r.Kill();
		// r.Init();

		ssg.reset();
		////renderer.reset();
		//for (auto& [k, renderer] : rendererContext.renderers) {
		//	renderer.buildBuffers();
		//	renderer.reset();
		//}

		//for (auto& [k, rc] : rendererContext.rendererConfigs) {
		//	rc.reset();
		//}

		KillUnicornfish();

		if (ssgUserData) {
			ssg.userdata = nullptr;
			delete ssgUserData;
			ssgUserData = nullptr;
		}
		//python.kill();
		//python.join();

		Widget::OnKill();
	}

	const Matrix4f& SSPHH_Application::GetCameraMatrix() const {
		return Interface.preCameraMatrix;
	}


	void SSPHH_Application::OnReshape(int width, int height) {
		float w = (float)width;
		float h = (float)height;
		screenWidth = w;
		screenHeight = h;
		aspect = w / h;

		// FIXME: Are we using rendererContext
		//rendererContext.SetDeferredRect(Recti(0, 0, (int)w, (int)h));

		rendererContext.rendererConfigs["default"].projectionMatrix.LoadIdentity();
		rendererContext.rendererConfigs["default"].projectionMatrix.Perspective(ssg.camera.fov,
																				ssg.camera.imageAspect, ssg.camera.imageNearZ, ssg.camera.imageFarZ);

		screenOrthoMatrix.LoadIdentity();
		screenOrthoMatrix.Ortho2D(0.0f, screenWidth, screenHeight, 0.0f);

		Hf::Log.setMaxHistory(height / 30);
	}



	void SSPHH_Application::RenderTest1SunShadows() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		RendererConfig& rc = rendererContext.rendererConfigs["rectShadow"];
		int w = rc.viewportRect.w;
		int h = rc.viewportRect.h;
		glClear(GL_DEPTH_BUFFER_BIT);
		rc.viewportRect.x = (GLsizei)(screenWidth - 256);
		rc.viewportRect.y = 0;
		rc.viewportRect.w = 256;
		rc.viewportRect.h = 256;
		rc.clearColorBuffer = false;
		rc.renderToFBO = false;
		// TODO: fix Advanced render
		// ssg.AdvancedRender(rc);
		rc.viewportRect.w = w;
		rc.viewportRect.h = h;
		rc.viewportRect.x = 0;
		rc.viewportRect.y = 0;
	}

	void SSPHH_Application::RenderTest2SphereCubeMap() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		Matrix4f cameraMatrix = Interface.inversePreCameraMatrix * ssg.camera.viewMatrix;
		Vector3f cameraPosition(cameraMatrix.m14, cameraMatrix.m24, cameraMatrix.m34);
		int s = 128;
		RendererConfig* cubeRC = rendererContext.renderers["gles30CubeMap"].getRenderConfig();
		if (!cubeRC) return;

		// FIXME: why are we setting these values here?
		cubeRC->clearColorBuffer = false;
		cubeRC->viewportRect.x = 0;
		cubeRC->viewportRect.y = 0;
		cubeRC->preCameraMatrix = Interface.inversePreCameraMatrix;
		cubeRC->postCameraMatrix.LoadIdentity();
		cubeRC->useZOnly = false;
		cubeRC->useMaterials = true;
		cubeRC->viewportRect.w = s;
		cubeRC->viewportRect.h = s;
		cubeRC->postCameraMatrix = ssg.spheres[1].transform;
		cubeRC->useSceneCamera = true;
		cubeRC->isCubeMap = true;
		// gles30CubeMap.Render();
	}

	void SSPHH_Application::RenderTest3EnviroCubeMap() {
		FxSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);

		// FIXME: Are we using rendererContext?
		RendererProgramPtr program;// = rendererContext.FindProgram("glut", "UnwrappedCubeMap");
		if (program != nullptr) {
			program->use();
			GLint tloc = program->getAttribLocation("aTexCoord");
			GLint vloc = program->getAttribLocation("aPosition");
			RendererUniform orthoProjectionMatrix = Matrix4f().Ortho2D(0.0f, screenWidth, 0.0f, screenHeight);
			RendererUniform identityMatrix = Matrix4f().LoadIdentity();
			program->applyUniform("uCubeTexture", RendererUniform(0));
			program->applyUniform("ProjectionMatrix", orthoProjectionMatrix);
			program->applyUniform("CameraMatrix", identityMatrix);
			program->applyUniform("WorldMatrix", identityMatrix);
			FxBindTextureAndSampler(0,
									GL_TEXTURE_CUBE_MAP,
									rendererContext.textures["enviroSkyBox"].getTextureId(),
									ssg.environment.enviroColorMapSamplerId);
			FxDrawGL2UnwrappedCubeMap(0, 0, 256, vloc, tloc);
			FxBindTextureAndSampler(0, GL_TEXTURE_CUBE_MAP, 0, 0);
			glUseProgram(0);
		}
	}


	void SSPHH_Application::SaveScreenshot() {
		if (Interface.saveScreenshot) {
			Interface.saveScreenshot = false;
			glFinish();

			Image3ub image((int)screenWidth, (int)screenHeight);

			std::string filename = GetPathTracerSphlRenderName(
				Interface.sceneName,
				Interface.ssphh.enableKs,
				Interface.ssphh.REF_MaxRayDepth,
				Interface.ssphh.REF_PassLimit,
				Interface.ssphh.MaxDegrees);
			filename += ".ppm";
			Interface.ssphh.lastSphlRenderPath = filename;
			glReadPixels(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight, GL_RGB, GL_UNSIGNED_BYTE, (void*)image.getPixels(0)->const_ptr());
			image.savePPMi(filename, 1.0f, 0, 255, 0, true);
		}
	}

	std::string SSPHH_Application::GetPathTracerName(const std::string& sceneName, bool ks, int mrd, int pl) {
		std::ostringstream ostr;
		ostr << sceneName;
		ostr << "_" << std::setw(2) << std::setfill('0') << mrd;
		ostr << "_" << std::setw(2) << std::setfill('0') << pl;
		if (ks)
			ostr << "_Ks";
		return ostr.str();
	}

	std::string SSPHH_Application::GetSphlRenderName(const std::string& sceneName, int md) {
		std::ostringstream ostr;
		ostr << sceneName;
		ostr << "_sphlrender_" << std::setw(2) << std::setfill('0') << md;
		return ostr.str();
	}

	std::string SSPHH_Application::GetPathTracerSphlRenderName(const std::string& sceneName, bool ks, int mrd, int pl, int md) {
		std::ostringstream ostr;
		ostr << sceneName;
		ostr << "_" << std::setw(2) << std::setfill('0') << mrd;
		ostr << "_" << std::setw(2) << std::setfill('0') << pl;
		if (ks)
			ostr << "_Ks";
		ostr << "_sphlrender_";
		ostr << std::setw(2) << std::setfill('0') << md;
		return ostr.str();
	}

	std::string SSPHH_Application::GetStatsName(const std::string& sceneName, bool ks, int mrd, int pl, int md) {
		std::ostringstream ostr;
		ostr << sceneName;
		if (ks)
			ostr << "_Ks";
		ostr << "_" << std::setw(2) << std::setfill('0') << mrd;
		ostr << "_" << std::setw(2) << std::setfill('0') << pl;
		ostr << "_" << std::setw(2) << std::setfill('0') << md;
		return ostr.str();
	}
} // namespace SSPHH
