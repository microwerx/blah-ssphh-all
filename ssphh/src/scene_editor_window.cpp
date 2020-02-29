#include "pch.hpp"
#include <ssphhapp.hpp>
#include <scene_editor_window.hpp>

SceneEditorWindow::SceneEditorWindow(const std::string& name)
	: Vf::Window(name) {}

SceneEditorWindow::~SceneEditorWindow() {}

void SceneEditorWindow::OnUpdate(double timestamp) {
	if (!ssphh_widget_ptr) {
		ssg->reset();
		return;
	}
	else if (ssg != ssphh_widget_ptr->ssg) {
		ssg = ssphh_widget_ptr->ssg;
	}
	if (!ssg) return;

	if (!sun)
		sun = ssg->dirToLights.getPtr("sun");

	if (!moon)
		moon = ssg->dirToLights.getPtr("moon");

	Vf::Window::OnUpdate(timestamp);
}

void SceneEditorWindow::OnRenderDearImGui() {
	// Note: Scene Editor Window is mapped to F4
	if (!ssg) return;
	if (!beginWindow()) return;
	Vf::Window::OnRenderDearImGui();

	Fluxions::BaseEnvironment* e = (Fluxions::BaseEnvironment*) & ssg->environment;
	SSPHH::SSPHH_Application* app = ssphh_widget_ptr.get();
	ImGui::Checkbox("Sun Cycle", &app->Interface.enableSunCycle);
	if (ImGui::TreeNode("Environment")) {
		ImGui::Checkbox("Has Sun", &ssg->environment.hasSun);
		ImGui::Checkbox("Has Moon", &ssg->environment.hasMoon);
		Vf::ImGuiValue4f("tonemap ", e->toneMap);
		Vf::ImGuiValue4f("fog     ", e->fogSH);
		Vf::ImGuiValue4f("ground  ", e->ground);
		Vf::ImGuiValue4f("location", e->location);
		Vf::ImGuiValue4f("date    ", e->date);
		Vf::ImGuiValue4f("time    ", e->time);
		ImGui::DragFloat("yyyy", e->date.ptr() + 0, 1.0f, 2010.0f, 2030.0f, "%04f");
		ImGui::DragFloat("mmm", e->date.ptr() + 1, 1.0f, 1.0f, 12.0f, "%03f");
		ImGui::DragFloat("ddd", e->date.ptr() + 2, 1.0f, 1.0f, 30.0f, "%02f");
		ImGui::DragFloat("hh", e->time.ptr() + 0, 1.0f, 0.0f, 23.0f, "%2f");
		ImGui::DragFloat("mm", e->time.ptr() + 1, 1.0f, 0.0f, 59.0f, "%2f");
		ImGui::DragFloat("ss", e->time.ptr() + 2, 1.0f, 0.0f, 59.0f, "%2f");
		ImGui::DragFloat("lat", e->location.ptr() + 0, 1.0f, -90.0f, 90.0f, "% 3.2f");
		ImGui::DragFloat("lon", e->location.ptr() + 1, 1.0f, -180.0f, 180.0f, "% 3.2f");
		ImGui::DragFloat("agl", e->location.ptr() + 2, 1.0f, 0.0f, 100.0f, "% 3.2f");
		ImGui::DragFloat("alt", e->location.ptr() + 3, 1.0f, 0.0f, 100.0f, "% 3.2f");
		ImGui::TreePop();
	}

	if (sun && ImGui::TreeNode("Sun")) {
		SimpleDirToLight& sun = ssg->dirToLights["sun"];
		ImGui::TextColored(Colors::Azure, "Sun");
		ImGui::ColorEdit3("sunE0", sun.E0.ptr());
		ImGui::DragFloat3("sunDirTo", sun.dirTo.ptr(), 0.01f, -1.0f, 1.0f);
		if (ImGui::Button("unit")) sun.dirTo.normalize();
		if (ImGui::Button("real")) {
			sun.dirTo = ssg->environment.curSunDirTo;
		}
		ImGui::TreePop();
	}

	if (moon && ImGui::TreeNode("Moon")) {
		ImGui::TextColored(Colors::Azure, "Moon");
		ImGui::ColorEdit3("moonE0", moon->E0.ptr());
		ImGui::DragFloat3("moonDirTo", moon->dirTo.ptr(), 0.01f, -1.0f, 1.0f);
		if (ImGui::Button("unit")) moon->dirTo.normalize();
		if (ImGui::Button("real")) {
			moon->dirTo = ssg->environment.curMoonDirTo;
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("AstroCalc")) {
		auto cdt = ssg->environment.pbsky.GetCivilDateTime();
		Color3f modis = ssg->environment.pbsky.computeModisAlbedo(false);
		auto sunRADec = ssg->environment.pbsky.getSunRADec();
		auto moonRADec = ssg->environment.pbsky.getMoonRADec();
		float lat = ssg->environment.pbsky.GetLatitude();
		float lon = ssg->environment.pbsky.GetLongitude();
		const char* NS = lat < 0 ? "S" : "N";
		const char* EW = lat < 0 ? "W" : "E";
		ImGui::LabelText("lat/lon", "%2.2f%s %3.2f%s", std::abs(lat), NS, std::abs(lon), EW);
		if (ImGui::SmallButton("MODIS")) {
			ssg->environment.pbsky.computeModisAlbedo(true);
		}
		ImGui::SameLine(); ImGui::ColorEdit3("MODIS", modis.ptr());
		// TODO: print \u0251 for alpha, \u03B4 for delta
		ImGui::LabelText("Moon RA", "a: % 03.4f, d: % 03.4f", sunRADec.alpha, sunRADec.delta);
		ImGui::LabelText("Moon Dec", "a: % 03.4f, d: % 03.4f", moonRADec.alpha, moonRADec.delta);
		ImGui::TreePop();
	}

	endWindow();
}
