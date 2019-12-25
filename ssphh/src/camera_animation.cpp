#include <camera_animation.hpp>

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
