#include <camera_animation.hpp>

void CameraAnimation::resize(unsigned count) {
	//controlQuaternions.resize(keyframes);
	//controlPoints.resize(keyframes);
	//controlAlpha.resize(keyframes);
	//controlTime.resize(keyframes);
	keyframes.resize(count);
}

void CameraAnimation::clear() {
	keyframes.clear();
	//controlQuaternions.clear();
	//controlPoints.clear();
	//controlAlpha.clear();
	//controlTime.clear();
}

void CameraAnimation::create() {
	clear();
	resize(w * h);

	using Fluxions::randomSampler;
	constexpr float xsize = 10.0f;
	constexpr float ysize = 5.0f;
	Vector3f lastp;
	for (int i = 0; i < size(); i++) {
		float x = (float)(i % w) * xsize;
		float y = (float)(i / w) * xsize;
		float s = 0.25f;
		Vector3f p(1 + x + 0.5f, 3.0f, 1 + y + 0.5f);

		Vector3f cp = p + Vector3f(randomSampler(-s, s) * xsize,
								   randomSampler(-s, s) * ysize,
								   randomSampler(-s, s) * xsize);
		float ca = 1.0f;
		Quaternionf cq = Quaternionf::makeFromAngles(randomSampler(-180, 180.0),
													 randomSampler(-30.0, 0.0),
													 randomSampler(-15.0, 15.0));
		float ct = 0.0f;
		if (i > 0) {
			ct = (lastp - p).length();
		}
		lastp = p;
		Keyframe kf{ ct, ca, cp, cq };
		keyframes[i] = kf;
	}
}

void CameraAnimation::calcgraph(Vf::AnimPathWindowPtr& mw) {
	constexpr int nump = 32;
	constexpr int numq = 8;
	constexpr float scale = 20.0f;
	mw->circles.resize(size());
	mw->points.resize(size() * nump);
	mw->lines.resize(size() * numq);

	bool lerp = mw->blerp;
	bool squad = mw->bsquad;

	for (int j = 0; j < keyframes.size(); j++) {
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
	const unsigned size = (unsigned)keyframes.size();
	int cp1 = int(t) % keyframes.size();
	int cp0 = (cp1 - 1) % keyframes.size();
	int cp2 = (cp1 + 1) % keyframes.size();
	int cp3 = (cp1 + 2) % keyframes.size();
	t = t - int(t);
	float alpha = Fluxions::lerp(t, keyframes[cp1].a, keyframes[cp2].a);
	return Fluxions::CatmullRomSegment(t,
									   keyframes[cp0].p,
									   keyframes[cp1].p,
									   keyframes[cp2].p,
									   keyframes[cp3].p,
									   alpha);
}

Fluxions::Vector3f CameraAnimation::plerp(float t) const {
	int cp1 = int(t) % keyframes.size();
	int cp2 = (cp1 + 1) % keyframes.size();
	t = t - int(t);
	return (1 - t) * keyframes[cp1].p + t * keyframes[cp2].p;
}

Fluxions::Quaternionf CameraAnimation::qslerp(float t) const {
	int cp1 = int(t) % keyframes.size();
	int cp2 = (cp1 + 1) % keyframes.size();
	t = t - int(t);
	return Fluxions::slerp(keyframes[cp1].q, keyframes[cp2].q, t).normalized();
}

Fluxions::Quaternionf CameraAnimation::qsquad(float t) const {
	using Fluxions::slerp;
	int cp1 = int(t) % keyframes.size();
	int cp0 = (cp1 - 1) % keyframes.size();
	int cp2 = (cp1 + 1) % keyframes.size();
	int cp3 = (cp1 + 2) % keyframes.size();
	t = t - int(t);
	Fluxions::Quaternionf q0 = keyframes[cp0].q;
	Fluxions::Quaternionf q1 = keyframes[cp1].q;
	Fluxions::Quaternionf q2 = keyframes[cp2].q;
	Fluxions::Quaternionf q3 = keyframes[cp3].q;
	return Fluxions::squad(q0, q1, q2, q3, t).normalized();
}
