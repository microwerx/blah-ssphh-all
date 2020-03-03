#ifndef SCENE_EDITOR_WINDOW_HPP
#define SCENE_EDITOR_WINDOW_HPP

#include <fluxions_ssg_scene_graph.hpp>
#include <viperfish_window.hpp>

class SceneEditorWindow : public Vf::Window {
public:
	SceneEditorWindow(const std::string& name);
	~SceneEditorWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;
	void OnKill() override { ssg.reset(); }

private:
	Fluxions::SimpleSceneGraphPtr ssg;
	Fluxions::BaseDirToLight* moon{ nullptr };
	Fluxions::BaseDirToLight* sun{ nullptr };
};

using SceneEditorWindowPtr = std::shared_ptr<SceneEditorWindow>;

#endif
