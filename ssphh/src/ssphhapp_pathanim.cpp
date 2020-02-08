#include "pch.hpp"
#include <ssphhapp.hpp>
#include <viperfish_animpath_window.hpp>

extern Vf::AnimPathWindowPtr animpath_window_ptr;

namespace SSPHH
{
	const std::string default_animation_path{ "resources/anim/animation.txt" };

	void SSPHH_Application::PathAnim_LoadCameraPath(const std::string& path) {
		cameraAnimation.clear();
		std::ifstream fin;
		if (path.empty())
			fin.open(default_animation_path);
		else
			fin.open(path);
		while (fin) {
			std::string line;
			std::getline(fin, line);
			std::istringstream istr(line);
			std::string token;
			istr >> token;
			if (token.empty()) continue;
			cameraAnimation.read(token, istr);
		}
	}

	void SSPHH_Application::PathAnim_SaveCameraPath(const std::string& path) {
		std::ofstream fout(path);
		cameraAnimation.write(fout);
	}

	void SSPHH_Application::PathAnim_Play() {
		Interface.enableAnimation = true;
	}

	void SSPHH_Application::PathAnim_Stop() {
		Interface.enableAnimation = false;
	}

	void SSPHH_Application::PathAnim_Toggle() {
		Interface.enableAnimation = !Interface.enableAnimation;
	}

	void SSPHH_Application::PathAnim_Update() {
		if (!animpath_window_ptr) return;

		cameraAnimationTime = animpath_window_ptr->t;
		cameraAnimationSpeed = animpath_window_ptr->speed;
		cameraAnimationKeyframe = animpath_window_ptr->key;
		int i = cameraAnimationKeyframe;

		if (Interface.enableAnimation) {
			cameraAnimationTime += cameraAnimationSpeed * (float)GetFrameTime();
			if (cameraAnimationTime > (float)cameraAnimation.size()) {
				cameraAnimationTime -= std::floor(cameraAnimationTime);
			}
		}

		constexpr float fps = 60.0f;
		constexpr float invfps = 1.0f / fps;
		constexpr float microunit = 0.1f / fps;
		float t = cameraAnimationTime;
		float keyt = std::floor(t * fps) / fps;
		float sect = std::floor(t);
		switch (animpath_window_ptr->action) {
		case Vf::PathAnimActions::GoToStart:
			t = 0.0f;
			break;
		case Vf::PathAnimActions::GoToPrevKeyFrame:
			t = (t > sect) ? sect : sect - 1.0f;
			break;
		case Vf::PathAnimActions::GoToPrevFrame:
			t = (t > keyt) ? keyt : keyt - invfps;
			break;
		case Vf::PathAnimActions::GoToPrevMicroUnit:
			t -= microunit;
			break;
		case Vf::PathAnimActions::GoToNextMicroUnit:
			t += microunit;
			break;
		case Vf::PathAnimActions::GoToNextFrame:
			t = (t < keyt) ? keyt : keyt + invfps;
			break;
		case Vf::PathAnimActions::GoToNextKeyFrame:
			t = (t < sect) ? sect : sect + 1.0f;
			break;
		case Vf::PathAnimActions::GoToEnd:
			t = (float)animpath_window_ptr->max_keys;
			break;
		case Vf::PathAnimActions::FragileOperation:
			// Do nothing, the actions afterwards need to quit this update function
			break;
		case Vf::PathAnimActions::SetKey:			
			if (i >= 0 && i < cameraAnimation.keyframes.size()) {
				cameraAnimation.keyframes[i].setPosition(animpath_window_ptr->p1);
				cameraAnimation.keyframes[i].setQuaternion(animpath_window_ptr->q1);
			}
			break;
		case Vf::PathAnimActions::SetAlpha:
			cameraAnimation.set_alpha(animpath_window_ptr->alpha);
			break;
		case Vf::PathAnimActions::NewPathAnim:
			cameraAnimation.create();
			break;
		case Vf::PathAnimActions::ClearAnimation:
			cameraAnimation.clear();
			t = 0.0f;
			break;
		case Vf::PathAnimActions::LoadAnimation:
			PathAnim_LoadCameraPath(default_animation_path);
			t = 0.0f;
			break;
		case Vf::PathAnimActions::SaveAnimation:
			PathAnim_SaveCameraPath(default_animation_path);
			break;
		}
		if (animpath_window_ptr->action >= Vf::PathAnimActions::FragileOperation) {
			animpath_window_ptr->action = Vf::PathAnimActions::None;
			return;
		}
		animpath_window_ptr->action = Vf::PathAnimActions::None;
		t = Fluxions::wrap<float>(t, 0.0f, (float)animpath_window_ptr->max_keys);
		cameraAnimationTime = t;
		
		animpath_window_ptr->t = cameraAnimationTime;
		animpath_window_ptr->speed = cameraAnimationSpeed;
		animpath_window_ptr->max_keys = (int)cameraAnimation.size();
		animpath_window_ptr->key = cameraAnimationKeyframe;

		if (i >= 0 && i < cameraAnimation.keyframes.size()) {
			animpath_window_ptr->p1 = cameraAnimation.keyframes[i].p();
			animpath_window_ptr->q1 = cameraAnimation.keyframes[i].q();
		}

		Vector3f p = animpath_window_ptr->blerp ?
			cameraAnimation.plerp(t) :
			cameraAnimation.pcatmullrom(t);
		Quaternionf q = animpath_window_ptr->bsquad ?
			cameraAnimation.qsquad(t) :
			cameraAnimation.qslerp(t);

		animpath_window_ptr->q2 = q;
		animpath_window_ptr->p2 = p;
		Matrix4f m;
		m.Translate(p.x, p.y, p.z);
		m.MultMatrix((animpath_window_ptr->q * q).toMatrix4());
		ssg.camera.viewMatrix(m.AsInverse());

		cameraAnimation.calcgraph(animpath_window_ptr);
	}
}
