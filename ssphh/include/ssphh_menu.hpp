#ifndef SSPHH_MENU_HPP
#define SSPHH_MENU_HPP

#include <viperfish_window.hpp>
#include <fluxions_ssg_scene_graph.hpp>

class SsphhMenu : public Vf::Window {
public:
	SsphhMenu(const std::string& name);
	~SsphhMenu() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;


private:
	Fluxions::SimpleSceneGraph* ssg{ nullptr };
};

using SsphhMenuPtr = std::shared_ptr<SsphhMenu>;

#endif
