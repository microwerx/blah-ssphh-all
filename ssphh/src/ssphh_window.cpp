#include "pch.hpp"
#include <ssphhapp.hpp>
#include <ssphh_window.hpp>

SsphhWindow::SsphhWindow(const std::string& name)
	: Vf::Window(name) {}

SsphhWindow::~SsphhWindow() {}

void SsphhWindow::OnUpdate(double timestamp) {
	if (!ssphh_widget_ptr) {
		ssg = nullptr;
		return;
	}
	else if (ssg != &ssphh_widget_ptr->ssg) {
		ssg = &ssphh_widget_ptr->ssg;
	}
	if (!ssg) return;
	Vf::Window::OnUpdate(timestamp);
}

void SsphhWindow::OnRenderDearImGui() {
	if (!ssg) return;
	if (!beginWindow()) return;
	Vf::Window::OnRenderDearImGui();
	auto& app = *ssphh_widget_ptr;
	
	ImGui::Text("Scene: %s", ssg->name());

	if (ImGui::Button("Generate Corona REF")) {
		app.imguiCoronaGenerateSCN();
	}
	if (ImGui::Button("HOSEK-WILKIE")) {
		app.Sky_RegenCoronaSky();
	}
	ImGui::SameLine();
	ImGui::Text("Create/use Corona Hosek-Wilkie sky.");

	if (ImGui::Button("REFERENCE")) {
		app.imguiCoronaGenerateREF();
	}
	ImGui::SameLine();
	ImGui::Text("Create reference for comparison (%.3lf sec)", app.Interface.ssphh.lastREFTime);

	if (ImGui::Button("DELETE CACHE")) {
		app.imguiCoronaDeleteCache();
	}
	ImGui::SameLine();
	ImGui::Text("Delete cached light solution");

	if (ImGui::Button("HIERGEN INIT")) {
		app.imguiCoronaGenerateSphlINIT();
	}
	ImGui::SameLine();
	ImGui::Text("Reset Hierarchies (%.3lf sec)", app.Interface.ssphh.lastINITTime);

	if (ImGui::Button("SPHLVIZ")) {
		app.imguiCoronaGenerateSphlVIZ();
	}
	ImGui::SameLine();
	ImGui::Text("Generate visibility network (%.3lf sec)", app.Interface.ssphh.lastVIZTime);

	if (ImGui::Button("SPHLGEN")) {
		app.imguiCoronaGenerateSphlGEN();
		app.DirtySPHLs();
		//ssg.MakeSphlsUnclean();
	}
	ImGui::SameLine();
	ImGui::Text("Generate GI solution (%.3lf sec)", app.Interface.ssphh.lastGENTime);

	if (ImGui::Button("HIERGEN")) {
		app.imguiCoronaGenerateSphlHIER();
		app.DirtySPHLs();
		//ssg.MakeSphlsUnclean();
	}
	ImGui::SameLine();
	ImGui::Text("Generate Hierarchies (%.3lf sec)", app.Interface.ssphh.lastHIERTime);

	if (ImGui::Button("SAVEOBJ")) {
		app.imguiSphlSaveToOBJ();
	}
	ImGui::SameLine();
	ImGui::Text("Save current SPHL(s) to OBJ/MTL");

	ImGui::Separator();

	ImGui::Checkbox("Sphl Editor", &app.Interface.tools.showSphlEditor);
	ImGui::PushID(1234);
	ImGui::SameLine();
	if (ImGui::SmallButton("+")) {
		app.imguiSphlAdd();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("-")) {
		app.imguiSphlDelete();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("l-")) {
		app.imguiSphlDecreaseDegrees();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("l+")) {
		app.imguiSphlIncreaseDegrees();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("?")) {
		app.imguiSphlRandomize();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Unclean!")) {
		app.DirtySPHLs();
		//ssg.MakeSphlsUnclean();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Dump Hier")) {
		for (size_t i = 0; i < app.ssgUserData->ssphhLights.size(); i++) {
			auto& sphl = app.ssgUserData->ssphhLights[i];
			sphl.setHierarchyDescription();
			HFLOGINFO("hierarchy %02d %s", sphl.index, sphl.hier_description.c_str());
		}
	}

	// do a little visualization on the current enabled status
	int size = (int)app.ssgUserData->ssphhLights.size();
	std::string bits(app.ssgUserData->ssphhLights.size(), 'n');
	for (int i = 0; i < size; i++)
		if (app.ssgUserData->ssphhLights[i].enabled)
			bits[i] = 'y';
	ImGui::SameLine();
	ImGui::Text("%d %s", size, bits.c_str());

	ImGui::PopID();

	ImGui::Separator();

	ImGui::Checkbox("Enable shadow map VIZ", &app.Interface.ssphh.enableShadowColorMap);
	ImGui::SameLine();
	ImGui::Checkbox("Enable sRGB", &app.rendererContext.rendererConfigs["default"].enableSRGB);
	ImGui::Text("REF:");
	ImGui::SameLine();
	ImGui::Checkbox("2D", &app.Interface.ssphh.enableREF);
	ImGui::SameLine();
	ImGui::Checkbox("Cube", &app.Interface.ssphh.enableREFCubeMap);
	ImGui::SameLine();
	ImGui::Checkbox("HQ", &app.Interface.ssphh.enableHQ);
	ImGui::SameLine();
	ImGui::Checkbox("HDR", &app.Interface.ssphh.enableHDR);
	ImGui::SameLine();
	ImGui::Checkbox("Ks", &app.Interface.ssphh.enableKs);
	ImGui::SameLine();
	ImGui::Checkbox("PPMs", &app.ssphh.savePPMs);
	ImGui::SameLine();
	ImGui::Checkbox("JSONs", &app.ssphh.saveJSONs);

	ImGui::PushItemWidth(100);
	ImGui::PushID(1);
	ImGui::TextColored(Colors::Rose, "VIZ");
	ImGui::SameLine();
	ImGui::SliderInt("MaxRayDepth", &app.Interface.ssphh.VIZ_MaxRayDepth, 0, 25);
	ImGui::SameLine();
	ImGui::SliderInt("PassLimit", &app.Interface.ssphh.VIZ_PassLimit, 1, 100);
	ImGui::SameLine();
	ImGui::Checkbox("Regen", &app.Interface.ssphh.VIZ_IgnoreCache);
	ImGui::PopID();
	ImGui::PushID(2);
	ImGui::TextColored(Colors::Cyan, "GEN");
	ImGui::SameLine();
	ImGui::SliderInt("MaxRayDepth", &app.Interface.ssphh.GEN_MaxRayDepth, 0, 25);
	ImGui::SameLine();
	ImGui::SliderInt("PassLimit", &app.Interface.ssphh.GEN_PassLimit, 1, 100);
	ImGui::SameLine();
	ImGui::Checkbox("Regen", &app.Interface.ssphh.GEN_IgnoreCache);
	ImGui::PopID();
	ImGui::PushID(3);
	ImGui::TextColored(Colors::Yellow, "REF");
	ImGui::SameLine();
	ImGui::SliderInt("MaxRayDepth", &app.Interface.ssphh.REF_MaxRayDepth, 0, 25);
	ImGui::SameLine();
	ImGui::SliderInt("PassLimit", &app.Interface.ssphh.REF_PassLimit, 1, 100);
	ImGui::SameLine();
	ImGui::Checkbox("Regen", &app.Interface.ssphh.REF_IgnoreCache);
	ImGui::PopID();
	app.imgui2NSizeSlider("SPHL Size", &app.Interface.ssphh.LightProbeSizeChoice, &app.Interface.ssphh.LightProbeSize, 4, 10);
	ImGui::SameLine();
	app.imgui2NSizeSlider("Shadow Size", &app.Interface.ssphh.ShadowSizeChoice, &app.Interface.ssphh.ShadowSize, 4, 10);
	ImGui::PopItemWidth();

	ImGui::Separator();

	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Max Hierarchies", &app.Interface.ssphh.HierarchiesMaxSphls, 0, MaxSphlLights);
	ImGui::SameLine();
	ImGui::SliderInt("Max Degrees", &app.Interface.ssphh.MaxDegrees, 0, MaxSphlDegree);
	ImGui::Text("Accum");
	ImGui::SameLine();
	ImGui::Checkbox("Self ", &app.Interface.ssphh.HierarchiesIncludeSelf);
	ImGui::SameLine();
	ImGui::Checkbox("Neighbors", &app.Interface.ssphh.HierarchiesIncludeNeighbors);
	ImGui::SameLine();
	ImGui::SliderFloat("Mix", &app.Interface.ssphh.HierarchiesMix, 0.0, 1.0);
	ImGui::Text("Show ");
	ImGui::SameLine();
	ImGui::Checkbox("SPHLs", &app.Interface.ssphh.enableShowSPHLs);
	ImGui::SameLine();
	ImGui::Checkbox("Basic", &app.Interface.ssphh.enableBasicShowSPHLs);
	ImGui::SameLine();
	ImGui::Checkbox("Hierarchies", &app.Interface.ssphh.enableShowHierarchies);
	ImGui::SameLine();
	if (ImGui::Button("GO!")) {
		app.DirtySPHLs();
		//ssg.MakeSphlsUnclean();
	}
	ImGui::PopItemWidth();

	ImGui::Separator();

	if (ImGui::Button("GEN Test Products")) {
		app.imguiCoronaGenerateTestProducts();
	}
	ImGui::SameLine();
	if (ImGui::Button("DEL Test Products")) {
		app.imguiCoronaDeleteTestProducts();
	}
	ImGui::SameLine();
	ImGui::Checkbox("REGEN Test Products", &app.Interface.ssphh.genProductsIgnoreCache);

	ImGui::Checkbox("PPMCOMP Diffs", &app.Interface.ssphh.ppmcompareGenPPMs);
	ImGui::SameLine();
	ImGui::Checkbox("PPMCOMP Regen", &app.Interface.ssphh.ppmcompareIgnoreCache);

	ImGui::Separator();

	// show sorted list of hierarchies
	for (size_t i = 0; i < app.ssgUserData->ssphhLights.size(); i++) {
		ImGui::Text("%s", app.ssgUserData->ssphhLights[i].hier_description.c_str());
	}

	endWindow();
}
