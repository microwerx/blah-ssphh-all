#ifndef UNICORNFISH_WINDOW_HPP
#define UNICORNFISH_WINDOW_HPP

#include <viperfish_window.hpp>
#include <fluxions_ssg_scene_graph.hpp>

class UnicornfishWindow : public Vf::Window {
public:
	UnicornfishWindow(const std::string& name);
	~UnicornfishWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;


private:
	Fluxions::SimpleSceneGraphPtr ssg;
};

using UnicornfishWindowPtr = std::shared_ptr<UnicornfishWindow>;

#endif
