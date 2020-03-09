// ssphh-tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#if __cplusplus < 201703
#error "C++17 required"
#endif
#define WIN32_LEAN_AND_MEAN

#include "test-resample.hpp"
// STL
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>
// Non STL
#include <fluxions_gte.hpp>
#include <fluxions_gte_catmull_rom.hpp>
#include <fluxions_ssg_aniso_light.hpp>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

using namespace Fluxions;
namespace fs = std::filesystem;
using namespace std;

#pragma comment(lib, "fluxions.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "xinput.lib")

void printControlPoints(ostream& fout, const std::vector<Fluxions::Vector3f>& points, const std::string& style) {
	for (auto& p : points) {
		fout << "<circle ";
		fout << "cx=\"" << (p.x * 100) << "\" ";
		fout << "cy=\"" << (p.y * 100) << "\" ";
		fout << "r=\"5\" ";
		fout << "style=\"" << style << "\" />\n";
	}
}

void printControlPoints(
	ostream& fout,
	const std::vector<Fluxions::Vector3f>& points,
	const std::vector<float>& alpha,
	const std::string& style) {
	for (unsigned i = 0; i < points.size(); i++) {

		fout << "<circle ";
		fout << "cx=\"" << (points[i].x * 100) << "\" ";
		fout << "cy=\"" << (points[i].y * 100) << "\" ";
		fout << "r=\"" << (alpha[i] + 2) << "\" ";
		fout << "style=\"" << style << "\" />\n";
	}
}

void printPolyline(ostream& fout, const std::vector<Fluxions::Vector3f>& curve, const std::string& style) {
	fout << "<polyline points=\"";
	for (auto& p : curve) {
		fout << (p.x * 100) << "," << (p.y * 100) << "\n";
	}
	fout << "\" style=\"" << style << "\" />\n";
}

void printLine(ostream& fout, float x1, float y1, float x2, float y2, const std::string& style) {
	fout << "<line ";
	fout << "x1=\"" << x1 * 100 << "\" ";
	fout << "y1=\"" << y1 * 100 << "\" ";
	fout << "x2=\"" << x2 * 100 << "\" ";
	fout << "y2=\"" << y2 * 100 << "\" ";
	fout << "style=\"" << style << "\" />\n";
}

void printQuaternion(
	ostream& fout,
	const Fluxions::Vector3f& p,
	const Fluxions::Quaternionf& q,
	const std::string& style) {
	Fluxions::Matrix3f M = q.toMatrix3();
	float x1 = p.x;
	float y1 = p.y;
	float x2 = x1 + M.m11 * 0.25f;
	float y2 = y1 + M.m21 * 0.25f;
	printLine(fout, x1, y1, x2, y2, style);
}

void printQuaternions(
	ostream& fout,
	const std::vector<Fluxions::Vector3f>& controlPoints,
	const std::vector<Fluxions::Quaternionf>& controlQuaternions,
	const std::string& style) {
	// const std::string style{ "stroke: black; stroke-width: 1; marker-end: url(#arrow);" };
	if (controlPoints.size() != controlQuaternions.size())
		return;
	for (unsigned i = 0; i < controlPoints.size(); i++) {
		printQuaternion(fout, controlPoints[i], controlQuaternions[i], style);
	}
}

class CameraAnimation {
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
	std::vector<float> controlTime;
	std::vector<Quaternionf> curveQuaternions;

	void create() {
		using Fluxions::randomSampler;
		for (int i = 0; i < numControlPoints; i++) {
			float x = (float)(i % w);
			float y = (float)(i / w);
			float s = 0.25f;
			Vector3f p(1 + x + 0.5f, 1 + y + 0.5f, 0.0f);
			controlPoints.push_back(p + Vector3f(randomSampler(-s, s), randomSampler(-s, s), randomSampler(-s, s)));
			controlAlpha.push_back(1.0f);
			controlQuaternions.push_back(Quaternionf::makeFromAngleAxis(randomSampler(0.0, 360.0), 0, 0, 1));
		}
	}

	Vector3f pcatmullrom(float t) const { return CatmullRomSplinePoint(t, controlPoints, controlAlpha); }

	Quaternionf q(float t) const {
		int q1 = int(t) % numControlPoints;
		int q2 = (q1 + 1) % numControlPoints;
		t = t - int(t);
		return Fluxions::slerp(controlQuaternions[q1], controlQuaternions[q2], t);
	}

