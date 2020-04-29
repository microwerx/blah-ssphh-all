#ifndef UNICORNFISH_WINDOW_HPP
#define UNICORNFISH_WINDOW_HPP

#include <fluxions_ssg_scene_graph.hpp>
#include <viperfish_window.hpp>

class UnicornfishWindow : public Vf::Window {
public:
	UnicornfishWindow(const std::string& name);
	~UnicornfishWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;
	void OnKill() override { ssg.reset(); }

private:
	Fluxions::SimpleSceneGraphPtr ssg;
};

using UnicornfishWindowPtr = std::shared_ptr<UnicornfishWindow>;

#endif
