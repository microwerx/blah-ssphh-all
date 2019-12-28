#ifndef UNICORNFISH_WINDOW_HPP
#define UNICORNFISH_WINDOW_HPP

#include <viperfish_window.hpp>
#include <fluxions_simple_scene_graph.hpp>

class UnicornfishWindow : public Vf::Window {
public:
	UnicornfishWindow(const std::string& name);
	~UnicornfishWindow() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;


private:
	Fluxions::SimpleSceneGraph* ssg{ nullptr };
};

using UnicornfishWindowPtr = std::shared_ptr<UnicornfishWindow>;

#endif