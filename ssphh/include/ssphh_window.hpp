#ifndef SSPHH_WINDOW_HPP
#define SSPHH_WINDOW_HPP

#include <viperfish_window.hpp>
#include <fluxions_simple_scene_graph.hpp>

class SsphhWindow : public Vf::Window {
public:
	SsphhWindow(const std::string& name);
	~SsphhWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;

private:
	Fluxions::SimpleSceneGraph* ssg{ nullptr };
	float buttonWidth{ 0.0f };
	float vgrWidth{ 0.0f };
	void alignat(float& width, const char* referencestr = nullptr);
};

using SSPHHWindowPtr = std::shared_ptr<SsphhWindow>;

#endif
