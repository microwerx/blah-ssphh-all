// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// PhD Scalable Spherical Harmonics Hierarchies
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

#define USE_GLFW 1
//#define USE_FREEGLUT 1

#ifdef _WIN32
#define _STDCALL_SUPPORTED

#pragma comment(lib, "xinput.lib")		// XInput Library
#pragma comment(lib, "glu32.lib")		// OpenGL Utility library
#pragma comment(lib, "opengl32.lib")	// Microsoft OpenGL library
#pragma comment(lib, "gdi32.lib")		// Windows GDI library
#pragma comment(lib, "winmm.lib")		// Windows MultiMedia library
#pragma comment(lib, "user32.lib")		// Windows User library

#ifndef FLUXIONS_NO_SDL
#ifdef _DEBUG
#pragma comment(lib, "SDL2d.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_mixer.lib")
#else
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_mixer.lib")
#endif
#endif

#pragma comment(lib, "fluxions.lib")

#ifndef FLUXIONS_NO_OPENEXR
#ifdef _DEBUG
#pragma comment(lib, "Half-2_3_d.lib")
#pragma comment(lib, "IlmImf-2_3_d.lib")
#else
#pragma comment(lib, "Half-2_3.lib")
#pragma comment(lib, "IlmImf-2_3.lib")
#endif // _DEBUG
//#pragma comment(lib, "Iex-2_3")
//#pragma comment(lib, "IexMath-2_3")
//#pragma comment(lib, "IlmImfUtil-2_3")
//#pragma comment(lib, "IlmThread-2_3")
//#pragma comment(lib, "Imath-2_3")
#endif


//#ifdef _WIN32
//#pragma comment(lib, "libsodium.lib")
//#pragma comment(lib, "libzmq.lib")
//#pragma comment(lib, "libczmq.lib")
//#pragma comment(lib, "libcurl.lib")
//#endif

#ifdef _DEBUG
//#pragma comment(lib, "libzmq-mt-gd-4_3_3.lib")
#pragma comment(lib, "glew32d.lib")
#pragma comment(lib, "glfw3dll.lib")
#else
#pragma comment(lib, "libzmq-mt-4_3_3.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")
#endif // _DEBUG


#ifdef SSPHH_USE_FREEGLUT
#ifdef _DEBUG
#pragma comment(lib, "freeglut_staticd.lib")
#else
#pragma comment(lib, "freeglut_static.lib")
#endif // _DEBUG
#endif // SSPHH_USE_FREEGLUT

#endif // _WIN32

#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fluxions.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <regex>
#include <viperfish.hpp>
#include <viperfish_root_window.hpp>
#include <viperfish_loading_window.hpp>
#include <viperfish_stats_window.hpp>
#include <viperfish_animation_window.hpp>
#include <unicornfish.hpp>
#include <ssphhapp.hpp>

#include <DragDrop.hpp>
#include <GLFW_template.hpp>
#include <GLUT_template.hpp>

#ifndef FLUXIONS_NO_SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

extern void do_tests();

Vf::WidgetPtr vf_app_ptr;
Vf::WidgetPtr imgui_widget_ptr;
Vf::LoadingWindowPtr loading_window_ptr;
Vf::StatsWindowPtr stats_window_ptr;
Vf::AnimationWindowPtr animation_window_ptr;

double g_distance = -10.0;
double xrot = 0.0;
double yrot = 0.0;

void InitApp();
void KillApp();

namespace Fluxions
{
	extern int test_fluxions_simple_property(int argc, char **argv);
}

namespace Df
{
	extern int test_PythonInterpreter(int argc, char **argv);
}

int main(int argc, char **argv)
{
	do_tests();

	Uf::Init();

	std::map<std::string, std::string> options = Fluxions::MakeOptionsFromArgs(argc, (const char **)argv);

	if (options.count("version")) {
		printf("SSPHH by Jonathan Metzgar\nCopyright (C) 2017 Jonathan Metzgar\n\n");
		printf("This program is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\n");
		printf("This program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\n");
		printf("You should have received a copy of the GNU General Public License\nalong with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n");
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
	InitApp();

#ifdef USE_FREEGLUT
	GlutTemplateWidget(vf_app_ptr);
	GlutTemplateMainLoop();
#elif USE_GLFW
	GlfwTemplateWidget(vf_app_ptr);
	GlfwTemplateMainLoop();
#endif

	KillApp();
	dragDrop.Kill();

	Uf::Kill();

	return 0;
}

// InitApp()
// This function is called prior to the main loop of the application.
// OpenGL is already initialized prior to this call.
//
void InitApp()
{
	Fluxions::Init();
	Fluxions::EnableGLDebugFunc();
	constexpr int testwidgets = 1;
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
	}
	else {
		vf_app_ptr = Vf::MakeShared<Vf::RootWindow>("root");
		imgui_widget_ptr = Vf::MakeSharedDecorator<Vf::DearImGuiWidget>(vf_app_ptr, "DearImGui");
		ssphh_widget_ptr = Vf::MakeSharedChild<SSPHH::SSPHH_Application>(vf_app_ptr, "ssphh");
		loading_window_ptr = Vf::MakeSharedChild<Vf::LoadingWindow>(vf_app_ptr, "Loading");
		stats_window_ptr = Vf::MakeSharedChild<Vf::StatsWindow>(vf_app_ptr, "Statistics");
		animation_window_ptr = Vf::MakeSharedChild<Vf::AnimationWindow>(vf_app_ptr, "Animation");
	}
}

void KillApp()
{
	Fluxions::Kill();
}