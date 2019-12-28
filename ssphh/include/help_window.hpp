#ifndef HELP_WINDOW_HPP
#define HELP_WINDOW_HPP

#include <viperfish_window.hpp>
#include <fluxions_simple_scene_graph.hpp>

class HelpWindow : public Vf::Window {
public:
	HelpWindow(const std::string& name);
	~HelpWindow() override;

	void OnInit(const std::vector<std::string>& args) override;
	void OnRenderDearImGui() override;


private:
	using int_string_string = std::tuple<int, std::string, std::string>;
	std::vector<int_string_string> lines;
};

using HelpWindowPtr = std::shared_ptr<HelpWindow>;

#endif