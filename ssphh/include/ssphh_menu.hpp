#ifndef SSPHH_MENU_HPP
#define SSPHH_MENU_HPP

#include <fluxions_ssg_scene_graph.hpp>
#include <viperfish_window.hpp>

class SsphhMenu : public Vf::Window {
public:
	SsphhMenu(const std::string& name);
	~SsphhMenu() override;

	void OnUpdate(double timeStamp) override;
	void OnRenderDearImGui() override;
	void OnKill() override { ssg.reset(); }

private:
	Fluxions::SimpleSceneGraphPtr ssg;
};

using SsphhMenuPtr = std::shared_ptr<SsphhMenu>;

#endif