	Fluxions::Quaternionf qsquad(float t) const {
		int iq1 = int(t) % numControlPoints;
		int iq0 = (iq1 - 1) % numControlPoints;
		int iq2 = (iq1 + 1) % numControlPoints;
		int iq3 = (iq1 + 2) % numControlPoints;
		t = t - int(t);
		Fluxions::Quaternionf q0 = controlQuaternions[iq0];
		Fluxions::Quaternionf q1 = controlQuaternions[iq1];
		Fluxions::Quaternionf q2 = controlQuaternions[iq2];
		Fluxions::Quaternionf q3 = controlQuaternions[iq3];
		return Fluxions::squad(q0, q1, q2, q3, t);
	}
};

void TestCatmullRom() {
	constexpr unsigned w = 4;
	constexpr unsigned h = 4;
	constexpr unsigned numControlPoints = w * h;
	constexpr unsigned numPoints = 6;

	using namespace Fluxions;

	std::vector<Quaternionf> controlQuaternions;
	std::vector<Vector3f> controlPoints;
	std::vector<float> controlAlpha;
	std::vector<float> curveTime;
	std::vector<Quaternionf> curveQuaternions;

	for (int i = 0; i < numControlPoints; i++) {
		float x = (float)(i % w);
		float y = (float)(i / w);
		float s = 0.25f;
		Vector3f p(1 + x + 0.5f, 1 + y + 0.5f, 0.0f);
		controlPoints.push_back(p + Vector3f(randomSampler(-s, s), randomSampler(-s, s), randomSampler(-s, s)));
		// controlPoints.push_back(Vector3f(randomSampler(0, w),
		//								 randomSampler(0, h),
		//								 randomSampler(0, 1)));
		controlAlpha.push_back(1.0f);
		controlQuaternions.push_back(Quaternionf::makeFromAngleAxis(randomSampler(0.0, 360.0), 0, 0, 1));
	}
	int first = 0;
	int last = numControlPoints * numPoints;
	for (int j = first; j < last + 1; j++) {
		curveTime.push_back((float)j / (float)numPoints);
	}

	curveQuaternions.resize(curveTime.size());
	const unsigned ccount = (unsigned)controlQuaternions.size();
	const unsigned pcount = (unsigned)curveQuaternions.size();
	for (int j = 0; j < curveTime.size(); j++) {
		int q1 = int(curveTime[j % pcount]) % ccount;
		int q0 = (q1 - 1) % ccount;
		int q2 = (q1 + 1) % ccount;
		int q3 = (q1 + 2) % ccount;
		float t = curveTime[j] - int(curveTime[j]);
		// curveQuaternions[j] = Fluxions::slerp(controlQuaternions[q1], controlQuaternions[q2], t);
		curveQuaternions[j] = Fluxions::squad(
			controlQuaternions[q0],
			controlQuaternions[q1],
			controlQuaternions[q2],
			controlQuaternions[q3],
			t);
	}

	std::vector<Vector3f> curve;
	ofstream fout("curve.svg");
	fout << "<svg xmlns=\"http://www.w3.org/2000/svg\" ";
	fout << "width=\"" << (w + 2) * 100 << "\" ";
	fout << "height=\"" << (h + 2) * 100 << "\">" << endl;

	fout << "<defs>\n";
	fout << "<marker id=\"arrow\" markerWidth=\"10\" markerHeight=\"10\" refX=\"0\" refY=\"3\" orient=\"auto\" "
			"markerUnits=\"strokeWidth\">\n";
	fout << "<path d=\"M0,0 L0,6 L9,3 z\" fill=\"#f00\" />\n";
	fout << "</marker>\n";
	fout << "</defs>\n";

	printControlPoints(fout, controlPoints, controlAlpha, "fill:red; stroke: black; stroke-width: 2;");

	CatmullRomClosedSpline(controlPoints, curve, numPoints, 1.0f);
	printPolyline(fout, curve, "fill: none; stroke: green; stroke-width: 1;");

	CatmullRomClosedSpline(controlPoints, curve, numPoints, -1.0f);
	printPolyline(fout, curve, "fill: none; stroke: red; stroke-width: 1;");

	CatmullRomSplineUniform(controlPoints, controlAlpha, curveTime, curve);
	printControlPoints(fout, curve, "fill:green; stroke:black; stroke-width: 2;");
	printPolyline(fout, curve, "fill: none; stroke: blue; stroke-width: 1;");

	printQuaternions(
		fout,
		controlPoints,
		controlQuaternions,
		"stroke: blue; stroke-width: 2; marker-end: url(#arrow);");
	printQuaternions(fout, curve, curveQuaternions, "stroke: black; stroke-width: 1; marker-end: url(#arrow);");

	fout << "</svg>" << endl;
	fout.close();
}

