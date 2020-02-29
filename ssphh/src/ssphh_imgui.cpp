#include "pch.hpp"
#include <fluxions_ssphh.hpp>
#include <fluxions_ssphh_utilities.hpp>
#include <ssphhapp.hpp>
#include <unicornfish_corona_job.hpp>
#include <unicornfish_corona_scene_file.hpp>
#include <viperfish_dear_imgui.hpp>

using namespace Fluxions;

static const int AllBands = -100;
static const int AllDegrees = -1;

// static float imguiWinX = 64.0f;
// static float imguiWinW = 384.0f;

#ifdef __APPLE__
#define __unix__
#endif

#ifdef __unix__
auto DeleteFile = unlink;
#endif

Uf::CoronaSceneFile coronaScene;

// struct COLORS
//{
//	ImVec4 Red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
//	ImVec4 Green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
//	ImVec4 Blue = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
//	ImVec4 Yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
//	ImVec4 Cyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
//	ImVec4 Magenta = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
//	ImVec4 Black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
//	ImVec4 White = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
//} Colors;

static const std::string DebugShaderChoices[21] = { " 0 - Default",
													" 1 - Kd - Material Diffuse Color",
													" 2 - Ks - Material Specular Color",
													" 3 - N - Material Normal Map",
													" 4 - V - View Vector",
													" 5 - R - Reflection Vector",
													" 6 - L - Light Vector",
													" 7 - H - Half Vector",
													" 8 - N.L - I/pi * N.L",
													" 9 - Mobile BRDF 1",
													"10 - Mobile BRDF 2 + c_e",
													"11 - BRDF Diffuse Only",
													"12 - BRDF Specular Only",
													"13 - BRDF Environment Only",
													"14 - BRDF in toto",
													"15 - BRDF in toto + c_e",
													"16 - BRDF in toto + c_e + c_a",
													"17 - Closest SPHL only + Sun",
													"18 - N.L + shadow",
													"19 - ",
													"20 - " };

#define NEWLINE_SEPARATOR                                                                                              \
	ImGui::Separator();                                                                                                \
	ImGui::NewLine();
#define SEPARATOR_NEWLINE                                                                                              \
	ImGui::NewLine();                                                                                                  \
	ImGui::Separator();

namespace SSPHH {
	void SSPHH_Application::InitImGui() {
		// ImGuiIO& io = ImGui::GetIO();
		// io.Fonts->AddFontDefault();
		// io.Fonts->AddFontFromFileTTF("resources/fonts/dock-medium.otf", 16.0f);
	}

	void SSPHH_Application::RenderImGuiHUD() {
		HFLOGDEBUGFIRSTRUN();

		imguiWinX = 64.0f;
		imguiWinW = 384.0f;

		imguiShowMenuBar();
		imguiShowToolWindow();
		imguiShowMaterialEditor();
		imguiShowScenegraphEditor();
		imguiShowSphlEditor();
		imguiShowUfWindow();
		imguiShowSSPHHWindow();
		imguiShowRenderConfigWindow();
		imguiShowTestWindow();
		vcPbsky->show();

		// ImGui::ShowUserGuide();
	}

