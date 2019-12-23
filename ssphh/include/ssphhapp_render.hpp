#ifndef SSPHHAPP_RENDERER_WINDOW_HPP
#define SSPHHAPP_RENDERER_WINDOW_HPP

#include <viperfish_window.hpp>

class Fluxions::RendererContext;

class RendererWindow : public Vf::Window {
public:
	RendererWindow(const std::string& name);
	~RendererWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;


private:
	Fluxions::RendererContext* context{ nullptr };
	float lastShadersLoadTime{ 0.0f };
	float lastTextureLoadTime{ 0.0f };
	float lastConfigsLoadTime{ 0.0f };
};

using RendererWindowPtr = std::shared_ptr<RendererWindow>;

#endif
