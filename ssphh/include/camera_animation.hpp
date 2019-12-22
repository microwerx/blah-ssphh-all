#ifndef CAMERA_ANIMATION_HPP
#define CAMERA_ANIMATION_HPP

#include <fluxions_gte_vector_math.hpp>
#include <fluxions_gte_catmull_rom.hpp>
#include <viperfish_animpath_window.hpp>

struct Keyframe {
	// time of keyframe
	float t = 0.0f;
	// alpha of keyframe for motion
	float a = 1.0f;
	// position of keyframe
	Fluxions::Vector3f p;
	// orientation of keyframe
	Fluxions::Quaternionf q;

	bool operator<(const Keyframe& b) {
		return t < b.t;
	}
};

class CameraAnimation
{
public:
	static constexpr unsigned w = 4;
	static constexpr unsigned h = 4;
	//static constexpr unsigned numControlPoints = w * h;
	static constexpr unsigned numPoints = 6;

	using Quaternionf = Fluxions::Quaternionf;
	using Vector3f = Fluxions::Vector3f;

	std::vector<Keyframe> keyframes;

	//std::vector<Quaternionf> controlQuaternions;
	//std::vector<Vector3f> controlPoints;
	//std::vector<float> controlAlpha;
	//std::vector<float> controlTime;
	//std::vector<Quaternionf> curveQuaternions;

	unsigned size() const { return (unsigned)keyframes.size(); }
	void resize(unsigned count);
	void clear();
	void create();
	void calcgraph(Vf::AnimPathWindowPtr& mw);
	Vector3f pcatmullrom(float t) const;
	Vector3f plerp(float t) const;
	Quaternionf qslerp(float t) const;
	Quaternionf qsquad(float t) const;

	//const Quaternionf& getq(int i) const {
	//	return controlQuaternions[i % controlQuaternions.size()];
	//}

	const Keyframe& operator[](int i) const {
		return keyframes[i % keyframes.size()];
	}
};


#endif
