#ifndef SCENE_GRAPH_WINDOW_HPP
#define SCENE_GRAPH_WINDOW_HPP

#include <viperfish_window.hpp>

class SceneGraphWindow : public Vf::Window {
public:
	SceneGraphWindow(const std::string& name);
	~SceneGraphWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;
	void OnKill() override { ssg.reset(); }

private:
	Fluxions::SimpleSceneGraphPtr ssg;
};

using SceneGraphWindowPtr = std::shared_ptr<SceneGraphWindow>;

#endif
