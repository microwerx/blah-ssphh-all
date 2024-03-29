#include "pch.hpp"
#include <ssphhapp.hpp>

namespace SSPHH {
	// void SSPHH_Application::OnKeyDown(int key)
	//{
	//	return;
	//	int modifiers = glutGetModifiers();
	//	if (modifiers == 0)
	//	{
	//		if (key == 27)
	//		{
	//			glutLeaveMainLoop();
	//		}
	//
	//		if (key == '1')
	//		{
	//			Hf::StopWatch stopwatch;
	//			InitRenderConfigs();
	//			LoadRenderConfigs();
	//			stopwatch.Stop();
	//			HFLOGINFO("reload took %4.2f milliseconds", stopwatch.GetMillisecondsElapsed());
	//		}
	//
	//		if (key == '2')
	//		{
	//			Hf::StopWatch stopwatch;
	//			ssg.Load(sceneFilename);
	//			stopwatch.Stop();
	//			HFLOGINFO("SSG reload took %4.2f milliseconds", stopwatch.GetMillisecondsElapsed());
	//		}
	//
	//		if (tolower(key) == '3') {
	//			counter++;
	//			if (counter > 3)
	//				counter = 0;
	//		}
	//
	//		if (key == '4')
	//		{
	//			python.addCommand(Df::PythonInterpreter::Command::PyRunFile, "resources/scripts/guitest.py");
	//		}
	//
	//		if (key == '5')
	//		{
	//			python.addCommand(Df::PythonInterpreter::Command::PyRunString, "print('hello')");
	//		}
	//
	//		if (key == '6')
	//		{
	//			Interface->drawSkyBox = !Interface->drawSkyBox;
	//		}
	//
	//		if (key == '7')
	//		{
	//
	//		}
	//
	//		if (key == '8')
	//		{
	//
	//		}
	//
	//		if (key == '9')
	//		{
	//
	//		}
	//
	//		if (key == 'r' || key == '\t')
	//		{
	//			Interface->cameraMatrix_.LoadIdentity();
	//			ResetScene();
	//		}
	//
	//		if (key == ' ')
	//		{
	//			Interface->enableOrbit = !Interface->enableOrbit;
	//		}
	//
	//		if (key == '-' || key == '_')
	//		{
	//			// decrease 1 hour
	//			Sun_AdvanceClock(-3600.0, true);
	//		}
	//
	//		if (key == '=' || key == '+')
	//		{
	//			// increase 1 hour
	//			Sun_AdvanceClock(3600.0, true);
	//		}
	//
	//		if (tolower(key) == 'a') Interface->moveLeft = true;
	//		if (tolower(key) == 'd') Interface->moveRight = true;
	//		if (tolower(key) == 'w') Interface->moveForward = true;
	//		if (tolower(key) == 's') Interface->moveBackward = true;
	//		if (tolower(key) == 'z') Interface->moveUp = true;
	//		if (tolower(key) == 'c') Interface->moveDown = true;
	//		if (tolower(key) == 'q') Interface->rollLeft = true;
	//		if (tolower(key) == 'e') Interface->rollRight = true;
	//	}
	//	else if (modifiers &= GLUT_ACTIVE_SHIFT)
	//	{
	//		if (tolower(key) == 'h') Interface->decreaseLongitude = true;
	//		if (tolower(key) == 'j') Interface->increaseLongitude = true;
	//		if (tolower(key) == 'k') Interface->decreaseLatitude = true;
	//		if (tolower(key) == 'l') Interface->increaseLatitude = true;
	//	}
	//	else if (modifiers &= GLUT_ACTIVE_ALT)
	//	{
	//
	//	}
	//}
	//
	//
	// void SSPHH_Application::OnKeyUp(int key)
	//{
	//	if (tolower(key) == 'a') Interface->moveLeft = false;
	//	if (tolower(key) == 'd') Interface->moveRight = false;
	//	if (tolower(key) == 'w') Interface->moveForward = false;
	//	if (tolower(key) == 's') Interface->moveBackward = false;
	//	if (tolower(key) == 'z') Interface->moveUp = false;
	//	if (tolower(key) == 'c') Interface->moveDown = false;
	//	if (tolower(key) == 'q') Interface->rollLeft = false;
	//	if (tolower(key) == 'e') Interface->rollRight = false;
	//}
	//
	//
	// void SSPHH_Application::OnSpecialKeyDown(int key)
	//{
	//	return;
	//
	//	int modifiers = glutGetModifiers();
	//
	//	if (key == GLUT_KEY_LEFT) Interface->yawLeft = true;
	//	if (key == GLUT_KEY_RIGHT) Interface->yawRight = true;
	//	if (key == GLUT_KEY_UP) Interface->pitchUp = true;
	//	if (key == GLUT_KEY_DOWN) Interface->pitchDown = true;
	//
	//	if (key == GLUT_KEY_F1) Interface->showHelp = !Interface->showHelp;
	//	if (key == GLUT_KEY_F2) Interface->showMainHUD = !Interface->showMainHUD;
	//	if (key == GLUT_KEY_F3) Interface->showHUD = !Interface->showHUD;
	//	if (key == GLUT_KEY_F4) Interface->recomputeSky = !Interface->recomputeSky;
	//	if (key == GLUT_KEY_F5 && modifiers == 0) Interface->saveCoronaSCN = true;
	//	if (key == GLUT_KEY_F6 && modifiers == 0) Interface->saveCoronaCubeMapSCN = true;
	//	if (key == GLUT_KEY_F5 && modifiers == GLUT_ACTIVE_CTRL) Interface->renderCoronaSCN = true;
	//	if (key == GLUT_KEY_F6 && modifiers == GLUT_ACTIVE_CTRL) Interface->renderCoronaCubeMapSCN = true;
	//
	//	if (key == GLUT_KEY_F11)
	//	{
	//		SavePbskyTextures();
	//	}
	//
	//	if (key == GLUT_KEY_F12) {
	//		renderMode++;
	//		if (renderMode >= MAX_RENDER_MODES)
	//			renderMode = 0;
	//	}
	//}
	//
	//
	// void SSPHH_Application::OnSpecialKeyUp(int key)
	//{
	//	return;
	//
	//	if (key == GLUT_KEY_LEFT) Interface->yawLeft = false;
	//	if (key == GLUT_KEY_RIGHT) Interface->yawRight = false;
	//	if (key == GLUT_KEY_UP) Interface->pitchUp = false;
	//	if (key == GLUT_KEY_DOWN) Interface->pitchDown = false;
	//}

