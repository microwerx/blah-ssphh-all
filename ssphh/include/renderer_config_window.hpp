#ifndef SSPHHAPP_RENDERCONFIGS_HPP
#define SSPHHAPP_RENDERCONFIGS_HPP

#include <fluxions_renderer_config.hpp>
#include <viperfish_window.hpp>

class RendererConfigWindow : public Vf::Window {
public:
	RendererConfigWindow(const std::string& name);
	~RendererConfigWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;
	void OnKill() override {
		rendererContext.reset();
	}


private:
	Fluxions::RendererContextPtr rendererContext;
	int curRendererConfigIndex{ 0 };
	std::vector<const char*> renderConfigList;
	Fluxions::RendererConfigWeakPtr rc_wptr;
	float defaultParameterWidth{ 100.0f };
};

using RendererConfigWindowPtr = std::shared_ptr<RendererConfigWindow>;

#endif
