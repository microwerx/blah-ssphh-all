#ifndef CAMERA_ANIMATION_HPP
#define CAMERA_ANIMATION_HPP

#include <fluxions_ssg_path_animation.hpp>
#include <viperfish_animpath_window.hpp>

class CameraAnimation : public Fluxions::SimplePathAnimation {
public:
	void calcgraph(Vf::AnimPathWindowPtr& mw);
};


#endif
