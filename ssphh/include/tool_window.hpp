#ifndef TOOL_WINDOW_HPP
#define TOOL_WINDOW_HPP

#include <viperfish_window.hpp>
#include <fluxions_simple_scene_graph.hpp>

class ToolWindow : public Vf::Window {
public:
	ToolWindow(const std::string& name);
	~ToolWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;


private:
	Fluxions::SimpleSceneGraph* ssg{ nullptr };
};

using ToolWindowPtr = std::shared_ptr<ToolWindow>;

#endif
