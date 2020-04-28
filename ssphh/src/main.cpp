#include "pch.hpp"

#define USE_GLFW 1
//#define USE_FREEGLUT 1

#ifdef _WIN32
#define _STDCALL_SUPPORTED

#pragma comment(lib, "xinput.lib")	 // XInput Library
#pragma comment(lib, "glu32.lib")	 // OpenGL Utility library
#pragma comment(lib, "opengl32.lib") // Microsoft OpenGL library
#pragma comment(lib, "gdi32.lib")	 // Windows GDI library
#pragma comment(lib, "winmm.lib")	 // Windows MultiMedia library
#pragma comment(lib, "user32.lib")	 // Windows User library

#pragma comment(lib, "fluxions.lib")

//#ifdef _WIN32
//#pragma comment(lib, "libsodium.lib")
//#pragma comment(lib, "libzmq.lib")
//#pragma comment(lib, "libczmq.lib")
//#pragma comment(lib, "libcurl.lib")
//#endif
#ifdef NEED_LIBARIES
#ifdef _DEBUG
#pragma comment(lib, "libzmq-mt-gd-4_3_3.lib")
#pragma comment(lib, "glew32d.lib")
#ifdef USE_GLFW
#pragma comment(lib, "glfw3dll.lib")
#endif
#ifdef USE_FREEGLUT
#pragma comment(lib, "freeglut_staticd.lib")
#endif
#ifndef FLUXIONS_NO_OPENEXR
#pragma comment(lib, "Half-2_3_d.lib")
#pragma comment(lib, "IlmImf-2_3_d.lib")
#endif
#ifndef FLUXIONS_NO_SDL
#pragma comment(lib, "SDL2d.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_mixer.lib")
#endif
#else
#pragma comment(lib, "libzmq-mt-4_3_3.lib")
#pragma comment(lib, "glew32.lib")
#ifdef USE_GLFW
#pragma comment(lib, "glfw3dll.lib")
#endif
#ifdef USE_FREEGLUT
#pragma comment(lib, "freeglut_static.lib")
#endif
#ifndef FLUXIONS_NO_OPENEXR
#pragma comment(lib, "Half-2_3.lib")
#pragma comment(lib, "IlmImf-2_3.lib")
#endif
#ifndef FLUXIONS_NO_SDL
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_mixer.lib")
#endif
#endif // _DEBUG
#endif

#endif // _WIN32

#include <DragDrop.hpp>
#include <GLFW_template.hpp>
#include <GLUT_template.hpp>

#include <fluxions.hpp>
#include <help_window.hpp>
#include <renderer_config_window.hpp>
#include <renderer_window.hpp>
#include <scene_editor_window.hpp>
#include <scene_graph_window.hpp>
#include <ssphh_menu.hpp>
#include <ssphh_window.hpp>
#include <ssphhapp.hpp>
#include <tool_window.hpp>
#include <unicornfish.hpp>
#include <unicornfish_window.hpp>
#include <viperfish.hpp>
#include <viperfish_animation_window.hpp>
#include <viperfish_animpath_window.hpp>
#include <viperfish_loading_window.hpp>
#include <viperfish_root_window.hpp>
#include <viperfish_stats_window.hpp>

SSPHHPtr ssphh_widget_ptr;
Vf::AnimPathWindowPtr animpath_window_ptr;

class MainApp {
	SSPHHPtr ssphh_widget_ptr;
	Vf::WidgetPtr vf_app_ptr;
	Vf::WidgetPtr imgui_widget_ptr;
	Vf::LoadingWindowPtr loading_window_ptr;