using Fluxions::Quaternionf;
using std::endl;
using std::ostream;

ostream& operator<<(ostream& os, Quaternionf q) {
	os << q.a << ", " << q.b << ", " << q.c << ", " << q.d;
	return os;
}

namespace Fluxions {}

void TestQuaternions() {
	Quaternionf q(1, 2, 3, 4);
	cout << setprecision(6);
	cout << setfill(' ');
	cout << fixed << showpos;
	cout << "q            " << q << endl;
	cout << "q'           " << q.normalized() << endl;
	cout << "log(q)       " << q.log() << endl;
	cout << "exp(q)       " << q.exp() << endl;
	cout << "exp(log(q)): " << q.log().exp() << endl;
	cout << "q^-1         " << q.inverse() << endl;
	cout << "q*           " << q.conjugate() << endl;
	cout << "q^p          " << q.pow(-1.0f) << endl;

	Quaternionf q0 = Quaternionf::makeFromAngleAxis(15.0, 0, 0, 1);
	Quaternionf q1 = Quaternionf::makeFromAngleAxis(35.0, 1, 0, 0);
	Quaternionf q2 = Quaternionf::makeFromAngleAxis(75.0, 0, 1, 0);
	Quaternionf q3 = Quaternionf::makeFromAngleAxis(95.0, 0, 0, 1);
	Quaternionf a = Fluxions::squad_si(q0, q1, q2);
	Quaternionf b = Fluxions::squad_si(q1, q2, q3);
	cout << "q0           " << q0 << endl;
	cout << "q1           " << q1 << endl;
	cout << "a            " << a << endl;
	cout << "b            " << b << endl;
	cout << "q2           " << q2 << endl;
	cout << "q3           " << q3 << endl;

	// Quaternion kQ0inv = rkQ0.UnitInverse();
	// Quaternion kQ1inv = rkQ1.UnitInverse();
	// Quaternion rkP0 = kQ0inv * rkQ1;
	// Quaternion rkP1 = kQ1inv * rkQ2;
	// Quaternion kArg = 0.25 * (rkP0.Log() - rkP1.Log());
	// Quaternion kMinusArg = -kArg;

	// rkA = rkQ1 * kArg.Exp();
	// rkB = rkQ1 * kMinusArg.Exp();

	Quaternionf q0inv = q0.inverse();
	Quaternionf q1inv = q1.inverse();
	Quaternionf p0 = q0inv * q1;
	Quaternionf p1 = q1inv * q2;
	Quaternionf arg = (p0.log() - p1.log()).scale(0.25f);
	Quaternionf narg = -arg;
	a = q1 * arg.exp();
	b = q1 * narg.exp();
	a = Fluxions::squad_a(q0, q1, q2);
	b = Fluxions::squad_b(q0, q1, q2);
	cout << "a            " << a << endl;
	cout << "b            " << b << endl;

	float y = Fluxions::randomSampler(-180, 180);
	float p = Fluxions::randomSampler(-90, 90);
	float r = Fluxions::randomSampler(-180, 180);
	q0 = Quaternionf::makeFromAngles(y, p, r);
	cout << "y,p,r        " << y << ", " << p << ", " << r << endl;
	cout << "q            " << q0 << endl;
	y = q0.yawInDegrees();
	p = q0.pitchInDegrees();
	r = q0.rollInDegrees();
	q0 = Quaternionf::makeFromAngles(y, p, r);
	cout << "y,p,r        " << y << ", " << p << ", " << r << endl;
	y = q0.yawInDegrees();
	p = q0.pitchInDegrees();
	r = q0.rollInDegrees();
	q0 = Quaternionf::makeFromAngles(y, p, r);
	cout << "y,p,r        " << y << ", " << p << ", " << r << endl;
	y = q0.yawInDegrees();
	p = q0.pitchInDegrees();
	r = q0.rollInDegrees();
	q0 = Quaternionf::makeFromAngles(y, p, r);
	cout << "y,p,r        " << y << ", " << p << ", " << r << endl;
}

