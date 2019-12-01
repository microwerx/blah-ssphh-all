// ssphh-tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <type_traits>
#include <iterator>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <fluxions_gte.hpp>
#include <fluxions_gte_catmull_rom.hpp>

using namespace std;

#pragma comment(lib, "fluxions.lib")

void printControlPoints(ostream& fout,
						const std::vector<Fluxions::Vector3f>& points,
						const std::string& style) {
	for (auto& p : points) {
		fout << "<circle ";
		fout << "cx=\"" << (p.x * 100) << "\" ";
		fout << "cy=\"" << (p.y * 100) << "\" ";
		fout << "r=\"5\" ";
		fout << "style=\"" << style << "\" />\n";
	}
}

void printControlPoints(ostream& fout,
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

void printPolyline(ostream& fout,
				   const std::vector<Fluxions::Vector3f>& curve,
				   const std::string& style) {
	fout << "<polyline points=\"";
	for (auto& p : curve) {
		fout << (p.x * 100) << "," << (p.y * 100) << "\n";
	}
	fout << "\" style=\"" << style << "\" />\n";
}

void printLine(ostream& fout,
			   float x1, float y1, float x2, float y2,
			   const std::string& style) {
	fout << "<line ";
	fout << "x1=\"" << x1 * 100 << "\" ";
	fout << "y1=\"" << y1 * 100 << "\" ";
	fout << "x2=\"" << x2 * 100 << "\" ";
	fout << "y2=\"" << y2 * 100 << "\" ";
	fout << "style=\"" << style << "\" />\n";
}

void printQuaternion(ostream& fout,
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

void printQuaternions(ostream& fout,
					  const std::vector<Fluxions::Vector3f>& controlPoints,
					  const std::vector<Fluxions::Quaternionf>& controlQuaternions,
					  const std::string& style) {
	//const std::string style{ "stroke: black; stroke-width: 1; marker-end: url(#arrow);" };
	if (controlPoints.size() != controlQuaternions.size()) return;
	for (unsigned i = 0; i < controlPoints.size(); i++) {
		printQuaternion(fout, controlPoints[i], controlQuaternions[i], style);
	}
}

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

	void create() {
		using Fluxions::randomSampler;
		for (int i = 0; i < numControlPoints; i++) {
			float x = (float)(i % w);
			float y = (float)(i / w);
			float s = 0.25f;
			Vector3f p(1 + x + 0.5f, 1 + y + 0.5f, 0.0f);
			controlPoints.push_back(p + Vector3f(randomSampler(-s, s),
												 randomSampler(-s, s),
												 randomSampler(-s, s)));
			controlAlpha.push_back(1.0f);
			controlQuaternions.push_back(Quaternionf::makeFromAngleAxis(randomSampler(0.0, 360.0), 0, 0, 1));
		}
	}

	Vector3f p(float t) const {
		return CatmullRomSplinePoint(t, controlPoints, controlAlpha);
	}

	Quaternionf q(float t) const {
		int q1 = int(t) % numControlPoints;
		int q2 = (q1 + 1) % numControlPoints;
		t = t - int(t);
		return Fluxions::slerp(t, controlQuaternions[q1], controlQuaternions[q2]);
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
		controlPoints.push_back(p + Vector3f(randomSampler(-s, s),
											 randomSampler(-s, s),
											 randomSampler(-s, s)));
		//controlPoints.push_back(Vector3f(randomSampler(0, w),
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
		int q2 = (q1 + 1) % ccount;
		float t = curveTime[j] - int(curveTime[j]);
		curveQuaternions[j] = Fluxions::slerp(t, controlQuaternions[q1], controlQuaternions[q2]);
	}

	std::vector<Vector3f> curve;
	ofstream fout("curve.svg");
	fout << "<svg xmlns=\"http://www.w3.org/2000/svg\" ";
	fout << "width=\"" << (w + 2) * 100 << "\" ";
	fout << "height=\"" << (h + 2) * 100 << "\">" << endl;

	fout << "<defs>\n";
	fout << "<marker id=\"arrow\" markerWidth=\"10\" markerHeight=\"10\" refX=\"0\" refY=\"3\" orient=\"auto\" markerUnits=\"strokeWidth\">\n";
	fout << "<path d=\"M0,0 L0,6 L9,3 z\" fill=\"#f00\" />\n";
	fout << "</marker>\n";
	fout << "</defs>\n";

	printControlPoints(fout, controlPoints, controlAlpha,
					   "fill:red; stroke: black; stroke-width: 2;");

	CatmullRomClosedSpline(controlPoints, curve, numPoints, 1.0f);
	printPolyline(fout, curve, "fill: none; stroke: green; stroke-width: 1;");

	CatmullRomClosedSpline(controlPoints, curve, numPoints, -1.0f);
	printPolyline(fout, curve, "fill: none; stroke: red; stroke-width: 1;");

	CatmullRomSplineUniform(controlPoints, controlAlpha, curveTime, curve);
	printControlPoints(fout, curve,
					   "fill:green; stroke:black; stroke-width: 2;");
	printPolyline(fout, curve, "fill: none; stroke: blue; stroke-width: 1;");

	printQuaternions(fout, controlPoints, controlQuaternions, "stroke: blue; stroke-width: 2; marker-end: url(#arrow);");
	printQuaternions(fout, curve, curveQuaternions, "stroke: black; stroke-width: 1; marker-end: url(#arrow);");

	fout << "</svg>" << endl;
	fout.close();
}

int main() {
	auto float_category = std::iterator_traits<float*>::iterator_category();

	//cout << _Is_random_iter_v<Fluxions::Vector3f>;
	cout << "TCommonIterator<float>"
		<< "----------------------" << endl;
	cout << "input_iterator_tag:         " << (bool)std::is_convertible_v<Fluxions::TCommonIterator<float>::iterator_category, input_iterator_tag> << endl;
	cout << "output_iterator_tag:        " << (bool)std::is_convertible_v<Fluxions::TCommonIterator<float>::iterator_category, output_iterator_tag> << endl;
	cout << "forward_iterator_tag:       " << (bool)std::is_convertible_v<Fluxions::TCommonIterator<float>::iterator_category, forward_iterator_tag> << endl;
	cout << "bidirectional_iterator_tag: " << (bool)std::is_convertible_v<Fluxions::TCommonIterator<float>::iterator_category, bidirectional_iterator_tag> << endl;
	cout << "random_access_iterator_tag: " << (bool)std::is_convertible_v<Fluxions::TCommonIterator<float>::iterator_category, random_access_iterator_tag> << endl;
	//cout << "contiguous_iterator_tag:    " << (bool)std::is_convertible_v<Fluxions::TCommonIterator<float>, contiguous_iterator_tag> << endl;
	cout << "input_iterator_tag:         " << (bool)std::is_convertible_v<std::iterator_traits<float*>::iterator_category, input_iterator_tag> << endl;
	cout << "output_iterator_tag:        " << (bool)std::is_convertible_v<std::iterator_traits<float*>::iterator_category, output_iterator_tag> << endl;
	cout << "forward_iterator_tag:       " << (bool)std::is_convertible_v<std::iterator_traits<float*>::iterator_category, forward_iterator_tag> << endl;
	cout << "bidirectional_iterator_tag: " << (bool)std::is_convertible_v<std::iterator_traits<float*>::iterator_category, bidirectional_iterator_tag> << endl;
	cout << "random_access_iterator_tag: " << (bool)std::is_convertible_v<std::iterator_traits<float*>::iterator_category, random_access_iterator_tag> << endl;
	//cout << "contiguous_iterator_tag:    " << (bool)std::is_convertible_v<Fluxions::TCommonIterator<float>, contiguous_iterator_tag> << endl;
	cout << "---------------------------------------" << endl;
	Fluxions::TestFluxionsGTE();
	TestCatmullRom();
	return 0;
}