	/* F1  */ HelpWindowPtr help_window_ptr;
	/* F2  */ Vf::StatsWindowPtr stats_window_ptr;
	/* F3  */ SceneGraphWindowPtr scene_graph_window_ptr;
	/* F4  */ SceneEditorWindowPtr scene_editor_window_ptr;
	/* F5  */ SSPHHWindowPtr ssphh_window_ptr;
	/* F6  */ Vf::AnimPathWindowPtr animpath_window_ptr;
	/* F7  */ Vf::AnimationWindowPtr animation_window_ptr;
	/* F8  */ RendererConfigWindowPtr renderer_config_window_ptr;
	/* F9  */ RendererWindowPtr renderer_window_ptr;
	/* F10 */ SsphhMenuPtr ssphh_menu_ptr;
	/* F11 */ ToolWindowPtr tool_window_ptr;
	/* F12 */ UnicornfishWindowPtr unicornfish_window_ptr;
public:
	int run(int argc, char** argv);
	void init();
	void kill();
};

int main(int argc, char** argv) {
	int result = 0;
	{ MainApp app;
	result = app.run(argc, argv);
	}

	HFLOGINFO("Program finished");

	return result;
}

int MainApp::run(int argc, char** argv) {
	Uf::Init();

	std::map<std::string, std::string> options = Fluxions::MakeOptionsFromArgs(argc, (const char**)argv);

	if (options.count("version")) {
		printf("SSPHH by Jonathan Metzgar\nCopyright (C) 2017-2020 Jonathan Metzgar\n\n");
		printf("This program is free software: you can redistribute it and/or modify\n"
			   "it under the terms of the GNU General Public License as published by\n"
			   "the Free Software Foundation, either version 3 of the License, or\n"
			   "(at your option) any later version.\n\n");
		printf("This program is distributed in the hope that it will be useful,\n"
			   "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
			   "GNU General Public License for more details.\n\n");
		printf("You should have received a copy of the GNU General Public License\n"
			   "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n");
	}

	Fluxions::debugging = false;
	if (options.count("debug")) {
		Fluxions::debugging = true;
	}

	const std::string WindowTitle{ "SSPHH" };
	constexpr int WindowWidth = 1280;
	constexpr int WindowHeight = 720;

#ifdef USE_FREEGLUT
	GlutTemplateSetParameters(WindowTitle, WindowWidth, WindowHeight);
	GlutTemplateInit(argc, argv);
#elif USE_GLFW
	GlfwTemplateSetParameters(WindowTitle, WindowWidth, WindowHeight);
	if (!GlfwTemplateInit(argc, argv)) {
		return -1;
	}
#endif

	dragDrop.Init();
	init();
	::ssphh_widget_ptr = ssphh_widget_ptr;
	::animpath_window_ptr = animpath_window_ptr;

#ifdef USE_FREEGLUT
	GlutTemplateWidget(vf_app_ptr);
	GlutTemplateMainLoop();
#elif USE_GLFW
	GlfwTemplateWidget(vf_app_ptr);
	GlfwTemplateMainLoop();
#endif

	kill();
	dragDrop.Kill();

	::animpath_window_ptr.reset();
	::ssphh_widget_ptr.reset();

	Uf::Kill();

	//std::this_thread::sleep_for(std::chrono::seconds(10));

	return 0;
}

