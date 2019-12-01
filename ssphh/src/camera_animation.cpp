#include <camera_animation.hpp>

void CameraAnimation::create() {
	using Fluxions::randomSampler;
	for (int i = 0; i < numControlPoints; i++) {
		float x = (float)(i % w);
		float y = (float)(i / w);
		float s = 0.25f;
		Vector3f p(1 + x + 0.5f, 3.0f, 1 + y + 0.5f);
		controlPoints.push_back(p + Vector3f(randomSampler(-s, s) * 4.0f,
											 randomSampler(-s, s),
											 randomSampler(-s, s) * 4.0f));
		controlAlpha.push_back(1.0f);
		controlQuaternions.push_back(Quaternionf::makeFromAngles(randomSampler(0, 360.0),
																 randomSampler(-45.0, 45.0),
																 randomSampler(-25.0, 25.0)));
	}
}

Fluxions::Vector3f CameraAnimation::p(float t) const {
	return CatmullRomSplinePoint(t, controlPoints, controlAlpha);
}

Fluxions::Quaternionf CameraAnimation::q(float t) const {
	int q1 = int(t) % numControlPoints;
	int q2 = (q1 + 1) % numControlPoints;
	t = t - int(t);
	return Fluxions::slerp(t, controlQuaternions[q1], controlQuaternions[q2]);
}
