#ifndef SSPHHAPP_RENDERCONFIGS_HPP
#define SSPHHAPP_RENDERCONFIGS_HPP

#include <viperfish_window.hpp>

class Fluxions::RendererContext;

class RendererConfigWindow : public Vf::Window {
public:
	RendererConfigWindow(const std::string& name);
	~RendererConfigWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;


private:
	Fluxions::RendererContext* context{ nullptr };
	int curRendererConfigIndex{ 0 };
	std::vector<const char*> renderConfigList;
	Fluxions::RendererConfig* rc{ nullptr };
	float defaultParameterWidth{ 100.0f };
};

using RendererConfigWindowPtr = std::shared_ptr<RendererConfigWindow>;

#endif