int oldmain(int argc, char** argv) {
	TestResample();
	return 0;

	auto float_category = std::iterator_traits<float*>::iterator_category();

	// cout << _Is_random_iter_v<Fluxions::Vector3f>;
	cout << "TCommonIterator<float>"
		 << "----------------------" << endl;
	cout << "input_iterator_tag:         "
		 << (bool)
				std::is_convertible_v<Fluxions::TCommonIterator<float>::iterator_category, input_iterator_tag> << endl;
	cout << "output_iterator_tag:        "
		 << (bool)
				std::is_convertible_v<Fluxions::TCommonIterator<float>::iterator_category, output_iterator_tag> << endl;
	cout
		<< "forward_iterator_tag:       "
		<< (bool)
			   std::is_convertible_v<Fluxions::TCommonIterator<float>::iterator_category, forward_iterator_tag> << endl;
	cout << "bidirectional_iterator_tag: "
		 << (bool)std::is_convertible_v<
				Fluxions::TCommonIterator<float>::iterator_category,
				bidirectional_iterator_tag> << endl;
	cout << "random_access_iterator_tag: "
		 << (bool)std::is_convertible_v<
				Fluxions::TCommonIterator<float>::iterator_category,
				random_access_iterator_tag> << endl;
	// cout << "contiguous_iterator_tag:    " << (bool)std::is_convertible_v<Fluxions::TCommonIterator<float>,
	// contiguous_iterator_tag> << endl;
	cout << "input_iterator_tag:         "
		 << (bool)std::is_convertible_v<std::iterator_traits<float*>::iterator_category, input_iterator_tag> << endl;
	cout << "output_iterator_tag:        "
		 << (bool)std::is_convertible_v<std::iterator_traits<float*>::iterator_category, output_iterator_tag> << endl;
	cout << "forward_iterator_tag:       "
		 << (bool)std::is_convertible_v<std::iterator_traits<float*>::iterator_category, forward_iterator_tag> << endl;
	cout << "bidirectional_iterator_tag: "
		 << (bool)std::
				is_convertible_v<std::iterator_traits<float*>::iterator_category, bidirectional_iterator_tag> << endl;
	cout << "random_access_iterator_tag: "
		 << (bool)std::
				is_convertible_v<std::iterator_traits<float*>::iterator_category, random_access_iterator_tag> << endl;
	// cout << "contiguous_iterator_tag:    " << (bool)std::is_convertible_v<Fluxions::TCommonIterator<float>,
	// contiguous_iterator_tag> << endl;
	cout << "---------------------------------------" << endl;
	Fluxions::TestFluxionsGTE();
	TestCatmullRom();
	return 0;
}


TEST_CASE("Fluxions GTE", "[gte]") {
	Vector3f v{ 0.0f, 0.0f, 0.0f };
	v.make_finite();
	REQUIRE(v == Vector3f{ 0.0f, 0.0f, 0.0f });
	v.reset(
		std::numeric_limits<float>::quiet_NaN(),
		std::numeric_limits<float>::quiet_NaN(),
		std::numeric_limits<float>::quiet_NaN());
	v.make_finite();
	REQUIRE(v == Vector3f{ 0.0f, 0.0f, 0.0f });
}


TEST_CASE("Fluxions SSPHH Algorithm", "[ssphh]") {
	SimpleAnisoLight al1;
	al1.SH.resize(10, 0.0f);
	REQUIRE(al1.SH.degrees() == 10);
	for (unsigned i = 0; i < al1.SH.channels(); i++) {
		for (auto& a_lm : al1.SH[i]) {
			REQUIRE(a_lm == 0);
		}
	}


	al1.SH.randomize(-1.0f, 1.0f);
	for (int l = 0; l <= 10; l++) {
		for (int m = -l; m <= l; m++) {
			REQUIRE(al1.SH.Y(l, m) != 0);
		}
	}

	const std::string filename{ "sh.json" };
	if (fs::exists(filename))
		fs::remove(filename);
	REQUIRE(al1.SH.saveJSON(filename) == true);
	REQUIRE(fs::exists(filename) == true);

	SimpleAnisoLight al2;
	REQUIRE(al2.SH.loadJSON(filename) == true);
}
