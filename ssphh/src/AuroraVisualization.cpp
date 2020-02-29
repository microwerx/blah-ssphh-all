#include "pch.hpp"
#include <AuroraVisualization.hpp>

namespace SSPHH {
	using Fluxions::Vector3f;

	void AuroraViz::OnUpdate(double timeStamp) {}


	void AuroraViz::RenderFrame(double frameno) {
		return;
		frameno *= 8.0f;
		int frame1 = int(frameno) % FRAME_COUNT;
		int frame2 = (frame1 + 1) % FRAME_COUNT;
		float t = (float)(frameno - int(frameno));

		Matrix4f M;
		M.Translate(0.0f, 3.0f, 0.0f);
		M.Scale(0.001f, 0.001f, 0.001f);
		for (int i = 0; i < NUM_SPLINES; i++) {
			if (!visibleFrames[i][frame1] && !visibleFrames[i][frame2]) continue;
			auto start1 = mesh_frames[i].Surfaces[frame1].first;
			auto start2 = mesh_frames[i].Surfaces[frame2].first;
			if (mesh_frames[i].Surfaces[frame1].count < NUM_POINTS) continue;
			if (mesh_frames[i].Surfaces[frame2].count < NUM_POINTS) continue;
			glBegin(GL_LINE_LOOP);
			for (int pt = 0; pt < NUM_POINTS; pt++) {
				auto& vtx1 = mesh_frames[i].Vertices[(int64_t)start1 + pt];
				auto& vtx2 = mesh_frames[i].Vertices[start2 + pt];
				Color4f c = Fluxions::lerp(t, vtx1.color, vtx2.color);
				Vector3f pos = Fluxions::lerp(t, vtx1.position, vtx2.position);
				pos *= 1.0f / 6471.0f;
				glColor3f(0.0f, c.r, 0.0f);
				glVertex3f(pos.x, pos.z, 0.0f);
			}
			glEnd();
			glBegin(GL_LINES);
			for (int pt = 0; pt < NUM_POINTS; pt++) {
				auto& vtx1 = mesh_frames[i].Vertices[start1 + pt];
				auto& vtx2 = mesh_frames[i].Vertices[start2 + pt];
				Color4f c = Fluxions::lerp(t, vtx1.color, vtx2.color);
				Vector3f pos1 = Fluxions::lerp(t, vtx1.position, vtx2.position);
				pos1 *= 1.0f / 6471.0f;
				glColor3f(0.0f, c.r, 0.0f);
				glVertex3f(pos1.x, pos1.z, 0.0f);
				glColor3f(0.0f, 0.0f, 0.0f);
				glVertex3f(pos1.x, pos1.z + c.r * 0.1f, 0.0f);
			}
			glEnd();
		}
	}


	void AuroraViz::loadSpline(int which, const char* path) {
		Hf::StopWatch stopwatch;
		Fluxions::string_vector lines;
		visibleFrames[which].resize(FRAME_COUNT, 0);
		ReadLines(path, lines, false);
		bool visible = false;
		int frameno{ -1 };
		int framesRead{ 0 };
		int frameIndex{ -1 };
		for (auto& line : lines) {
			std::istringstream istr(line);
			std::string token;

			if (frameno < 0) {
				istr >> token;
				if (token == "invisible") {
					visible = false;
					frameno = -1;
					continue;
				}

				if (token == "visible") {
					visible = true;
					frameno = -1;
					framesRead = 0;
					continue;
				}

				if (token == "frame") {
					istr >> token;
					istr >> frameno;
					frameIndex = frameno - FRAME_START;
					visibleFrames[which][frameIndex] = visible ? 1 : 0;
					mesh_frames[which].beginSurface(Fluxions::SimpleGeometryMesh::LineLoop);
					if (!visible) frameno = -1;
					continue;
				}

				if (token == "no") {
					continue;
				}
			}
			else if (framesRead == NUM_POINTS) {
				frameno = -1;
			}
			else {
				Vector3f p = Fluxions::ReadVector3f(istr);
				mesh_frames[which].texcoord2f(uvs[which][framesRead].x, uvs[which][framesRead].y);
				mesh_frames[which].color3f(uvs[which][framesRead].z, uvs[which][framesRead].z, uvs[which][framesRead].z);
				mesh_frames[which].position3f(p, true);
				framesRead++;
			}
		}
		HFLOGINFO("Splines '%s' read in %3.2fms", path, stopwatch.Stop_msf());
	}


	//void AuroraViz::loadSpline(int which, const char* path) {
	//	std::ifstream fin(path);
	//	for (int i = 0; i < NUM_POINTS; i++) {
	//		fin >> splines[which][i].x;
	//		fin >> splines[which][i].y;
	//		fin >> splines[which][i].z;
	//	}
	//}


	void AuroraViz::loadUV(int which, const char* path) {
		std::ifstream fin(path);
		for (int i = 0; i < NUM_POINTS; i++) {
			fin >> uvs[which][i].x;
			fin >> uvs[which][i].y;
		}
	}


	void AuroraViz::loadBrightness(int which, const char* path) {
		std::ifstream fin(path);
		for (int i = 0; i < NUM_POINTS; i++) {
			int index;
			fin >> index;
			fin >> uvs[which][i].z;
		}
	}


	void AuroraViz::load() {
		return;
		//static const char* splinefiles[NUM_SPLINES]{
		//	"resources/aurora/22100_1.data",
		//	"resources/aurora/22100_2.data",
		//	"resources/aurora/22100_2-1.data",
		//	"resources/aurora/22100_3.data",
		//	"resources/aurora/22100_3-1.data",
		//	"resources/aurora/22100_4.data" };
		static const char* splinefiles[NUM_SPLINES]{
			"resources/aurora/curtainMain.data",
			"resources/aurora/secondaryCurtain.data",
			"resources/aurora/secondaryCurtain-1.data",
			"resources/aurora/tertiaryCurtain.data",
			"resources/aurora/tertiaryCurtain-1.data",
			"resources/aurora/clouds.data" };
		static const char* uvfiles[NUM_SPLINES] =
		{ "resources/aurora/curtainMain.uv",
		  "resources/aurora/secondaryCurtain.uv",
		  "resources/aurora/secondaryCurtain-1.uv",
		  "resources/aurora/tertiaryCurtain.uv",
		  "resources/aurora/tertiaryCurtain-1.uv",
		  "resources/aurora/clouds.uv" };
		static const char* brightfiles[NUM_SPLINES] =
		{ "resources/aurora/curtainMain.brightness",
		  "resources/aurora/secondaryCurtain.brightness",
		  "resources/aurora/secondaryCurtain.brightness",
		  "resources/aurora/tertiaryCurtain.brightness",
		  "resources/aurora/tertiaryCurtain.brightness",
		  "resources/aurora/clouds.brightness" };

		for (int i = 0; i < NUM_SPLINES; i++) {
			splines[i].resize(NUM_POINTS);
			uvs[i].resize(NUM_POINTS);
			loadUV(i, uvfiles[i]);
			loadBrightness(i, brightfiles[i]);
			loadSpline(i, splinefiles[i]);
		}
		HFLOGINFO("Aurora Data Loaded");
	}
}
