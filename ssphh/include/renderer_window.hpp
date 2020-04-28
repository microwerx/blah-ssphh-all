#ifndef SSPHHAPP_RENDERER_WINDOW_HPP
#define SSPHHAPP_RENDERER_WINDOW_HPP

#include <fluxions_renderer_context.hpp>
#include <viperfish_window.hpp>

class RendererWindow : public Vf::Window {
public:
	RendererWindow(const std::string& name);
	~RendererWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;
	void OnKill() override { rendererContext.reset(); }

private:
	Fluxions::RendererContextPtr rendererContext;
	float lastShadersLoadTime{ 0.0f };
	float lastTextureLoadTime{ 0.0f };
	float lastConfigsLoadTime{ 0.0f };
};

using RendererWindowPtr = std::shared_ptr<RendererWindow>;

#endif
