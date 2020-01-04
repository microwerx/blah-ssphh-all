#include <ssphhapp.hpp>
#include <hatchetfish_stopwatch.hpp>

namespace SSPHH
{
	void SSPHH_Application::Sky_InitViewController() {
		vcPbsky = new PbskyViewController(this);

		//if (!rendererContext.textures["enviroSkyBox"].loadTextureCoronaCubeMap(default_coronaskyboxcubemap_path, true)) {
		//	HFLOGERROR("enviroSkyBoxTexture...could not load %s", default_coronaskyboxcubemap_path.c_str());
		//}
		//else {
		//	HFLOGINFO("enviroSkyBoxTexture...loaded %s", default_coronaskyboxcubemap_path.c_str());
		//}

		//TODO: Make sure pbsky texture is in renderconfig
		//RendererTextureObject& PBSkyCubeMap = rendererContext.textures["pbSkyBox"];
		//PBSkyCubeMap.init("pbskyCubeMap");
		//PBSkyCubeMap.setTextureCubeMap(GL_RGB, GL_FLOAT, 64, 64, nullptr, true);
		//PBSkyCubeMap.samplerObject.init("pbskyCubeMapSampler");
		//PBSkyCubeMap.samplerObject.setMinFilter(GL_LINEAR);
		//PBSkyCubeMap.samplerObject.setMagFilter(GL_LINEAR);
		//PBSkyCubeMap.samplerObject.setWrapSTR(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	}


	void SSPHH_Application::Sky_RegenHosekWilkieTextures() {
		ssg.environment.ComputePBSky();
	}

	void SSPHH_Application::Sky_SaveHosekWilkieTextures() {
		HFLOGINFO("Saving pbsky ppm texture maps");
		Hf::StopWatch stopwatch;
		ssg.environment.pbsky.generatedCylMap.savePPM(default_pbsky_cylmap_ppm);
		ssg.environment.pbsky.generatedCubeMap.savePPM(default_pbsky_cubemap1_ppm, 0);
		ssg.environment.pbsky.generatedCubeMap.savePPM(default_pbsky_cubemap2_ppm, 1);
		ssg.environment.pbsky.generatedCubeMap.savePPM(default_pbsky_cubemap3_ppm, 2);
		ssg.environment.pbsky.generatedCubeMap.savePPM(default_pbsky_cubemap4_ppm, 3);
		ssg.environment.pbsky.generatedCubeMap.savePPM(default_pbsky_cubemap5_ppm, 4);
		ssg.environment.pbsky.generatedCubeMap.savePPM(default_pbsky_cubemap6_ppm, 5);
		ssg.environment.pbsky.generatedCubeMap.saveCubePPM(default_pbsky_cubemap_ppm);
		stopwatch.Stop();
		HFLOGINFO("Saving pbsky ppm texture maps took %4.2f seconds", stopwatch.GetSecondsElapsed());
	}

	void SSPHH_Application::Sky_RegenCoronaSky() {
		Uf::CoronaJob job("ssphh_sky", Uf::CoronaJob::Type::Sky);

		if (Interface.ssphh.enableHQ)
			job.EnableHQ();
		if (Interface.ssphh.enableHDR)
			job.EnableHDR();

		job.Start(coronaScene, ssg);

		vcPbsky->coronaPath = job.GetOutputPath(vcPbsky->coronaLoadEXR);
		Sky_LoadCoronaSky(vcPbsky->coronaLoadEXR, vcPbsky->coronaPath);
	}


	void SSPHH_Application::Sky_LoadCoronaSky(bool loadEXR, const std::string& path) {
		FilePathInfo fpi(path);
		if (fpi.Exists()) {
			Image4f lightProbe;
			if (loadEXR) {
				lightProbe.loadEXR(fpi.path);
			}
			else {
				lightProbe.loadPPM(fpi.path);
				//lightProbe.scaleColors(1.0f / (2.5f * powf(2.0f, ssg.environment.toneMapExposure)));
				lightProbe.ReverseSRGB().ReverseToneMap(ssg.environment.toneMapExposure());
			}
			lightProbe.convertRectToCubeMap();
			//FxDebugBindTexture(GL_TEXTURE_CUBE_MAP, ssg.environment.pbskyColorMapId);
			//for (int i = 0; i < 6; i++) {
			//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, (GLsizei)lightProbe.width(), (GLsizei)lightProbe.height(), 0, GL_RGBA, GL_FLOAT, lightProbe.getImageData(i));
			//}
			//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			//FxDebugBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		else {
			HFLOGERROR("Could not generate %s", fpi.path.c_str());
		}
	}


	void SSPHH_Application::Sky_Render() {
		static GLfloat size = 500.0f;
		static GLfloat v[] = {
			-size, size, -size,
			size, size, -size,
			size, -size, -size,
			-size, -size, -size,
			size, size, size,
			-size, size, size,
			-size, -size, size,
			size, -size, size };

		static GLfloat texcoords[] = {
			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f };

		static GLfloat buffer[] = {
			-size, size, -size, -1.0f, 1.0f, -1.0f,
			size, size, -size, 1.0f, 1.0f, -1.0f,
			size, -size, -size, 1.0f, -1.0f, -1.0f,
			-size, -size, -size, -1.0f, -1.0f, -1.0f,
			size, size, size, 1.0f, 1.0f, 1.0f,
			-size, size, size, -1.0f, 1.0f, 1.0f,
			-size, -size, size, -1.0f, -1.0f, 1.0f,
			size, -size, size, 1.0f, -1.0f, 1.0f };

		static GLushort indices[] = {
			// FACE 0
			7, 4, 1,
			1, 2, 7,
			// FACE 1
			3, 0, 5,
			5, 6, 3,
			// FACE 2
			1, 4, 5,
			5, 0, 1,
			// FACE 3
			7, 2, 3,
			3, 6, 7,
			// FACE 5
			6, 5, 4,
			4, 7, 6,
			// FACE 4
			2, 1, 0,
			0, 3, 2 };

		static GLuint abo = 0;
		static GLuint eabo = 0;
		GLuint vbo = 0;
		GLint vloc = -1;
		GLint tloc = -1;
		GLuint program = 0;
		GLint uCubeTexture = -1;
		GLint uWorldMatrix = -1;
		GLint uCameraMatrix = -1;
		GLint uProjectionMatrix = -1;
		GLint uSunE0 = -1;
		GLint uSunDirTo = -1;
		GLint uGroundE0 = -1;
		GLint uIsHemisphere = -1;
		GLint uToneMapScale = -1;
		GLint uToneMapExposure = -1;
		GLint uToneMapGamma = -1;

		// FIXME: Are we using rendererContext?
		RendererProgramPtr p;// = rendererContext.FindProgram("skybox", "skybox");
		if (p) {
			program = p->getProgram();
			uCubeTexture = p->getUniformLocation("uCubeTexture");
			uWorldMatrix = p->getUniformLocation("WorldMatrix");
			uCameraMatrix = p->getUniformLocation("CameraMatrix");
			uProjectionMatrix = p->getUniformLocation("ProjectionMatrix");
			uSunE0 = p->getUniformLocation("SunE0");
			uGroundE0 = p->getUniformLocation("GroundE0");
			uSunDirTo = p->getUniformLocation("SunDirTo");
			uIsHemisphere = p->getUniformLocation("IsHemisphere");
			uToneMapScale = p->getUniformLocation("ToneMapScale");
			uToneMapExposure = p->getUniformLocation("ToneMapExposure");
			uToneMapGamma = p->getUniformLocation("ToneMapGamma");
		}

		if (program == 0)
			return;

		vloc = glGetAttribLocation(program, "aPosition");
		tloc = glGetAttribLocation(program, "aTexCoord");

		if (abo == 0) {
			glGenBuffers(1, &abo);
			glGenBuffers(1, &eabo);
			glBindBuffer(GL_ARRAY_BUFFER, abo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}

		glUseProgram(program);
		if (uToneMapScale >= 0)
			glUniform1f(uToneMapScale, 2.5f * powf(2.0f, ssg.environment.toneMapExposure()));
		if (uToneMapExposure >= 0)
			glUniform1f(uToneMapExposure, 2.5f * powf(2.0f, ssg.environment.toneMapExposure()));
		if (uToneMapGamma >= 0)
			glUniform1f(uToneMapGamma, ssg.environment.toneMapGamma());
		if (uCubeTexture >= 0) {
			//FxBindTextureAndSampler(ssg.environment.pbskyColorMapUnit, GL_TEXTURE_CUBE_MAP, ssg.environment.pbskyColorMapId, ssg.environment.pbskyColorMapSamplerId);
			//glUniform1i(uCubeTexture, ssg.environment.pbskyColorMapUnit);
		}
		if (uProjectionMatrix >= 0) {
			Matrix4f projectionMatrix = ssg.camera.projectionMatrix;
			glUniformMatrix4fv(uProjectionMatrix, 1, GL_FALSE, projectionMatrix.const_ptr());
		}
		if (uCameraMatrix >= 0) {
			Matrix4f viewMatrix = Interface.inversePreCameraMatrix * ssg.camera.viewMatrix;
			viewMatrix.m14 = viewMatrix.m24 = viewMatrix.m34 = viewMatrix.m41 = viewMatrix.m42 = viewMatrix.m43 = 0.0f;
			glUniformMatrix4fv(uCameraMatrix, 1, GL_FALSE, viewMatrix.const_ptr());
		}
		if (uWorldMatrix >= 0) {
			Matrix4f worldMatrix;
			glUniformMatrix4fv(uWorldMatrix, 1, GL_FALSE, worldMatrix.const_ptr());
		}
		if (uSunDirTo >= 0)
			glUniform3fv(uSunDirTo, 1, ssg.environment.curSunDirTo.const_ptr());
		if (uSunE0 >= 0)
			glUniform4fv(uSunE0, 1, ssg.environment.pbsky.GetSunDiskRadiance().const_ptr());
		if (uGroundE0 >= 0)
			glUniform4fv(uGroundE0, 1, ssg.environment.pbsky.GetGroundRadiance().const_ptr());
		if (uIsHemisphere >= 0)
			glUniform1i(uIsHemisphere, ssg.environment.isHemisphere);

		glBindBuffer(GL_ARRAY_BUFFER, abo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
		glVertexAttribPointer(vloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void*)0);
		glVertexAttribPointer(tloc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void*)12);
		if (vloc >= 0)
			glEnableVertexAttribArray(vloc);
		if (tloc >= 0)
			glEnableVertexAttribArray(tloc);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		if (vloc >= 0)
			glDisableVertexAttribArray(vloc);
		if (tloc >= 0)
			glDisableVertexAttribArray(tloc);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}
}