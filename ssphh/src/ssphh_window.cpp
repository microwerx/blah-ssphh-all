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
	else if (ssg != ssphh_widget_ptr->ssg) {
		ssg = ssphh_widget_ptr->ssg;
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

	ImGui::Text("Options: ");
	ImGui::SameLine();
	ImGui::Checkbox("Clear Cache", &app.coronaScene.currentConfig.clearCache);
	ImGui::SameLine();
	ImGui::Checkbox("Use Previous", &app.coronaScene.currentConfig.usePreviousRun);
	ImGui::SameLine();
	ImGui::Checkbox("Enable Specular", &app.coronaScene.currentConfig.enableSpecular);

	if (ImGui::Button("SCENE")) {
		app.PathAnim_Stop();
		app.Corona_GenerateSCN();
	}
	ImGuiAlignAt(buttonWidth, "ABCDEFGHIJKLMNOPQRS");
	ImGui::Text("%s", app.coronaScene.lastSCN.c_str());

	if (ImGui::Button("HOSEK-WILKIE")) {
		app.Sky_RegenCoronaSky();
	}
	ImGuiAlignAt(buttonWidth);
	ImGui::Text("Create/use Corona Hosek-Wilkie sky.");

	if (ImGui::Button("REFERENCE")) {
		app.PathAnim_Stop();
		app.Corona_GenerateREF();
	}
	ImGuiAlignAt(buttonWidth);
	ImGui::Text("Create reference for comparison (%.3lf sec)", app.Interface->ssphh.lastREFTime);

	if (ImGui::Button("SCREENSHOT")) {
		app.PathAnim_Stop();
		app.Interface->saveScreenshot = true;
	}
	ImGuiAlignAt(buttonWidth);
	ImGui::Text("Save screenshot comparison (%.3lf sec)", app.Interface->ssphh.lastAPPTime);

	if (ImGui::Button("DELETE CACHE")) {
		app.Corona_EraseCache();
	}
	ImGuiAlignAt(buttonWidth);
	ImGui::Text("Delete cached light solution (%d files removed)", app.Interface->ssphh.cacheFilesRemoved);

	if (ImGui::Button("HIERGEN INIT")) {
		app.Corona_GenerateSphlINIT();
	}
	ImGuiAlignAt(buttonWidth);
	ImGui::Text("Reset Hierarchies (%.3lf sec)", app.Interface->ssphh.lastINITTime);

	if (ImGui::Button("SPHLVIZ")) {
		app.Corona_GenerateSphlVIZ();
	}
	ImGuiAlignAt(buttonWidth);
	ImGui::Text("Generate visibility network (%.3lf sec)", app.Interface->ssphh.lastVIZTime);

	if (ImGui::Button("SPHLGEN")) {
		app.Corona_GenerateSphlGEN();
		app.DirtySPHLs();
		//ssg.MakeSphlsUnclean();
	}
	ImGuiAlignAt(buttonWidth);
	ImGui::Text("Generate GI solution (%.3lf sec)", app.Interface->ssphh.lastGENTime);

	if (ImGui::Button("HIERGEN")) {
		app.Corona_GenerateSphlHIER();
		app.DirtySPHLs();
		//ssg.MakeSphlsUnclean();
	}
	ImGuiAlignAt(buttonWidth);
	ImGui::Text("Generate Hierarchies (%.3lf sec)", app.Interface->ssphh.lastHIERTime);

	if (ImGui::Button("SAVEOBJ")) {
		app.imguiSphlSaveToOBJ();
	}
	ImGuiAlignAt(buttonWidth);
	ImGui::Text("Save current SPHL(s) to OBJ/MTL");

	ImGui::Separator();

	ImGui::Checkbox("Sphl Editor", &app.Interface->tools.showSphlEditor);
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
		for (size_t i = 0; i < app.ssgUserData->anisoLights.size(); i++) {
			auto& sphl = app.ssgUserData->anisoLights[i];
			//sphl->setHierarchyDescription();
			//HFLOGINFO("hierarchy %02d %s", sphl->index, sphl->hier_description.c_str());
		}
	}

	// do a little visualization on the current enabled status
	int size = (int)app.ssgUserData->anisoLights.size();
	std::string bits(app.ssgUserData->anisoLights.size(), 'n');
	for (int i = 0; i < size; i++)
		if (app.ssgUserData->anisoLights[i]->enabled())
			bits[i] = 'y';
	ImGui::SameLine();
	ImGui::Text("%d %s", size, bits.c_str());

	ImGui::PopID();

	ImGui::Separator();

	ImGui::Checkbox("Enable shadow map VIZ", &app.Interface->ssphh.enableShadowColorMap);
	//ImGui::SameLine();
	//ImGui::Checkbox("Enable sRGB", &app.rendererContext->rendererConfigs["default"]->enableSRGB);
	
	ImGui::Text("REF:");
	ImGuiAlignAt(vgrWidth, "ABCD");
	ImGui::Checkbox("2D", &app.Interface->ssphh.enableREF);
	ImGui::SameLine();
	ImGui::Checkbox("Cube", &app.Interface->ssphh.enableREFCubeMap);
	ImGui::SameLine();
	ImGui::Checkbox("HQ", &app.coronaScene.currentConfig.enableHQ);
	ImGui::SameLine();
	ImGui::Checkbox("HDR", &app.coronaScene.currentConfig.enableHDR);
	ImGui::SameLine();
	ImGui::Checkbox("Ks", &app.coronaScene.currentConfig.enableSpecular);
	ImGui::SameLine();
	ImGui::Checkbox("PPMs", &app.ssphh.savePPMs);
	ImGui::SameLine();
	ImGui::Checkbox("JSONs", &app.ssphh.saveJSONs);

	ImGui::PushItemWidth(100);
	ImGui::PushID(1);
	ImGui::TextColored(Colors::Rose, "VIZ");
	ImGuiAlignAt(vgrWidth);
	ImGui::SliderInt("MaxRayDepth", &app.coronaScene.VIZ.maxRayDepth, 0, 25);
	ImGui::SameLine();
	ImGui::SliderInt("PassLimit", &app.coronaScene.VIZ.passLimit, 1, 100);
	ImGui::SameLine();
	ImGui::Checkbox("Use Previous", &app.coronaScene.VIZ.usePreviousRun);
	ImGui::PopID();
	ImGui::PushID(2);
	ImGui::TextColored(Colors::Cyan, "GEN");
	ImGuiAlignAt(vgrWidth);
	ImGui::SliderInt("MaxRayDepth", &app.coronaScene.GEN.maxRayDepth, 0, 25);
	ImGui::SameLine();
	ImGui::SliderInt("PassLimit", &app.coronaScene.GEN.passLimit, 1, 100);
	ImGui::SameLine();
	ImGui::Checkbox("Use Previous", &app.coronaScene.GEN.usePreviousRun);
	ImGui::PopID();
	ImGui::PushID(3);
	ImGui::TextColored(Colors::Yellow, "REF");
	ImGuiAlignAt(vgrWidth);
	ImGui::SliderInt("MaxRayDepth", &app.coronaScene.REF.maxRayDepth, 0, 25);
	ImGui::SameLine();
	ImGui::SliderInt("PassLimit", &app.coronaScene.REF.passLimit, 1, 100);
	ImGui::SameLine();
	ImGui::Checkbox("Use Previous", &app.coronaScene.REF.usePreviousRun);
	ImGui::PopID();
	ImGui2NSlider("SPHL Size", &app.Interface->ssphh.LightProbeSizeChoice, &app.Interface->ssphh.LightProbeSize, 4, 10);
	ImGui::SameLine();
	ImGui2NSlider("Shadow Size", &app.Interface->ssphh.ShadowSizeChoice, &app.Interface->ssphh.ShadowSize, 4, 10);
	ImGui::PopItemWidth();

	ImGui::Separator();

	ImGui::PushItemWidth(100);
	ImGui::SliderInt("Max Hierarchies", &app.Interface->ssphh.HierarchiesMaxSphls, 0, MaxSphlLights);
	ImGui::SameLine();
	ImGui::SliderInt("Max Degrees", &app.Interface->ssphh.MaxDegrees, 0, MaxSphlDegree);
	ImGui::Text("Accum");
	ImGui::SameLine();
	ImGui::Checkbox("Self ", &app.Interface->ssphh.HierarchiesIncludeSelf);
	ImGui::SameLine();
	ImGui::Checkbox("Neighbors", &app.Interface->ssphh.HierarchiesIncludeNeighbors);
	ImGui::SameLine();
	ImGui::SliderFloat("Mix", &app.Interface->ssphh.HierarchiesMix, 0.0, 1.0);
	ImGui::Text("Show ");
	ImGui::SameLine();
	ImGui::Checkbox("SPHLs", &app.Interface->ssphh.enableShowSPHLs);
	ImGui::SameLine();
	ImGui::Checkbox("Basic", &app.Interface->ssphh.enableBasicShowSPHLs);
	ImGui::SameLine();
	ImGui::Checkbox("Hierarchies", &app.Interface->ssphh.enableShowHierarchies);
	ImGui::SameLine();
	if (ImGui::Button("GO!")) {
		app.DirtySPHLs();
	}
	ImGui::PopItemWidth();

	ImGui::Separator();

	if (ImGui::Button("GEN Test Products")) {
		app.Corona_GenerateTestProducts();
	}
	ImGui::SameLine();
	if (ImGui::Button("DEL Test Products")) {
		app.Corona_EraseTestProducts();
	}
	ImGui::SameLine();
	ImGui::Checkbox("REGEN Test Products", &app.Interface->ssphh.genProductsIgnoreCache);

	ImGui::Checkbox("PPMCOMP Diffs", &app.Interface->ssphh.ppmcompareGenPPMs);
	ImGui::SameLine();
	ImGui::Checkbox("PPMCOMP Regen", &app.Interface->ssphh.ppmcompareIgnoreCache);

	ImGui::Separator();

	// show sorted list of hierarchies
	for (size_t i = 0; i < app.ssgUserData->anisoLights.size(); i++) {
		//ImGui::Text("%s", app.ssgUserData->anisoLights[i]->hier_description.c_str());
	}

	endWindow();
}
