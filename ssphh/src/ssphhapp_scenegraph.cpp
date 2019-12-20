#include <ssphhapp.hpp>
#include <hatchetfish_stopwatch.hpp>

namespace SSPHH
{
	const std::string ssg_default_scene_path{ "resources/scenes/test_texture_scene/" };

	void SSPHH_Application::SSG_ReloadScene() {
		ssg.reset();
		Hf::StopWatch stopwatch;
		SSG_LoadScene();
		stopwatch.Stop();
		HFLOGINFO("SSG reload took %4.2f milliseconds", stopwatch.GetMillisecondsElapsed());
		Interface.lastScenegraphLoadTime = stopwatch.GetMillisecondsElapsed();
	}

	void SSPHH_Application::Sun_AdvanceClock(double numSeconds, bool recomputeSky) {
		pbsky_timeOffsetInSeconds += numSeconds;
		ssg.environment.pbsky.SetTime(pbsky_localtime, (float)pbsky_timeOffsetInSeconds);
		Interface.recomputeSky = true;
	}

	void SSPHH_Application::Sun_ResetClock() {
		ssg.environment.pbsky.SetCivilDateTime(ssg.environment.pbsky_dtg);
		ssg.environment.pbsky.ComputeSunFromLocale();
		pbsky_localtime = ssg.environment.pbsky.GetTime();
		pbsky_timeOffsetInSeconds = 0.0;
		Interface.recomputeSky = true;
		Sun_AdvanceClock(0.0, true);
		Sky_RegenHosekWilkieTextures();
	}

	void SSPHH_Application::Sun_UseCurrentTime() {
		pbsky_localtime = time(NULL);
		pbsky_timeOffsetInSeconds = 0.0;

		ssg.environment.pbsky.SetTime(time(NULL), 0.0);
		ssg.environment.pbsky.ComputeSunFromLocale();
		Interface.recomputeSky = true;
		Sky_RegenHosekWilkieTextures();
	}

	void SSPHH_Application::SSG_LoadScene() {
		if (Interface.uf.uf_type == UfType::Broker) {
			HFLOGINFO("configured to be a broker, so not loading scene");
			return;
		}

		FilePathInfo fpi(sceneFilename);
		Interface.sceneName = fpi.fname;
		ssg.Load(sceneFilename);
		//ssg.BuildBuffers();

		rendererContext.renderers["gles30CubeMap"].setSceneGraph(&ssg);
	}

	void SSPHH_Application::SSG_OptimizeClippingPlanes() {
		//Matrix4f cameraMatrix = defaultRenderConfig.preCameraMatrix * ssg.camera.actualViewMatrix * defaultRenderConfig.postCameraMatrix;
		//cameraMatrix.AsInverse().col4()
		Matrix4f cameraMatrix = ssg.camera.actualViewMatrix.AsInverse();
		const BoundingBoxf& bbox = ssg.GetBoundingBox();
		const Matrix4f& frameOfReference = cameraMatrix;
		const Vector3f& position = frameOfReference.col4().xyz();
		float znear;
		float zfar;

		float distanceToBoxCenter = (position - bbox.Center()).length() + 1.0f;
		float boxRadius = bbox.RadiusRounded();
		znear = std::max(0.1f, distanceToBoxCenter - boxRadius);
		zfar = distanceToBoxCenter + 2 * boxRadius; // min(1000.0f, distanceToBoxCenter + boxRadius);

		rendererContext.rendererConfigs["default"].znear = znear;
		rendererContext.rendererConfigs["default"].zfar = zfar;
		rendererContext.rendererConfigs["rectShadow"].znear = std::max(0.1f, ssg.environment.sunShadowMapNearZ);
		rendererContext.rendererConfigs["rectShadow"].zfar = std::min(1000.0f, ssg.environment.sunShadowMapFarZ);
	}

	void SSPHH_Application::SSG_ProcessInterfaceTasks() {
		if (Interface.ssg.saveScene) {
			Interface.ssg.saveScene = false;
			sceneFilename = ssg_default_scene_path;
			sceneFilename += Interface.ssg.scenename;
			ssg.Save(sceneFilename);
		}

		if (Interface.ssg.resetScene) {
			Interface.ssg.resetScene = false;
			ssg.reset();
		}

		if (Interface.ssg.createScene) {
			Interface.ssg.createScene = false;
		}

		if (Interface.ssg.loadScene) {
			Interface.ssg.loadScene = false;
			sceneFilename = ssg_default_scene_path;
			sceneFilename += Interface.ssg.scenename;
			SSG_ReloadScene();
		}
	}
}