// InitApp()
// This function is called prior to the main loop of the application.
// OpenGL is already initialized prior to this call.
//
void MainApp::init() {
	Fluxions::Init();
	Fluxions::EnableGLDebugFunc();
	constexpr int testwidgets = 0;
	if (testwidgets) {
		vf_app_ptr = std::make_shared<Vf::RootWindow>("root");
		imgui_widget_ptr = std::make_shared<Vf::DearImGuiWidget>("imguiwidget");

		// vf_app decorates imgui_widget because
		// - imgui_widget needs to initialize first
		// - vf_app calls decoratee first, then children
		vf_app_ptr->decorateWith(imgui_widget_ptr);
		Vf::LoadingWindowPtr loadingWindow = std::make_shared<Vf::LoadingWindow>("Fluxions");
		vf_app_ptr->push_back(loadingWindow);
		Vf::StatsWindowPtr statsWindow = std::make_shared<Vf::StatsWindow>("Statistics");
		vf_app_ptr->push_back(statsWindow);
		Vf::AnimationWindowPtr animWindow = Vf::MakeSharedChild<Vf::AnimationWindow>(vf_app_ptr, "Animation");
	} else {
		vf_app_ptr = Vf::MakeShared<Vf::RootWindow>("root");
		imgui_widget_ptr = Vf::MakeSharedDecorator<Vf::DearImGuiWidget>(vf_app_ptr, "DearImGui");
		ssphh_widget_ptr = Vf::MakeSharedChild<SSPHH::SSPHH_Application>(vf_app_ptr, "ssphh");
		// loading_window_ptr = Vf::MakeSharedChild<Vf::LoadingWindow>(vf_app_ptr, "Loading");

		help_window_ptr = Vf::MakeSharedChild<HelpWindow>(vf_app_ptr, "Help");
		stats_window_ptr = Vf::MakeSharedChild<Vf::StatsWindow>(vf_app_ptr, "Statistics");
		scene_graph_window_ptr = Vf::MakeSharedChild<SceneGraphWindow>(vf_app_ptr, "Scene Graph");
		scene_editor_window_ptr = Vf::MakeSharedChild<SceneEditorWindow>(vf_app_ptr, "Scene Editor");
		ssphh_window_ptr = Vf::MakeSharedChild<SsphhWindow>(vf_app_ptr, "SSPHH Algorithm");
		animpath_window_ptr = Vf::MakeSharedChild<Vf::AnimPathWindow>(vf_app_ptr, "Animation Path");
		animation_window_ptr = Vf::MakeSharedChild<Vf::AnimationWindow>(vf_app_ptr, "Animation");
		renderer_config_window_ptr = Vf::MakeSharedChild<RendererConfigWindow>(vf_app_ptr, "Renderer Config");
		renderer_window_ptr = Vf::MakeSharedChild<RendererWindow>(vf_app_ptr, "Renderer");
		ssphh_menu_ptr = Vf::MakeSharedChild<SsphhMenu>(vf_app_ptr, "Renderer");
		tool_window_ptr = Vf::MakeSharedChild<ToolWindow>(vf_app_ptr, "Tools");
		unicornfish_window_ptr = Vf::MakeSharedChild<UnicornfishWindow>(vf_app_ptr, "Unicornfish");

		ssphh_widget_ptr->hotkeyWindows["F1"] = help_window_ptr;
		ssphh_widget_ptr->hotkeyWindows["F2"] = stats_window_ptr;
		ssphh_widget_ptr->hotkeyWindows["F3"] = scene_graph_window_ptr;
		ssphh_widget_ptr->hotkeyWindows["F4"] = scene_editor_window_ptr;
		ssphh_widget_ptr->hotkeyWindows["F5"] = ssphh_window_ptr;
		ssphh_widget_ptr->hotkeyWindows["F6"] = animpath_window_ptr;
		ssphh_widget_ptr->hotkeyWindows["F7"] = animation_window_ptr;
		ssphh_widget_ptr->hotkeyWindows["F8"] = renderer_config_window_ptr;
		ssphh_widget_ptr->hotkeyWindows["F9"] = renderer_window_ptr;
		ssphh_widget_ptr->hotkeyWindows["F10"] = ssphh_menu_ptr;
		ssphh_widget_ptr->hotkeyWindows["F11"] = tool_window_ptr;
		ssphh_widget_ptr->hotkeyWindows["F12"] = unicornfish_window_ptr;

		for (auto& [k, w] : ssphh_widget_ptr->hotkeyWindows) {
			w->Hide();
		}
	}
}

void MainApp::kill() {
	ssphh_widget_ptr->hotkeyWindows.clear();

	vf_app_ptr->Kill();

	loading_window_ptr.reset();
	help_window_ptr.reset();
	stats_window_ptr.reset();
	scene_graph_window_ptr.reset();
	scene_editor_window_ptr.reset();
	ssphh_window_ptr.reset();
	animpath_window_ptr.reset();
	animation_window_ptr.reset();
	renderer_config_window_ptr.reset();
	renderer_window_ptr.reset();
	ssphh_menu_ptr.reset();
	tool_window_ptr.reset();
	unicornfish_window_ptr.reset();

	imgui_widget_ptr.reset();
	ssphh_widget_ptr.reset();
	vf_app_ptr.reset();

	Fluxions::Kill();
}
