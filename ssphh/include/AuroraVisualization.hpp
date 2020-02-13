#ifndef AURORA_VISUALIZATION_HPP
#define AURORA_VISUALIZATION_HPP

#include <viperfish.hpp>

namespace SSPHH {
	class AuroraViz : public Vf::Widget {
	public:
		AuroraViz() {}

		void OnInit() override { load(); }
		void OnUpdate(double timeStamp) override;

		void RenderFrame(double frameno);
	protected:
		static constexpr int NUM_SPLINES = 6;
		static constexpr int NUM_POINTS = 103;
		static constexpr int FRAME_START = 21700;
		static constexpr int FRAME_FINAL = 22500;
		static constexpr int FRAME_COUNT = FRAME_FINAL - FRAME_START + 1;

		std::vector<Fluxions::Vector3f> splines[NUM_SPLINES];
		std::vector<Fluxions::Vector3f> uvs[NUM_SPLINES];

		std::vector<int> visibleFrames[NUM_SPLINES];
		Fluxions::SimpleGeometryMesh mesh_frames[NUM_SPLINES];

		void loadSpline(int which, const char* splinepath);
		void loadUV(int which, const char* uvpath);
		void loadBrightness(int which, const char* brightnesspath);
		void load();
	};
}

#endif
