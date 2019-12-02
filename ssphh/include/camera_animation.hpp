#ifndef CAMERA_ANIMATION_HPP
#define CAMERA_ANIMATION_HPP

#include <fluxions_gte_vector_math.hpp>
#include <fluxions_gte_catmull_rom.hpp>
#include <viperfish_math_window.hpp>

class CameraAnimation
{
public:
	static constexpr unsigned w = 4;
	static constexpr unsigned h = 4;
	static constexpr unsigned numControlPoints = w * h;
	static constexpr unsigned numPoints = 6;

	using Quaternionf = Fluxions::Quaternionf;
	using Vector3f = Fluxions::Vector3f;

	std::vector<Quaternionf> controlQuaternions;
	std::vector<Vector3f> controlPoints;
	std::vector<float> controlAlpha;
	std::vector<float> curveTime;
	std::vector<Quaternionf> curveQuaternions;

	void create();
	void calcgraph(Vf::MathWindowPtr& mw);
	Vector3f pcatmullrom(float t) const;
	Vector3f plerp(float t) const;
	Quaternionf qslerp(float t) const;
	Quaternionf qsquad(float t) const;
};


#endif
