#ifndef SSPHH_WINDOW_HPP
#define SSPHH_WINDOW_HPP

#include <fluxions_ssg_scene_graph.hpp>
#include <viperfish_window.hpp>

class SsphhWindow : public Vf::Window {
public:
	SsphhWindow(const std::string& name);
	~SsphhWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;
	void OnKill() override { ssg.reset(); }

private:
	Fluxions::SimpleSceneGraphPtr ssg;
	float buttonWidth{ 0.0f };
	float vgrWidth{ 0.0f };
	void alignat(float& width, const char* referencestr = nullptr);
};

using SSPHHWindowPtr = std::shared_ptr<SsphhWindow>;

#endif
