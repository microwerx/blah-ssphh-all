#include "test-resample.hpp"
#include <fluxions_gte.hpp>
#include <fluxions_gte_image_operations.hpp>

namespace Fluxions {
	template <typename T>
	void ResampleRectToCubeMap(TImage<T>& src, TImage<T>& dst, int size, Matrix3f R) {
		dst.resize(size, size, 6);
	}
}

void testResample() {
	Fluxions::Image3f src;
	src.LoadPFM("../ssphh/resources/textures/mwpan2_RGB_3600.pfm");
	Fluxions::Image3f dst;
	Fluxions::ResampleRectToCubeMap(src, dst, 64, Fluxions::Matrix3f::MakeIdentity());
}