	void SSPHH_Application::imguiShowMenuBar() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				ImGui::MenuItem("(dummy menu)", NULL, false, false);
				if (ImGui::MenuItem("New")) {
				}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {
				}
				if (ImGui::BeginMenu("Open Recent")) {
					ImGui::MenuItem("fish_hat.c");
					ImGui::MenuItem("fish_hat.inl");
					ImGui::MenuItem("fish_hat.h");
					if (ImGui::BeginMenu("More..")) {
						ImGui::MenuItem("Hello");
						ImGui::MenuItem("Sailor");
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
				}
				if (ImGui::MenuItem("Save As..")) {
				}
				ImGui::Separator();
				if (ImGui::BeginMenu("Options")) {
					static bool enabled = true;
					ImGui::MenuItem("Enabled", "", &enabled);
					ImGui::BeginChild("child", ImVec2(0, 60), true);
					for (int i = 0; i < 10; i++)
						ImGui::Text("Scrolling Text %d", i);
					ImGui::EndChild();
					static float f = 0.5f;
					static int n = 0;
					ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
					ImGui::InputFloat("Input", &f, 0.1f);
					ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Colors")) {
					for (int i = 0; i < ImGuiCol_COUNT; i++)
						ImGui::MenuItem(ImGui::GetStyleColorName((ImGuiCol)i));
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Disabled", false)) // Disabled
				{
					IM_ASSERT(0);
				}
				if (ImGui::MenuItem("Checked", NULL, true)) {
				}
				if (ImGui::MenuItem("Quit", "Alt+F4")) {
					LeaveMainLoop();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {
				}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {
				} // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {
				}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {
				}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools")) {
				if (ImGui::MenuItem("Scenegraph Editor")) {
					Interface.tools.showScenegraphEditor = true;
				}
				if (ImGui::MenuItem("Material Editor")) {
					Interface.tools.showMaterialEditor = true;
				}
				if (ImGui::MenuItem("SSPL Editor")) {
					Interface.tools.showSphlEditor = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void SSPHH_Application::imguiShowToolWindow() {
		///////////////////////////////////////////////////////////
		// T O O L   W I N D O W //////////////////////////////////
		///////////////////////////////////////////////////////////

		// ImGui::SetNextWindowSize(ImVec2(320, 200));
		ImGui::SetNextWindowContentSize({ imguiWinW, -1 });
		ImGui::SetNextWindowPos(ImVec2(imguiWinX, 64));
		ImGui::Begin("Tool Window");
		ImGui::PushID("ToolWindow");
		if (ImGui::Button("Hide GUI")) {
			Interface.showImGui = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Quit")) {
			LeaveMainLoop();
		}

		float time_ms = (float)(1000.0 / framesPerSecond);
		ImGui::SameLine();
		ImGui::Text("[ %3.1f fps/ %3.1f ms ]", framesPerSecond, time_ms);
		if (ImGui::SmallButton("Save Stats")) {
			imguiToolsSaveStats();
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Reset Stats")) {
			imguiToolsResetStats();
		}

		if (ImGui::SmallButton("Screenshot")) {
			imguiToolsTakeScreenshot();
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Shadows")) {
			Interface.captureShadows = true;
		}

		ImGui::Checkbox("Scenegraph Editor", &Interface.tools.showScenegraphEditor);
		ImGui::Checkbox("Material Editor", &Interface.tools.showMaterialEditor);
		ImGui::Checkbox("Unicornfish", &Interface.tools.showUnicornfishWindow);
		ImGui::Checkbox("SSPHH", &Interface.tools.showSSPHHWindow);
		ImGui::Checkbox("Render Config", &Interface.tools.showRenderConfigWindow);
		ImGui::Checkbox("Tests", &Interface.tools.showTestsWindow);
		ImGui::SameLine();
		ImGui::Checkbox("Debug view", &(rendererContext->rendererConfigs["default"]->enableDebugView));
		ImGui::Checkbox("Sphl Editor", &Interface.tools.showSphlEditor);
		ImGui::PushID(1234);
		ImGui::SameLine();
		if (ImGui::SmallButton("+")) {
			imguiSphlAdd();
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("-")) {
			imguiSphlDelete();
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("l-")) {
			imguiSphlDecreaseDegrees();
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("l+")) {
			imguiSphlIncreaseDegrees();
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("?")) {
			imguiSphlRandomize();
		}
		if (ImGui::SmallButton("Corona->SPH")) {
			if (!ssgUserData->ssphhLights.empty()) {
				imguiSphlCoronaToSPH(ssgUserData->ssphhLights.size() - 1);
			}
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("SPH->CubeMap")) {
			if (!ssgUserData->ssphhLights.empty()) {
				imguiSphlSPHtoCubeMap(ssgUserData->ssphhLights.size() - 1);
			}
		}
		ImGui::PopID();

		ImGui::Separator();

		ImGui::DragFloat("Exposure", &ssg->environment.toneMapExposure(), 0.1f, -12.0f, 12.0f, "%.1f");
		ImGui::DragFloat("Gamma", &ssg->environment.toneMapGamma(), 0.1f, 1.0f, 6.0f, "%.1f");

		ImGui::Separator();

		ImGui::Text("%s", DebugShaderChoices[Interface.tools.shaderDebugChoice].c_str());
		ImGui::Text("Shader Debug Choice: %02d", Interface.tools.shaderDebugChoice);
		ImGui::SameLine();
		if (ImGui::SmallButton("+")) {
			Interface.tools.shaderDebugChoice++;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("-")) {
			Interface.tools.shaderDebugChoice--;
		}
		Interface.tools.shaderDebugChoice = Fluxions::clamp<int>(Interface.tools.shaderDebugChoice, 0, 20);

		ImGui::DragInt("Debug Light", &Interface.tools.shaderDebugLight, 0.1f, -1, 16);
		ImGui::DragInt("Debug SPHL ", &Interface.tools.shaderDebugSphl, 0.1f, -1, 16);

		ImGui::SameLine();
		if (ImGui::SmallButton("Same")) {
			for (int i = 0; i < 4; i++) {
				Interface.tools.shaderDebugChoices[i] = Interface.tools.shaderDebugChoice;
			}
		}

		for (int i = 0; i < 4; i++) {
			ImGui::PushID(i);
			ImGui::Text("Shader %d / % 2d", i, Interface.tools.shaderDebugChoices[i]);
			ImGui::SameLine();
			if (ImGui::SmallButton("+")) {
				Interface.tools.shaderDebugChoices[i]++;
			}
			ImGui::SameLine();
			if (ImGui::SmallButton("-")) {
				Interface.tools.shaderDebugChoices[i]--;
			}
			Interface.tools.shaderDebugChoices[i] = Fluxions::clamp<int>(Interface.tools.shaderDebugChoices[i], 0, 20);
			ImGui::PopID();
		}

		ImGui::Separator();
		ImGui::Text(
			"CAMERA POS: % 3.2f/% 3.2f/ % 3.2f",
			Interface.cameraPosition.x,
			Interface.cameraPosition.y,
			Interface.cameraPosition.z);
		ImGui::Text(
			"CAMERA POS: % 3.2f/% 3.2f/ % 3.2f",
			rendererContext->rendererConfigs["default"]->cameraPosition.x,
			rendererContext->rendererConfigs["default"]->cameraPosition.y,
			rendererContext->rendererConfigs["default"]->cameraPosition.z);
		if (ImGui::SmallButton("LoadIdentity()")) {
			Interface.preCameraMatrix.LoadIdentity();
		}
		if (ImGui::SmallButton("+X")) {
			Interface.preCameraMatrix.Translate(1.0f, 0.0f, 0.0f);
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("-X")) {
			Interface.preCameraMatrix.Translate(-1.0f, 0.0f, 0.0f);
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("+Y")) {
			Interface.preCameraMatrix.Translate(0.0f, 1.0f, 0.0f);
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("-Y")) {
			Interface.preCameraMatrix.Translate(0.0f, -1.0f, 0.0f);
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("+Z")) {
			Interface.preCameraMatrix.Translate(0.0f, 0.0f, 1.0f);
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("-Z")) {
			Interface.preCameraMatrix.Translate(0.0f, 0.0f, -1.0f);
		}
		if (ImGui::SmallButton("Reset Orbit")) {
			Interface.cameraOrbit.reset();
		}
		ImGui::SameLine();
		ImGui::Checkbox("Enable Orbit", &Interface.enableOrbit);
		if (ImGui::SmallButton("-Az")) {
			Interface.cameraOrbit.x -= 5.0f;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("+Az")) {
			Interface.cameraOrbit.x += 5.0f;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("-Alt")) {
			Interface.cameraOrbit.y -= 5.0f;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("+Alt")) {
			Interface.cameraOrbit.y += 5.0f;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("-R")) {
			Interface.cameraOrbit.z -= 1.0f;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("+R")) {
			Interface.cameraOrbit.z += 1.0f;
		}

		ImGui::TextColored(Colors::Yellow, "Sun: ");
		ImGui::TextColored(
			Colors::Yellow,
			"%0.4fi %0.4fj %0.4fk",
			ssg->environment.curSunDirTo.x,
			ssg->environment.curSunDirTo.y,
			ssg->environment.curSunDirTo.z);
		// ImGui::TextColored(Colors::Yellow, "Shadow map time: %.3fms", ssg->environment.sunShadowMapTime);
		ImGui::TextColored(
			Colors::Yellow,
			"Sun: %3.1f %3.1f %3.1f",
			ssg->environment.curSunDiskRadiance.r,
			ssg->environment.curSunDiskRadiance.g,
			ssg->environment.curSunDiskRadiance.b);
		ImGui::TextColored(
			Colors::Red,
			"Gnd: %3.1f %3.1f %3.1f",
			ssg->environment.curGroundRadiance.r,
			ssg->environment.curGroundRadiance.g,
			ssg->environment.curGroundRadiance.b);
		ImGui::Checkbox("Sun Cycle", &Interface.enableSunCycle);
		ImGui::SameLine();
		if (ImGui::SmallButton("+ 1hr")) {
			Sun_AdvanceClock(3600.0, true);
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("- 1hr")) {
			Sun_AdvanceClock(-3600.0, true);
		}

		ImGui::Separator();

		ImGui::Text("Debug Viz %d", counter);
		ImGui::SameLine();
		if (ImGui::SmallButton("Disable Debug Viz"))
			counter = -1;
		if (ImGui::Button("Show SPHs")) {
			if (counter != 0) {
				counter = 0;
			} else {
				counter = -1;
			}
		}
		if (ImGui::Button("Show Sun Shadow Map")) {
			if (counter != 1) {
				counter = 1;
			} else {
				counter = -1;
			}
		}
		if (ImGui::Button("Show Sphere Cube Map")) {

			if (counter != 2) {
				counter = 2;
			} else {
				counter = -1;
			}
		}
		if (ImGui::Button("Show Enviro Cube Map")) {
			if (counter != 3) {
				counter = 3;
			} else {
				counter = -1;
			}
		}

		ImGui::Separator();

		ImGui::Checkbox("GL Info", &Interface.tools.showGLInfo);
		if (Interface.tools.showGLInfo) {
			if (ImGui::Button("GL Extensions") || Interface.tools.gl_extensions.empty()) {
				Interface.tools.gl_extensions.clear();
				Interface.tools.gl_extensions_cstr.clear();
				std::string extensions = (const char*)glGetString(GL_EXTENSIONS);
				std::regex my_regex("\\w+");
				std::sregex_iterator begin = std::sregex_iterator(extensions.begin(), extensions.end(), my_regex);
				std::sregex_iterator end = std::sregex_iterator();
				for (auto it = begin; it != end; it++) {
					std::smatch match = *it;
					if (match.str().size() < 3)
						continue;
					Interface.tools.gl_extensions.push_back(match.str());
				}

				sort(Interface.tools.gl_extensions.begin(), Interface.tools.gl_extensions.end());

				for (auto& s : Interface.tools.gl_extensions) {
					Interface.tools.gl_extensions_cstr.push_back(s.c_str());
				}
				Interface.tools.gl_version = (const char*)glGetString(GL_VERSION);
				Interface.tools.gl_renderer = (const char*)glGetString(GL_RENDERER);
			}
			ImGui::Text("GL_RENDERER: %s", Interface.tools.gl_renderer.c_str());
			ImGui::Text("GL_VERSION:  %s", Interface.tools.gl_version.c_str());
			ImGui::Text("GL_EXTENSIONS: %d", (int)Interface.tools.gl_extensions.size());
			if (!Interface.tools.gl_extensions.empty()) {
				// ImGui::SetNext
				ImGui::PushItemWidth(imguiWinW - 16);
				ImGui::ListBox(
					"##empty",
					&Interface.tools.gl_extensions_curitem,
					&Interface.tools.gl_extensions_cstr[0],
					(int)Interface.tools.gl_extensions.size(),
					16);
				ImGui::PopItemWidth();
			}
		}

		ImGui::PopID();
		ImGui::End();
	}

	void SSPHH_Application::imguiShowRenderConfigWindow() {
		if (!Interface.tools.showRenderConfigWindow) {
			return;
		}
		ImGui::SetNextWindowPos(ImVec2(imguiWinX + imguiWinW + 32, 64));
		ImGui::Begin("Render Configuration");
		auto& rc = rendererContext->rendererConfigs["default"];
		ImGui::Checkbox("Debug view", &rc->enableDebugView);
		ImGui::Checkbox("Depth Test", &rc->enableDepthTest);
		ImGui::Checkbox("Blend", &rc->enableBlend);
		ImGui::Checkbox("Cull Face", &rc->enableCullFace);
		ImGui::Checkbox("sRGB Framebuffer", &rc->enableSRGB);
		ImGui::Checkbox("Shadow color buffer", &rc->enableZColorBuffer);
		ImGui::Separator();
		ImGui::Text("ZNEAR: % .3f / ZFAR % .3f", rc->viewportZNear, rc->viewportZFar);

		// TODO: Fix DirToLight Shadows
		// ImGui::Separator();
		// ImGui::Text("Sun Shadows");
		// ImGui::Text("ZNEAR: % .3f / ZFAR % .3f",
		//			rendererContext.rendererConfigs["rectShadow"].viewportZNear,
		//			rendererContext.rendererConfigs["rectShadow"].viewportZFar);
		// ImGui::SliderInt("2^N", &Interface.renderconfig.sunShadowMapSizeChoice, 6, 12);
		// ImGui::SameLine();
		// ImGui::Text("= %d", 2 << Interface.renderconfig.sunShadowMapSizeChoice);
		// Interface.renderconfig.sunShadowMapSize = 2 << Interface.renderconfig.sunShadowMapSizeChoice;
		// ImGui::SliderFloat("Shadow Map Zoom", &ssg.environment.sunShadowMapZoom, 0.1f, 2.0f);
		// Vector2f before = ssg->environment.sunShadowMapOffset;
		// ImGui::SliderFloat2("Offset", ssg->environment.sunShadowMapOffset.ptr(), -20.0f, 20.0f);
		// if (before != ssg->environment.sunShadowMapOffset) {
		//	ssg.environment.Update(ssg.getBoundingBox());
		//}
		// ImGui::SameLine();
		// if (ImGui::Button("0")) {
		//	ssg.environment.sunShadowMapZoom = 1.0;
		//	ssg.environment.sunShadowMapOffset.reset();
		//}
		// ImGui::SameLine();
		// if (ImGui::Button("!")) {
		//	ssg.environment.Update(ssg.getBoundingBox());
		//}
		// ImGui::Checkbox("Shadow Cull Face", &rendererContext.rendererConfigs["rectShadow"].enableCullFace);
		// static bool frontOrBack = true;
		// ImGui::Checkbox("Shadow Cull Front/Back", &frontOrBack);
		// if (frontOrBack)
		//	rendererContext.rendererConfigs["rectShadow"].cullFaceMode = GL_FRONT;
		// else
		//	rendererContext.rendererConfigs["rectShadow"].cullFaceMode = GL_BACK;
		// ImGui::SameLine();
		// ImGui::Text("%s", frontOrBack ? "Front" : "Back");

		ImGui::Separator();

		for (size_t i = 0; i < sphls.size(); i++) {
			const GLuint* texid = sphls[i].lightProbeTexIds;
			ImGui::Text("sphls[%d] light probes %d -- %d -- %d", (int)i, (int)texid[0], (int)texid[1], (int)texid[2]);
		}

		ImGui::End();
	}

	void SSPHH_Application::imguiShowScenegraphEditor() {
		///////////////////////////////////////////////////////////
		// S C E N E G R A P H   E D I T O R //////////////////////
		///////////////////////////////////////////////////////////

		if (Interface.tools.showScenegraphEditor) {
			imguiWinX += imguiWinW + 64.0f;
			ImGui::SetNextWindowContentSize({ imguiWinW * 2, -1 });
			ImGui::SetNextWindowPos(ImVec2(imguiWinX, 64));
			imguiWinX += imguiWinW;
			ImGui::Begin("Scenegraph");
			static int sceneNumber = 0;
			static char scenenameBuffer[128];
			static const char* scenes = "test_indoor_scene\0test_outside_scene\0test_mitsuba_scene\0\0";
			// static const char *scenes = "indoor\0outside\0mitsuba\0\0";
			int lastSceneNumber = sceneNumber;
			ImGui::Combo("Scene", &sceneNumber, scenes);

			if (ImGui::Button("Reset")) {
				Interface.ssg.resetScene = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Save")) {
				Interface.ssg.saveScene = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Load")) {
				Interface.ssg.loadScene = true;
			}
			if (lastSceneNumber != sceneNumber) {
				switch (sceneNumber) {
				case 0:
					Interface.ssg.scenename = "test_indoor_scene.scn";
					break;
				case 1:
					Interface.ssg.scenename += "test_outdoor_scene.scn";
					break;
				case 2:
					Interface.ssg.scenename += "test_mitsuba_scene.scn";
					break;
				default:
					sceneFilename += "simple_inside_scene.scn";
					break;
				}
			}
			auto safeCopy = [](char* dest, std::string& input, size_t maxChars) {
				size_t size = std::min(input.size(), maxChars - 1);
				memcpy(dest, input.data(), size);
			};
			safeCopy(scenenameBuffer, Interface.ssg.scenename, sizeof(scenenameBuffer));
			ImGui::InputText("Scene", scenenameBuffer, sizeof(scenenameBuffer));
			Interface.ssg.scenename = scenenameBuffer;

			ImGui::Text("Load time: %3.2f msec", Interface.lastScenegraphLoadTime);
			if (ImGui::Button("Reload Render Config")) {
				ReloadRenderConfigs();
			}
			ImGui::Text("Load time: %3.2f msec", Interface.lastRenderConfigLoadTime);
			BoundingBoxf sceneBBox = ssg->getBoundingBox();
			Vector3f sceneMin(
				trunc(sceneBBox.minBounds.x - 0.5f),
				trunc(sceneBBox.minBounds.x - 0.5f),
				trunc(sceneBBox.minBounds.z - 0.5f));
			Vector3f sceneMax(
				trunc(sceneBBox.maxBounds.x + 0.5f),
				trunc(sceneBBox.maxBounds.y + 0.5f),
				trunc(sceneBBox.maxBounds.z + 0.5f));
			Vector3f sceneSize(trunc(sceneBBox.X() + 0.5f), trunc(sceneBBox.Y() + 0.5f), trunc(sceneBBox.Z() + 0.5f));
			ImGui::Text(
				"scene dimensions (%.1f %.1f %.1f)/(%.1f, %.1f, %.1f) [%.1f, %.1f, %.1f]",
				sceneMin.x,
				sceneMin.y,
				sceneMin.z,
				sceneMax.x,
				sceneMax.y,
				sceneMax.z,
				sceneSize.x,
				sceneSize.y,
				sceneSize.z);

			ImGui::Checkbox("Environment", &Interface.ssg.showEnvironment);
			if (Interface.ssg.showEnvironment) {
				// ImGui::Checkbox("Environment Details", &Interface.ssg.showEnvironmentDetails);
				// if (Interface.ssg.showEnvironmentDetails) {
				//	NEWLINE_SEPARATOR
				//	ImGui::Text("sunColorMap      Unit/Id/SamplerId: %2d/%2d/%2d", ssg->environment.sunColorMapUnit,
				// ssg->environment.sunColorMapId, ssg->environment.sunColorMapSamplerId); 	ImGui::Text("sunDepthMap
				// Unit/Id/SamplerId: %2d/%2d/%2d", ssg->environment.sunDepthMapUnit, ssg->environment.sunDepthMapId,
				// ssg->environment.sunDepthMapSamplerId); 	ImGui::Text("enviroColorMap   Unit/Id/SamplerId:
				// %2d/%2d/%2d", ssg->environment.enviroColorMapUnit, ssg->environment.enviroColorMapId,
				// ssg->environment.enviroColorMapSamplerId); 	ImGui::Text("enviroDepthMap   Unit/Id/SamplerId:
				//%2d/%2d/%2d", ssg->environment.enviroDepthMapUnit, ssg->environment.enviroDepthMapId,
				// ssg->environment.enviroDepthMapSamplerId); 	ImGui::Text("pbskyColorMap    Unit/Id/SamplerId:
				//%2d/%2d/%2d", ssg->environment.pbskyColorMapUnit, ssg->environment.pbskyColorMapId,
				// ssg->environment.pbskyColorMapSamplerId); 	ImGui::Text("shadow map: znear: %.1f, zfar: %0.1f,
				// origin(%.1f, %.1f, %.1f), target(%.1f, %.1f, %.1f), up(%.2f, %.2f, %.2f)",
				//				ssg.environment.sunShadowMapNearZ,
				//				ssg.environment.sunShadowMapFarZ,
				//				ssg.environment.sunShadowMapOrigin.x, ssg->environment.sunShadowMapOrigin.y,
				// ssg->environment.sunShadowMapOrigin.z, 				ssg.environment.sunShadowMapTarget.x,
				// ssg->environment.sunShadowMapTarget.y, ssg->environment.sunShadowMapTarget.z,
				//				ssg.environment.sunShadowMapUp.x, ssg->environment.sunShadowMapUp.y,
				// ssg->environment.sunShadowMapUp.z); 	SEPARATOR_NEWLINE
				//}

				ImGui::TextColored(Colors::Red, "Camera: ");
				ImGui::DragFloat("FOV: ", &Interface.ssg.cameraFOV);
				Matrix4f m = ssg->camera.viewMatrix();
				ImGui::Text(
					"viewMatrix: % 3.2f % 3.2f % 3.2f % 3.2f\n"
					"            % 3.2f % 3.2f % 3.2f % 3.2f\n"
					"            % 3.2f % 3.2f % 3.2f % 3.2f\n"
					"            % 3.2f % 3.2f % 3.2f % 3.2f",
					m.m11,
					m.m12,
					m.m13,
					m.m14,
					m.m21,
					m.m22,
					m.m23,
					m.m24,
					m.m31,
					m.m32,
					m.m33,
					m.m34,
					m.m41,
					m.m42,
					m.m43,
					m.m44);
				m = Interface.preCameraMatrix;
				ImGui::Text(
					"altMatrix:  % 3.2f % 3.2f % 3.2f % 3.2f\n"
					"            % 3.2f % 3.2f % 3.2f % 3.2f\n"
					"            % 3.2f % 3.2f % 3.2f % 3.2f\n"
					"            % 3.2f % 3.2f % 3.2f % 3.2f",
					m.m11,
					m.m12,
					m.m13,
					m.m14,
					m.m21,
					m.m22,
					m.m23,
					m.m24,
					m.m31,
					m.m32,
					m.m33,
					m.m34,
					m.m41,
					m.m42,
					m.m43,
					m.m44);
				Vector3f O = ssg->camera.origin();
				ImGui::Text("CAMERA POS: % 3.2f/% 3.2f/ % 3.2f", O.x, O.y, O.z);
				if (ImGui::SmallButton("LoadIdentity()")) {
					Interface.preCameraMatrix.LoadIdentity();
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("+X")) {
					Interface.preCameraMatrix.Translate(1.0f, 0.0f, 0.0f);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("-X")) {
					Interface.preCameraMatrix.Translate(-1.0f, 0.0f, 0.0f);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("+Y")) {
					Interface.preCameraMatrix.Translate(0.0f, 1.0f, 0.0f);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("-Y")) {
					Interface.preCameraMatrix.Translate(0.0f, -1.0f, 0.0f);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("+Z")) {
					Interface.preCameraMatrix.Translate(0.0f, 0.0f, 1.0f);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("-Z")) {
					Interface.preCameraMatrix.Translate(0.0f, 0.0f, -1.0f);
				}
				ImGui::Text("Orbit");
				ImGui::SameLine();
				if (ImGui::SmallButton("Reset Orbit")) {
					Interface.cameraOrbit.reset();
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("-Az")) {
					Interface.cameraOrbit.x -= 5.0f;
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("+Az")) {
					Interface.cameraOrbit.x += 5.0f;
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("-Alt")) {
					Interface.cameraOrbit.y -= 5.0f;
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("+Alt")) {
					Interface.cameraOrbit.y += 5.0f;
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("-R")) {
					Interface.cameraOrbit.z -= 1.0f;
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("+R")) {
					Interface.cameraOrbit.z += 1.0f;
				}

				ImGui::Text(
					"WxH: %4.fx%4.f / FOV: %3.2f",
					ssg->camera.imageWidth,
					ssg->camera.imageHeight,
					ssg->camera.fov);
				ImGui::Text("ZNEAR: %1.4f / ZFAR: %4.1f", ssg->camera.imageNearZ, ssg->camera.imageFarZ);
				NEWLINE_SEPARATOR

				ImGui::TextColored(Colors::Yellow, "Sun: ");
				ImGui::SameLine();
				if (ImGui::SmallButton("+ 1hr")) {
					Sun_AdvanceClock(3600.0, true);
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("- 1hr")) {
					Sun_AdvanceClock(-3600.0, true);
				}

				double turbidity = ssg->environment.pbsky.GetTurbidity();
				double lastTurbidity = turbidity;
				ImGui::Text("Turbidity: % 2.1f", turbidity);
				ImGui::SameLine();
				if (ImGui::SmallButton("+")) {
					turbidity += 1.0;
				}
				ImGui::SameLine();
				if (ImGui::SmallButton("-")) {
					turbidity -= 1.0;
				}
				turbidity = Fluxions::clamp<double>(turbidity, 1.0, 10.0);
				ssg->environment.pbsky.SetTurbidity((float)turbidity);
				if (turbidity != lastTurbidity) {
					ssg->environment.pbsky.computeAstroFromLocale();
					ssg->environment.ComputePBSky();
				}

				ImGui::Text(
					"DATE: %02d/%02d/%04d %02d:%02d:%02d%.3f -- LST : %2.3f",
					ssg->environment.pbsky.getMonth(),
					ssg->environment.pbsky.getDay(),
					ssg->environment.pbsky.getYear(),
					ssg->environment.pbsky.getHour(),
					ssg->environment.pbsky.getMin(),
					ssg->environment.pbsky.getSec(),
					ssg->environment.pbsky.getSecFract(),
					ssg->environment.pbsky.getLST());
				ImGui::Text(
					"LAT: % 2.2f LONG: % 3.2f",
					ssg->environment.pbsky.GetLatitude(),
					ssg->environment.pbsky.GetLongitude());
				NEWLINE_SEPARATOR
			}

			ImGui::Checkbox("Geometry", &Interface.ssg.showGeometry);
			if (Interface.ssg.showGeometry) {
				NEWLINE_SEPARATOR
				if (Interface.ssg.geometryCollapsed.size() != ssg->geometryGroups.size()) {
					Interface.ssg.geometryCollapsed.resize(ssg->geometryGroups.size());
				}

				int i = 0;
				for (auto& [id, group] : ssg->geometryGroups) {
					std::ostringstream label;
					label << group.name_str() << "(" << i << ")";
					ImGui::Checkbox(label.str().c_str(), &Interface.ssg.geometryCollapsed[i].second);
					if (Interface.ssg.geometryCollapsed[i].second) {
						ImGui::Text("name:     %s", group.name());
						ImGui::Text("filename: %s", group.fpi.stem().c_str());
						// ImGui::Text("mtllib:   %s", group.mtllibName.c_str());
						BoundingBoxf& bbox = group.bbox;
						Vector3f meshSceneMin = bbox.minBounds;
						Vector3f meshSceneMax = bbox.maxBounds;
						Vector3f meshSceneSize = bbox.Size();
						ImGui::Text(
							"dimensions (%.1f %.1f %.1f)/(%.1f, %.1f, %.1f) [%.1f, %.1f, %.1f]",
							meshSceneMin.x,
							meshSceneMin.y,
							meshSceneMin.z,
							meshSceneMax.x,
							meshSceneMax.y,
							meshSceneMax.z,
							meshSceneSize.x,
							meshSceneSize.y,
							meshSceneSize.z);
						// ImGui::Text("dimensions: %.2f/%.2f/%.2f", g.second.bbox.MaxX(), g.second.bbox.MaxY(),
						// g.second.bbox.MaxZ());

						imguiMatrix4fEditControl(i, group.transform);
					}
					i++;
				}
				SEPARATOR_NEWLINE
			}

			ImGui::Checkbox("Point Lights", &Interface.ssg.showPointLights);
			if (Interface.ssg.showPointLights) {
				int i = 0;
				for (auto& [k, spl] : ssg->pointLights) {
					std::ostringstream ostr;
					ImGui::Text("Name(%d): %s", i, spl.name());
					ImGui::ColorEdit4("E0", spl.E0.ptr());
					ImGui::DragFloat("Falloff Radius", &spl.position.w, 1.0f, 0.0f, 1000.0f);
					ImGui::DragFloat3("Position", spl.position.ptr(), 0.1f, -10.0f, 10.0f);
					i++;
				}
			}

			ImGui::End();
		}
	}

	void SSPHH_Application::imguiMatrix4fEditControl(int id, Matrix4f& m) {
		NEWLINE_SEPARATOR
		ImGui::PushID(id);
		ImGui::Text("Rotate: ");
		if (ImGui::Button("LoadIdentity()")) {
			m.LoadIdentity();
		}
		ImGui::Text("X: ");
		ImGui::SameLine();
		if (ImGui::Button("-5deg X")) {
			m.Rotate(-5.0f, 1.0f, 0.0f, 0.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("-1deg X")) {
			m.Rotate(-1.0f, 1.0f, 0.0f, 0.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("+1deg X")) {
			m.Rotate(1.0f, 1.0f, 0.0f, 0.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("+5deg X")) {
			m.Rotate(5.0f, 1.0f, 0.0f, 0.0f);
		}
		ImGui::Text("Y: ");
		ImGui::SameLine();
		if (ImGui::Button("-5deg Y")) {
			m.Rotate(-5.0f, 0.0f, 1.0f, 0.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("-1deg Y")) {
			m.Rotate(-1.0f, 0.0f, 1.0f, 0.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("+1deg Y")) {
			m.Rotate(1.0f, 0.0f, 1.0f, 0.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("+5deg Y")) {
			m.Rotate(5.0f, 0.0f, 1.0f, 0.0f);
		}
		ImGui::Text("Z: ");
		ImGui::SameLine();
		if (ImGui::Button("-5deg Z")) {
			m.Rotate(-5.0f, 0.0f, 0.0f, 1.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("-1deg Z")) {
			m.Rotate(-1.0f, 0.0f, 0.0f, 1.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("+1deg Z")) {
			m.Rotate(1.0f, 0.0f, 0.0f, 1.0f);
		}
		ImGui::SameLine();
		if (ImGui::Button("+5deg Z")) {
			m.Rotate(5.0f, 0.0f, 0.0f, 1.0f);
		}

		ImGui::Text("Translate: ");
		ImGui::SameLine();
		ImGui::DragFloat4("", m.ptr() + 12, 0.1f);
		ImGui::Text(
			"transform: % 3.2f % 3.2f % 3.2f % 3.2f\n"
			"           % 3.2f % 3.2f % 3.2f % 3.2f\n"
			"           % 3.2f % 3.2f % 3.2f % 3.2f\n"
			"           % 3.2f % 3.2f % 3.2f % 3.2f",
			m.m11,
			m.m12,
			m.m13,
			m.m14,
			m.m21,
			m.m22,
			m.m23,
			m.m24,
			m.m31,
			m.m32,
			m.m33,
			m.m34,
			m.m41,
			m.m42,
			m.m43,
			m.m44);
		ImGui::PopID();
	}

	void SSPHH_Application::imguiShowSphlEditor() {
		///////////////////////////////////////////////////////////
		// S P H L   E D I T O R //////////////////////////////////
		///////////////////////////////////////////////////////////

		static bool init = false;
		static float w = 512.0f;
		static float h = 256.0f;

		if (Interface.tools.showSphlEditor) {
			if (!init) {
				init = true;

				ImGui::SetNextWindowSize(ImVec2(w, h));
				ImGui::SetNextWindowPos(ImVec2(getWidth() - w, getHeight() - h));
			}
			ImGui::SetNextWindowContentSize({ w, -1 });
			ImGui::Begin("SSPL Editor");
			ImGui::Text("%d SSPLs", (int)ssgUserData->ssphhLights.size());
			if (ImGui::Button("Add SSPL")) {
				imguiSphlAdd();
			}
			int i = 0;
			const float stepSize = 0.1f;
			int whichToDelete = -1;
			
			for (auto& sspl : ssgUserData->ssphhLights) {
				std::ostringstream id;
				id << "MS" << std::setfill('0') << std::setw(2) << i << " '" << sspl.name() << "'";
				const void* ptr = &sspl;
#ifdef _WIN32
				// va_list args = nullptr;
#elif __unix__
				// va_list args = va_list();
#endif
				const char* colors[4] = { "R", "G", "B", "Mono" };
				const ImVec4 mscolors[4] = { { 1.0f, 0.0f, 0.0f, 1.0f },
											 { 0.0f, 1.0f, 0.0f, 1.0f },
											 { 0.0f, 0.0f, 1.0f, 1.0f },
											 { 1.0f, 1.0f, 1.0f, 1.0f } };
				float minValue = -5.0f;
				float maxValue = 5.0f;
				ImGui::PushID(i);
				// if (ImGui::TreeNodeV(ptr, id.str().c_str(), args))
				if (ImGui::TreeNode(ptr, "%s", id.str().c_str())) {
					ImGui::SameLine();
					ImGui::Checkbox("", &sspl.enabled);
					ImGui::SameLine();
					if (ImGui::SmallButton("Delete")) {
						whichToDelete = i;
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("Recalc")) {
						sspl.dirty = true;
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("min")) {
						sspl.setCoefficient(-1, AllDegrees, AllBands, minValue);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("-1")) {
						sspl.setCoefficient(-1, AllDegrees, AllBands, -1.0f);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("0")) {
						sspl.setCoefficient(-1, AllDegrees, AllBands, 0.0f);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("?1")) {
						sspl.randomizeCoefficient(-1, AllDegrees, AllBands, -1.0f, 1.0f);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("?MAX")) {
						sspl.randomizeCoefficient(-1, AllDegrees, AllBands, minValue, maxValue);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("1")) {
						sspl.setCoefficient(-1, AllDegrees, AllBands, 1.0f);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("max")) {
						sspl.setCoefficient(-1, AllDegrees, AllBands, maxValue);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("? Coefs")) {
						sspl.randomize = true;
						sspl.dirty = true;
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("? Position")) {
						sspl.randomizePosition = true;
					}

					ImGui::Text("Name %s %d", sspl.name(), 32);
					ImGui::Separator();
					ImGui::Text("Shadow build time: %.1fms", sspl.depthSphlMap.buildTime);

					int lastNumDegrees = sspl.maxDegree;
					ImGui::DragInt("Max Degree", &sspl.maxDegree, 0.1f, 0, 9);
					if (lastNumDegrees != sspl.maxDegree) {
						sspl.changeDegrees(sspl.maxDegree);
					}
					Vector3f position0 = sspl.position;
					ImGui::DragFloat3("Position", sspl.position.ptr(), stepSize, -100.0f, 100.0f);
					if (position0 != sspl.position) {
						sspl.depthSphlMap.dirty = true;
					}
					// sspl.position.w = Fluxions::clamp(sspl.position.w, 0.0f, 1.0f);
					ImGui::DragFloat("E0", &sspl.E0, stepSize, 0.0f, 10000.0f);
					ImGui::DragFloat("Falloff Radius", &sspl.falloffRadius, stepSize, 0.0f, 1000.0f);

					ImGui::Text(
						"status: dirty? %d / shadow map? %d",
						sspl.dirty ? 1 : 0,
						sspl.depthSphlMap.dirty ? 1 : 0);

					for (int j = 0; j < 4; j++) {
						std::ostringstream label;
						label << "Multispectral " << colors[j];
						ptr = &sspl.msph[j];
						if (ImGui::TreeNode(ptr, "%s", label.str().c_str()))
						// if (ImGui::TreeNodeV(ptr, label.str().c_str(), args))
						{
							ImGui::SameLine();
							ImGui::TextColored(mscolors[j], "%s", label.str().c_str());
							ImGui::PushID(j);
							ImGui::SameLine();
							if (ImGui::SmallButton("min")) {
								sspl.setCoefficient(j, AllDegrees, AllBands, minValue);
							}
							ImGui::SameLine();
							if (ImGui::SmallButton("-1")) {
								sspl.setCoefficient(j, AllDegrees, AllBands, -1.0f);
							}
							ImGui::SameLine();
							if (ImGui::SmallButton("0")) {
								sspl.setCoefficient(j, AllDegrees, AllBands, 0.0f);
							}
							ImGui::SameLine();
							if (ImGui::SmallButton("?")) {
								sspl.randomizeCoefficient(j, AllDegrees, AllBands, -1.0f, 1.0f);
							}
							ImGui::SameLine();
							if (ImGui::SmallButton("?MAX")) {
								sspl.randomizeCoefficient(j, AllDegrees, AllBands, minValue, maxValue);
							}
							ImGui::SameLine();
							if (ImGui::SmallButton("1")) {
								sspl.setCoefficient(j, AllDegrees, AllBands, 1.0f);
							}
							ImGui::SameLine();
							if (ImGui::SmallButton("max")) {
								sspl.setCoefficient(j, AllDegrees, AllBands, maxValue);
							}
							ImGui::PopID();

							for (int l = 0; l <= sspl.maxDegree; l++) {
								ImGui::PushID(l);
								for (int m = -l; m <= l; m++) {
									int lm = (int)sspl.msph[j].getCoefficientIndex(l, m);
									float oldValue = sspl.msph[j].getCoefficient(l, m);
									ImGui::PushID(i * 100000 + j * 10000 + lm * 100);
									unsigned offset = sspl.msph[j].getCoefficientIndex(l, m);
									std::ostringstream label;
									label << "l = " << l << ", m = " << m << " (" << offset << ")"
										  << "##" << std::setw(2) << std::setfill('0') << i << j;

									if (ImGui::SmallButton("iso")) {
										sspl.isolateCoefficient(j, l, m);
									}
									ImGui::PushItemWidth(64.0f);
									ImGui::SameLine();
									ImGui::DragFloat(label.str().c_str(), &sspl.msph[j][offset], stepSize, -5.0f, 5.0f);
									ImGui::PopItemWidth();
									ImGui::SameLine();
									if (ImGui::SmallButton("min")) {
										sspl.setCoefficient(j, l, m, minValue);
									}
									ImGui::SameLine();
									if (ImGui::SmallButton("-1")) {
										sspl.setCoefficient(j, l, m, -1.0f);
									}
									ImGui::SameLine();
									if (ImGui::SmallButton("0")) {
										sspl.setCoefficient(j, l, m, 0.0f);
									}
									ImGui::SameLine();
									if (ImGui::SmallButton("?")) {
										sspl.randomizeCoefficient(j, l, m, -1.0f, 1.0f);
									}
									ImGui::SameLine();
									if (ImGui::SmallButton("?MAX")) {
										sspl.randomizeCoefficient(j, l, m, minValue, maxValue);
									}
									ImGui::SameLine();
									if (ImGui::SmallButton("1")) {
										sspl.setCoefficient(j, l, m, 1.0f);
									}
									ImGui::SameLine();
									if (ImGui::SmallButton("max")) {
										sspl.setCoefficient(j, l, m, maxValue);
									}

									if (0) // m == 0)
									{
										ImGui::PushID(-l);
										ImGui::SameLine();
										if (ImGui::SmallButton("min")) {
											sspl.setCoefficient(j, l, AllBands, minValue);
										}
										ImGui::SameLine();
										if (ImGui::SmallButton("-1")) {
											sspl.setCoefficient(j, l, AllBands, -1.0f);
										}
										ImGui::SameLine();
										if (ImGui::SmallButton("0")) {
											sspl.setCoefficient(j, l, AllBands, 0.0f);
										}
										ImGui::SameLine();
										if (ImGui::SmallButton("?")) {
											sspl.randomizeCoefficient(j, l, AllBands, -1.0f, 1.0f);
										}
										ImGui::SameLine();
										if (ImGui::SmallButton("?MAX")) {
											sspl.randomizeCoefficient(j, l, AllBands, minValue, maxValue);
										}
										ImGui::SameLine();
										if (ImGui::SmallButton("1")) {
											sspl.setCoefficient(j, l, AllBands, 1.0f);
										}
										ImGui::SameLine();
										if (ImGui::SmallButton("max")) {
											sspl.setCoefficient(j, l, AllBands, maxValue);
										}
										ImGui::PopID();
									}
									ImGui::PopID();
									float newValue = sspl.msph[j].getCoefficient(l, m);
									if (newValue != oldValue) {
										sspl.dirty = true;
									}
								}
								ImGui::PopID();
							}
							ImGui::TreePop();
						} else {
							ImGui::SameLine();
							ImGui::TextColored(mscolors[j], "%s", label.str().c_str());
							ImGui::PushID(j);
							ImGui::SameLine();
							if (ImGui::SmallButton("min")) {
								sspl.setCoefficient(j, AllDegrees, AllBands, minValue);
							}
							ImGui::SameLine();
							if (ImGui::SmallButton("-1")) {
								sspl.setCoefficient(j, AllDegrees, AllBands, -1.0f);
							}
							ImGui::SameLine();
							if (ImGui::SmallButton("0")) {
								sspl.setCoefficient(j, AllDegrees, AllBands, 0.0f);
							}
							ImGui::SameLine();
							if (ImGui::SmallButton("?")) {
								sspl.randomizeCoefficient(j, AllDegrees, AllBands, minValue, maxValue);
							}
							ImGui::SameLine();
							if (ImGui::SmallButton("1")) {
								sspl.setCoefficient(j, AllDegrees, AllBands, 1.0f);
							}
							ImGui::SameLine();
							if (ImGui::SmallButton("max")) {
								sspl.setCoefficient(j, AllDegrees, AllBands, maxValue);
							}
							ImGui::PopID();
						}
					}

					ImGui::TreePop();
				} else {
					ImGui::SameLine();
					ImGui::Checkbox("", &sspl.enabled);
					ImGui::SameLine();
					if (ImGui::SmallButton("Delete")) {
						whichToDelete = i;
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("Recalc")) {
						sspl.dirty = true;
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("min")) {
						sspl.setCoefficient(-1, AllDegrees, AllBands, minValue);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("-1")) {
						sspl.setCoefficient(-1, AllDegrees, AllBands, -1.0f);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("0")) {
						sspl.setCoefficient(-1, AllDegrees, AllBands, 0.0f);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("?")) {
						sspl.randomizeCoefficient(-1, AllDegrees, AllBands, minValue, maxValue);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("1")) {
						sspl.setCoefficient(-1, AllDegrees, AllBands, 1.0f);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("max")) {
						sspl.setCoefficient(-1, AllDegrees, AllBands, maxValue);
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("? Coefs")) {
						sspl.randomize = true;
						sspl.dirty = true;
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("? Position")) {
						sspl.randomizePosition = true;
					}
				}
				if (sspl.dirty || sspl.randomizePosition) {
					sspl.depthSphlMap.dirty = true;
				}
				ImGui::PopID();
				i++;
			}
			if (whichToDelete >= 0) {
				ssgUserData->ssphhLights.erase(ssgUserData->ssphhLights.begin() + whichToDelete);
			}
			ImGui::End();
		}
	}

	void SSPHH_Application::imguiShowMaterialEditor() {
		///////////////////////////////////////////////////////////
		// M A T E R I A L   E D I T O R //////////////////////////
		///////////////////////////////////////////////////////////

		//		if (Interface.tools.showMaterialEditor) {
		//			imguiWinX += imguiWinW + 64.0f;
		//			ImGui::SetNextWindowContentWidth(imguiWinW);
		//			ImGui::SetNextWindowPos(ImVec2(imguiWinX, 64));
		//			ImGui::Begin("Material Editor");
		//			//if (Interface.mtls.mtlsCollapsed.size() != ssg->materials.size())
		//			//{
		//			//	Interface.mtls.mtlsCollapsed.resize(ssg.materials.size());
		//			//}
		//
		//			ImGui::Checkbox("Maps", &Interface.mtls.showMaps);
		//			if (Interface.mtls.showMaps) {
		//				int i = 0;
		//				SEPARATOR_NEWLINE
		//					for (auto& mtllib : ssg->materialSystem) {
		//						ImGui::Text("Mtllib %i: %s", mtllib.first, mtllib.second.name.c_str());
		//						for (auto& map : mtllib.second.maps) {
		//							ImGui::Text("map: %s", map.second.mapName.c_str());
		//						}
		//						i++;
		//					}
		//				NEWLINE_SEPARATOR
		//			}
		//
		//			ImGui::Checkbox("Mtls", &Interface.mtls.showMtls);
		//			if (Interface.mtls.showMtls) {
		//				int i = 0;
		//				for (auto& mtllib : ssg->materialSystem) {
		//					std::string mtllib_key = mtllib.second.fpi.fullfname;
		//					bool* mtllib_collapsed = nullptr;
		//					if (Interface.mtls.mtllibsCollapsed.find(mtllib_key) ==
		// Interface.mtls.mtllibsCollapsed.end()) { 						Interface.mtls.mtllibsCollapsed[mtllib_key]
		// = false;
		//					}
		//					mtllib_collapsed = &Interface.mtls.mtllibsCollapsed[mtllib_key];
		//					if (mtllib_collapsed == nullptr)
		//						continue;
		//					ImGui::TextColored(Colors::Red, "MtlLib %i %s", mtllib.first,
		// mtllib.second.fpi.fullfname.c_str()); 					ImGui::SameLine();
		// ImGui::Checkbox(mtllib_key.c_str(),
		// mtllib_collapsed); 					if (*mtllib_collapsed) { 						int j = 0; 						for (auto& mtl
		// : mtllib.second.mtls) { 							std::string mtl_name =
		// mtllib.second.mtls.getNameFromHandle(mtl.first); std::string mtllib_mtl_key = mtllib_key + "!!" + mtl_name;
		// bool* mtl_collapsed = nullptr; 							if
		// (Interface.mtls.mtlsCollapsed.find(mtllib_mtl_key)
		// ==
		// Interface.mtls.mtlsCollapsed.end()) { Interface.mtls.mtlsCollapsed[mtllib_mtl_key] = false;
		// Interface.mtls.mtls_ptrs[mtllib_mtl_key].Ka = mtl.second.Ka.ptr();
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].Kd = mtl.second.Kd.ptr();
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].Ks = mtl.second.Ks.ptr();
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].Ke = mtl.second.Ke.ptr();
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].PBm = &mtl.second.PBm;
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].PBk = &mtl.second.PBk;
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].PBior = &mtl.second.PBior;
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].PBKdm = &mtl.second.PBKdm;
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].PBKsm = &mtl.second.PBKsm;
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].PBGGXgamma = &mtl.second.PBGGXgamma;
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].map_Kd = mtl.second.map_Kd.c_str();
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].map_Ks = mtl.second.map_Ks.c_str();
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].map_bump = mtl.second.map_bump.c_str();
		//								Interface.mtls.mtls_ptrs[mtllib_mtl_key].map_normal =
		// mtl.second.map_normal.c_str();
		//							}
		//							mtl_collapsed = &Interface.mtls.mtlsCollapsed[mtllib_mtl_key];
		//							if (mtl_collapsed == nullptr)
		//								continue;
		//
		//							ImGui::TextColored(Colors::Yellow, "Material Name: %i", mtl.first);
		//							ImGui::SameLine();
		//							ImGui::Checkbox(mtl_name.c_str(), mtl_collapsed);
		//							if (*mtl_collapsed) {
		//								auto pmtl = Interface.mtls.mtls_ptrs[mtllib_mtl_key];
		//								NEWLINE_SEPARATOR
		//									const float stepSize = 0.01f;
		//								// const float minSize = -5.0f;
		//								// const float maxSize = 5.0f;
		//#define JOIN(x) (x + ("##" + mtllib_mtl_key)).c_str()
		//								ImGui::DragFloat(JOIN("PBk"), pmtl.PBk, stepSize, 0.0f, 10.0f);
		//								ImGui::DragFloat(JOIN("PBior"), pmtl.PBior, stepSize, 0.0f, 2.5f);
		//								ImGui::DragFloat(JOIN("PBkdm"), pmtl.PBKdm, stepSize, -1.0f, 1.0f);
		//								ImGui::DragFloat(JOIN("PBksm"), pmtl.PBKsm, stepSize, -1.0f, 1.0f);
		//								ImGui::DragFloat(JOIN("PBGGXgamma"), pmtl.PBGGXgamma, stepSize, 0.0f, 1.0f);
		//								ImGui::DragFloat3(JOIN("Kd"), pmtl.Kd, stepSize, 0.0f, 1.0f);
		//								ImGui::Text(JOIN("map_Kd: %s"), pmtl.map_Kd);
		//								ImGui::DragFloat3(JOIN("Ks"), pmtl.Ks, stepSize, 0.0f, 1.0f);
		//								ImGui::Text(JOIN("map_Ks: %s"), pmtl.map_Ks);
		//								ImGui::Text(JOIN("map_bump: %s"), pmtl.map_bump);
		//								ImGui::Text(JOIN("map_normal: %s"), pmtl.map_normal);
		//#undef JOIN
		//							}
		//							j++;
		//							ImGui::Separator();
		//						}
		//						ImGui::Separator();
		//					}
		//					i++;
		//				}
		//			}
		//			ImGui::End();
		//		}
	}

	void SSPHH_Application::imguiSphlAdd() {
		ssgUserData->ssphhLights.resize(ssgUserData->ssphhLights.size() + 1);
		SimpleSSPHHLight& sphl = ssgUserData->ssphhLights.back();
		std::ostringstream name;
		name << "New SSPL " << ssgUserData->ssphhLights.size();
		sphl.setName(name.str());
		// sphl.name.reserve(32);
		sphl.changeDegrees(DefaultSphlDegree);

		for (auto& sphl : ssgUserData->ssphhLights) {
			sphl.dirty = true;
		}

		Corona_GenerateSphlINIT();
	}

	void SSPHH_Application::imguiSphlDelete() {
		if (!ssgUserData->ssphhLights.empty())
			ssgUserData->ssphhLights.pop_back();

		Corona_GenerateSphlINIT();
	}

	void SSPHH_Application::imguiSphlRandomize() {
		if (ssgUserData->ssphhLights.empty())
			return;
		auto i = ssgUserData->ssphhLights.size() - 1;
		ssgUserData->ssphhLights[i].randomizeCoefficients();
	}

	void SSPHH_Application::imguiSphlDecreaseDegrees() {
		if (ssgUserData->ssphhLights.empty())
			return;
		auto i = ssgUserData->ssphhLights.size() - 1;
		ssgUserData->ssphhLights[i].decDegrees();
	}

	void SSPHH_Application::imguiSphlIncreaseDegrees() {
		if (ssgUserData->ssphhLights.empty())
			return;
		auto i = ssgUserData->ssphhLights.size() - 1;
		ssgUserData->ssphhLights[i].incDegrees();
	}

	void SSPHH_Application::imguiSphlCoronaToSPH(size_t which) {
		if (ssgUserData->ssphhLights.empty() && ssgUserData->ssphhLights.size() <= which)
			return;

		RendererTextureCubePtr enviroSkyBoxTexture = rendererContext->textureCubes["enviroSkyBox"];
		if (enviroSkyBoxTexture) {
			enviroSkyBoxTexture->loadMap("export_cubemap.png", true);
		}
		SphlSampler sphlSampler;
		sphlSampler.resize(64, 32);
		sphlSampler.saveSampleMap("sample_stratification.ppm", 4);
		// TODO: Need to write a CubeMapToSphl routine
		// sphlSampler.sampleCubeMap(enviroSkyBoxTexture.getImage(), ssgUserData->ssphhLights[which].msph);

		ssgUserData->ssphhLights[which].dirty = true;
	}

	void SSPHH_Application::imguiSphlSPHtoCubeMap(size_t which) {
		if (ssgUserData->ssphhLights.empty() && ssgUserData->ssphhLights.size() <= which)
			return;

		ssgUserData->ssphhLights[which].dirty = true;
	}

	void SSPHH_Application::imguiSphlSaveToOBJ() {
		// int i = 0;
		// for (auto &it : sphls)
		//{
		//	auto &sphl = it.second;

		//	ostringstream ostr;
		//	ostr << "sphl" << setw(2) << setfill('0') << i;
		//	sphl.sph_model.saveOBJ(ostr.str());
		//	i++;
		//}

		size_t i = 0;
		for (auto& sphl : ssgUserData->ssphhLights) {
			std::ostringstream ostr;
			ostr << ssg->name() << "_sphl_" << std::setw(2) << std::setfill('0') << i;
			sphl.saveOBJ("output", ostr.str());
			std::string path = "output/" + ostr.str();
			sphl.savePtrcLightProbe(path + ".ppm");
			sphl.savePtrcLightProbe(path + ".exr");
			i++;
		}
	}

	void SSPHH_Application::imguiToolsTakeScreenshot() { Interface.saveScreenshot = true; }

	void SSPHH_Application::imguiToolsSaveStats() { Hf::Log.saveStats(ssg->name_str() + "_"); }

	void SSPHH_Application::imguiToolsResetStats() { Hf::Log.resetStat("frametime"); }


	void SSPHH_Application::imguiShowUfWindow() {
		if (!Interface.tools.showUnicornfishWindow)
			return;
		if (!Interface.uf.windowInit) {
			Interface.uf.windowInit = true;
			ImGui::SetNextWindowContentSize({ 512.0f, -1 });
			ImGui::SetNextWindowPos(ImVec2(imguiWinX + imguiWinW + 32, 64));
		}
		ImGui::Begin("Unicornfish");
		if (Interface.uf.uf_type == UfType::None) {
			// if (ImGui::Button("UF CLIENT GLES30")) { Interface.uf.uf_type = UfType::Client; }
			// if (ImGui::Button("UF CLIENT GLES20")) { Interface.uf.uf_type = UfType::ClientGLES20; }
			// if (ImGui::Button("UF BROKER")) { Interface.uf.uf_type = UfType::Broker; }
			// if (ImGui::Button("UF WORKER ECHO")) { Interface.uf.uf_type = UfType::WorkerEcho; }
			// if (ImGui::Button("UF WORKER CORONA")) { Interface.uf.uf_type = UfType::Worker; }
			// ImGui::Separator();
			ImGui::Checkbox("Standalone CLIENT", &Interface.uf.standalone_client);
			ImGui::Checkbox("Standalone BROKER", &Interface.uf.standalone_broker);
			ImGui::Checkbox("Standalone WORKER", &Interface.uf.standalone_worker);
			if (ImGui::Button("START")) {
				ssphhUf.StartStandalone(
					Interface.uf.standalone_client,
					Interface.uf.standalone_broker,
					Interface.uf.standalone_worker);
			}
			ImGui::SameLine();
			if (ImGui::Button("STOP")) {
				ssphhUf.Stop();
				ssphhUf.Join();
			}
			ImGui::TextColored(Colors::Cyan, "CLIENT: %s", ssphhUf.GetUIMessage(Unicornfish::NodeType::Client).c_str());
			ImGui::TextColored(Colors::Rose, "BROKER: %s", ssphhUf.GetUIMessage(Unicornfish::NodeType::Broker).c_str());
			ImGui::TextColored(
				Colors::Yellow,
				"WORKER: %s",
				ssphhUf.GetUIMessage(Unicornfish::NodeType::Worker).c_str());
			ImGui::TextColored(Colors::Green, "GI STATUS: %s", Interface.ssphh.gi_status.c_str());
		} else {
			if (ImGui::Button("STOP")) {
				Interface.uf.uf_stop = true;
			}
			ImGui::Separator();

			switch (Interface.uf.uf_type) {
			case UfType::Broker:
				imguiUfBrokerControls();
				break;
			case UfType::ClientGLES20:
				imguiUfClientGLES20Controls();
				break;
			case UfType::Client:
				imguiUfClientGLES30Controls();
				break;
			case UfType::WorkerEcho:
				imguiUfWorkerEchoControls();
				break;
			case UfType::Worker:
				imguiUfWorkerCoronaControls();
				break;
			default:
				Interface.uf.uf_type = UfType::None;
			}
		}
		ImGui::End();
	}

	void SSPHH_Application::imguiShowSSPHHWindow() {
		if (!Interface.tools.showSSPHHWindow)
			return;
		static bool init = false;
		if (!init) {
			float w = 640.0f;
			float h = 800.0f;
			ImGui::SetNextWindowPos(ImVec2(getWidth() - w, getHeight() - h));
			ImGui::SetNextWindowSize(ImVec2(w, h));
			init = true;
		}
		ImGui::SetNextWindowContentSize({ 640.0f, -1 });
		ImGui::Begin("SSPHH");

		ImGui::End();
	}
} // namespace SSPHH