	void SSPHH_Application::OnKeyDown(const std::string& key, int keymod) {
		Widget::OnKeyDown(key, keymod);

		if (keymod == keyboard.CtrlKeyBit) {
			if (key == "F5") Interface->renderCoronaSCN = true;
			if (key == "F6") Interface->renderCoronaCubeMapSCN = true;
		}

		if (keymod == keyboard.ShiftKeyBit) {}

		if (Interface->showImGui) {
			short c = (key.size() == 1) ? key[0] : 0;
			if (Vf::DearImGuiWidget::ContextInitialized) {
				ImGuiIO& io = ImGui::GetIO();
				if (isalnum(c) || c == ' ' || c == '-' || c == '_' || c == '.') {
					io.AddInputCharacter(c);
					return;
				}
			}
		}

		if (keymod == keyboard.AltKeyBit) {
			if (key == "F6") { PathAnim_LoadCameraPath(""); }
		}

		if (keymod == 0) {
			if (Interface->showImGui == false && key == "Escape") { LeaveMainLoop(); }
			else if (Interface->showImGui == true && key == "Escape") {
				return;
			}

			if (key == "1") { ReloadRenderConfigs(); }

			if (key == "2") { LoadShaders(); }

			if (key == "3") { SSG_ReloadScene(); }

			if (key == "4") {
				Interface->drawRectShadows = !Interface->drawRectShadows;
				// counter++;
				// if (counter > 3) counter = -1;
			}

			if (key == "5") {
				Interface->drawCubeShadows = !Interface->drawCubeShadows;

				// Interface->tools.shaderDebugChoice = Fluxions::clamp(Interface->tools.shaderDebugChoice - 1, 0, 20);
				// python.addCommand(Df::PythonInterpreter::Command::PyRunFile, "resources/scripts/guitest.py");
			}

			if (key == "6") {
				Interface->drawSkyBox = !Interface->drawSkyBox;

				// Interface->tools.shaderDebugChoice = Fluxions::clamp(Interface->tools.shaderDebugChoice + 1, 0, 20);
				// python.addCommand(Df::PythonInterpreter::Command::PyRunString, "print('hello')");
			}

			if (key == "7") {
				Interface->drawPBR = !Interface->drawPBR;
				// if (Interface->tools.shaderDebugSphl != SphlSunIndex)
				//	Interface->tools.shaderDebugSphl = SphlSunIndex;
				// else
				//	Interface->tools.shaderDebugSphl = -1;
			}

			if (key == "8") {
				Interface->drawVIZ = !Interface->drawVIZ;
				// Interface->tools.shaderDebugSphl = Fluxions::clamp(Interface->tools.shaderDebugSphl - 1, -1,
				// SphlSunIndex);
			}

			if (key == "9") {
				Interface->drawPOST = !Interface->drawPOST;
				// Interface->tools.shaderDebugSphl = Fluxions::clamp(Interface->tools.shaderDebugSphl + 1, -1,
				// SphlSunIndex);
			}

			if (key == "r" || key == "Tab") {
				Interface->cameraOrbit.reset();
				Interface->preCameraMatrix.LoadIdentity();
				Sun_ResetClock();
			}

			if (key == "n" || key == "N") { Sun_UseCurrentTime(); }

			if (key == " ") { PathAnim_Toggle(); }

			if (key == "o" || key == "O") { Interface->enableOrbit = !Interface->enableOrbit; }

			if (key == "i" || key == "I") {
				Corona_GenerateSphlINIT();
				Sky_RegenCoronaSky();
			}

			if (key == "v" || key == "V") { Corona_GenerateSphlVIZ(); }

			if (key == "g" || key == "G") { Corona_GenerateSphlGEN(); }

			if (key == "h" || key == "H") { Corona_GenerateSphlHIER(); }

			if (key == "j") Interface->cameraOrbit.x -= 5.0f;
			if (key == "k") Interface->cameraOrbit.x += 5.0f;

			if (key == "[")
				ssg->environment->toneMapExposure() =
					Fluxions::clamp(ssg->environment->toneMapExposure() - 0.5f, -12.0f, 12.0f);
			if (key == "]")
				ssg->environment->toneMapExposure() =
					Fluxions::clamp(ssg->environment->toneMapExposure() + 0.5f, -12.0f, 12.0f);
			if (key == ";")
				Interface->tools.shaderDebugChoice = Fluxions::clamp(Interface->tools.shaderDebugChoice - 1, 0, 20);
			if (key == "'")
				Interface->tools.shaderDebugChoice = Fluxions::clamp(Interface->tools.shaderDebugChoice + 1, 0, 20);
			if (key == ",") Interface->ssphh.MaxDegrees = Fluxions::clamp(Interface->ssphh.MaxDegrees - 1, 0, 9);
			if (key == ".") Interface->ssphh.MaxDegrees = Fluxions::clamp(Interface->ssphh.MaxDegrees + 1, 0, 9);

			if (key == "," || key == ".") Corona_GenerateSphlHIER();

			if (key == "-" || key == "_") {
				// decrease 1 hour
				Sun_AdvanceClock(-3600.0, true);
			}

			if (key == "=" || key == "+") {
				// increase 1 hour
				Sun_AdvanceClock(3600.0, true);
			}

			if (key == "F1") hotkeyWindows[key]->ToggleVisible();
			if (key == "F2") hotkeyWindows[key]->ToggleVisible();
			if (key == "F3") hotkeyWindows[key]->ToggleVisible();
			if (key == "F4") hotkeyWindows[key]->ToggleVisible();
			if (key == "F5") hotkeyWindows[key]->ToggleVisible();
			if (key == "F6") hotkeyWindows[key]->ToggleVisible();
			if (key == "F7") hotkeyWindows[key]->ToggleVisible();
			if (key == "F8") hotkeyWindows[key]->ToggleVisible();
			if (key == "F9") hotkeyWindows[key]->ToggleVisible();
			if (key == "F10") {
				hotkeyWindows[key]->ToggleVisible();
				Interface->showImGui = hotkeyWindows[key]->isVisible();
			}
			if (key == "F11") hotkeyWindows[key]->ToggleVisible();
			if (key == "F12") hotkeyWindows[key]->ToggleVisible();

			// Interface->showHelp = !Interface->showHelp;
			// if (key == "F2")
			//	Interface->showMainHUD = !Interface->showMainHUD;
			// if (key == "F3")
			//	Interface->showHUD = !Interface->showHUD;
			// if (key == "F4")
			//	Interface->showDeferredHUD = !Interface->showDeferredHUD;
			// if (key == "F5")
			//	Interface->recomputeSky = !Interface->recomputeSky;
			// if (key == "F6")
			//	Sky_RegenCoronaSky();
			// if (key == "F7")
			//	Interface->ssphh.enableShowSPHLs = !Interface->ssphh.enableShowSPHLs;
			// if (key == "F8")
			//	Interface->showImGui = !Interface->showImGui;
			// if (key == "F10") {
			//	Interface->showImGui = !Interface->showImGui;
			//	HFLOGDEBUG("%s ImGui", Interface->showImGui ? "showing" : "hiding");
			//}

			// if (key == "F11") {
			//	Sky_SaveHosekWilkieTextures();
			//}

			// if (key == "F12") {
			//	incr_wrap(renderMode, MAX_RENDER_MODES - 1);
			//}
		}
	}

	void SSPHH_Application::OnKeyUp(const std::string& key, int keymod) { Widget::OnKeyUp(key, keymod); }

	void SSPHH_Application::OnMouseMove(int x, int y) {
		if (Interface->showImGui) return;

		Widget::OnMouseMove(x, y);
	}

	void SSPHH_Application::OnMouseButtonDown(int button) { Widget::OnMouseButtonDown(button); }

	void SSPHH_Application::OnMouseButtonUp(int button) { Widget::OnMouseButtonUp(button); }

	void SSPHH_Application::OnMouseClick(int button, const MouseClickState& mcs) { Widget::OnMouseClick(button, mcs); }

	void SSPHH_Application::OnMouseDoubleClick(int button, const MouseDoubleClickState& mdcs) {
		Widget::OnMouseDoubleClick(button, mdcs);
	}

	void SSPHH_Application::OnMouseDrag(int button, const MouseDragState& mds) {
		Widget::OnMouseDrag(button, mds);
		if (button == 2) { rendererContext->setDeferredSplit(mds.currentPosition); }
	}
} // namespace SSPHH
