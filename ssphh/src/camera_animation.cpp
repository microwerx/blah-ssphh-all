#include <camera_animation.hpp>

void CameraAnimation::calcgraph(Vf::AnimPathWindowPtr& mw) {
	constexpr int nump = 32;
	constexpr int numq = 8;

	mw->circles.resize(size());
	mw->points.resize(size() * nump);
	mw->lines.resize(size() * numq);

	Fluxions::BoundingBoxf bbox;
	for (auto& kf : keyframes) {
		bbox += kf.p();
	}
	const float scale = 200.0f / bbox.MaxSize();
	const float ox = bbox.MinX() - 0.1f * scale;
	const float oy = bbox.MinZ() - 0.1f * scale;

	bool lerp = mw->blerp;
	bool squad = mw->bsquad;

	mw->offsetX = ox;
	mw->offsetY = oy;
	mw->scale = scale;

	for (int j = 0; j < keyframes.size(); j++) {
		for (int i = 0; i < nump; i++) {
			int k = j * nump + i;
			float t = j + float(i) / nump;
			Vector3f p = lerp ? plerp(t) : pcatmullrom(t);

			mw->points[k].x = (p.x - ox) * scale;
			mw->points[k].y = (p.z - oy) * scale;

			if (i == 0) {
				mw->circles[j].x = (p.x - ox) * scale;
				mw->circles[j].y = (p.z - oy) * scale;
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
			mw->lines[k].first.x = (p.x - ox) * scale;
			mw->lines[k].first.y = (p.z - oy) * scale;
			mw->lines[k].second.x = (p.x - ox + Z.x) * scale;
			mw->lines[k].second.y = (p.z - oy + Z.z) * scale;
		}
	}
}
