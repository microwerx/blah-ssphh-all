#ifndef SSPHHAPP_RENDERCONFIGS_HPP
#define SSPHHAPP_RENDERCONFIGS_HPP

#include <viperfish_window.hpp>

class RendererWindow : public Vf::Window {
public:
	RendererWindow(const std::string& name);
	~RendererWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;


private:
	RendererContext* context{ nullptr };
	float lastShadersLoadTime{ 0.0f };
	float lastTextureLoadTime{ 0.0f };
	float lastConfigsLoadTime{ 0.0f };
};

using RendererWindowPtr = std::shared_ptr<RendererWindow>;

#endif