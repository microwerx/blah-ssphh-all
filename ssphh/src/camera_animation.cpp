#include <camera_animation.hpp>

void CameraAnimation::create() {
	using Fluxions::randomSampler;
	constexpr float size = 10.0f;
	constexpr float ysize = 5.0f;
	for (int i = 0; i < numControlPoints; i++) {
		float x = (float)(i % w) * size;
		float y = (float)(i / w) * size;
		float s = 0.25f;
		Vector3f p(1 + x + 0.5f, 3.0f, 1 + y + 0.5f);
		controlPoints.push_back(p + Vector3f(randomSampler(-s, s) * size,
											 randomSampler(-s, s) * ysize,
											 randomSampler(-s, s) * size));
		controlAlpha.push_back(1.0f);
		controlQuaternions.push_back(Quaternionf::makeFromAngles(randomSampler(-360, 360.0),
																 randomSampler(-45.0, 45.0),
																 randomSampler(-30.0, 30.0)));
	}
}

void CameraAnimation::calcgraph(Vf::MathWindowPtr& mw) {
	constexpr int nump = 32;
	constexpr int numq = 8;
	constexpr float scale = 20.0f;
	mw->circles.resize(numControlPoints);
	mw->points.resize(numControlPoints * nump);
	mw->lines.resize(numControlPoints * numq);

	bool lerp = mw->blerp;
	bool squad = mw->bsquad;

	for (int j = 0; j < controlPoints.size(); j++) {
		for (int i = 0; i < nump; i++) {
			int k = j * nump + i;
			float t = j + float(i) / nump;
			Vector3f p = lerp ? plerp(t) : pcatmullrom(t);

			mw->points[k].x = p.x * scale;
			mw->points[k].y = p.z * scale;

			if (i == 0) {
				mw->circles[j].x = p.x * scale;
				mw->circles[j].y = p.z * scale;
			}
		}
		for (int i = 0; i < numq; i++) {
			int k = j * numq + i;
			float t = j + float(i) / numq;
			Vector3f p = lerp ? plerp(t) : pcatmullrom(t);
			Quaternionf q = squad ? qsquad(t) : qslerp(t);
			Vector3f Z(q.m13(), q.m23(), q.m33());
			Z.normalize();
			if (i == 0) Z *= 2.5f;
			else Z *= 1.5;
			mw->lines[k].first.x = p.x * scale;
			mw->lines[k].first.y = p.z * scale;
			mw->lines[k].second.x = (p.x + Z.x) * scale;
			mw->lines[k].second.y = (p.z + Z.z) * scale;
		}
	}
}

Fluxions::Vector3f CameraAnimation::pcatmullrom(float t) const {
	return CatmullRomSplinePoint(t, controlPoints, controlAlpha);
}

Fluxions::Vector3f CameraAnimation::plerp(float t) const {
	int i1 = int(t) % numControlPoints;
	int i2 = (i1 + 1) % numControlPoints;
	t = t - int(t);
	return (1 - t) * controlPoints[i1] + t * controlPoints[i2];
}

Fluxions::Quaternionf CameraAnimation::qslerp(float t) const {
	int q1 = int(t) % numControlPoints;
	int q2 = (q1 + 1) % numControlPoints;
	t = t - int(t);
	return Fluxions::slerp(controlQuaternions[q1], controlQuaternions[q2], t).normalized();
}

Fluxions::Quaternionf CameraAnimation::qsquad(float t) const {
	using Fluxions::slerp;
	int iq1 = int(t) % numControlPoints;
	int iq0 = (iq1 - 1) % numControlPoints;
	int iq2 = (iq1 + 1) % numControlPoints;
	int iq3 = (iq1 + 2) % numControlPoints;
	t = t - int(t);
	Fluxions::Quaternionf q0 = controlQuaternions[iq0];
	Fluxions::Quaternionf q1 = controlQuaternions[iq1];
	Fluxions::Quaternionf q2 = controlQuaternions[iq2];
	Fluxions::Quaternionf q3 = controlQuaternions[iq3];
	return Fluxions::squad(q0, q1, q2, q3, t).normalized();
}
