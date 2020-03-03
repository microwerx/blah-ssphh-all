#ifndef TOOL_WINDOW_HPP
#define TOOL_WINDOW_HPP

#include <fluxions_ssg_scene_graph.hpp>
#include <viperfish_window.hpp>

class ToolWindow : public Vf::Window {
public:
	ToolWindow(const std::string& name);
	~ToolWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;
	void OnKill() override { ssg.reset(); }

private:
	Fluxions::SimpleSceneGraphPtr ssg;
};

using ToolWindowPtr = std::shared_ptr<ToolWindow>;

#endif
