#include "pch.hpp"
#include <ssphhapp.hpp>
#include <fluxions_ssphh_utilities.hpp>

namespace SSPHH
{
	//using TestProduct = std::tuple<bool, int, int, int, std::string>;

	struct CoronaTestProduct {
		int enableSpecularReflection;
		int maxRayDepth;
		int passLimit;
		int maxShDegree;
		std::string productName;

		std::string dtg;
		double secsToRender;
		double pathTracerTotalEnergy;
		double sphlRenderTotalEnergy;
		double diff1TotalEnergy;
		double diff2TotalEnergy;

		static std::ostream& writeHeader(std::ostream& os) {
			os << "name,dtg,specular,mrd,pl,md,time,ptE,sphlE,d1E,d2E" << "\n";
		}

		std::ostream& writeln(std::ostream& os) const {
			os << productName << "," << dtg << "," << enableSpecularReflection;
			os << ", " << maxRayDepth << "," << passLimit << "," << maxShDegree << "," << secsToRender << ",";
			os << pathTracerTotalEnergy << ",";
			os << sphlRenderTotalEnergy << ",";
			os << diff1TotalEnergy << ",";
			os << diff2TotalEnergy << "\n";
			return os;
		}

		int sortvalue() const {
			int result = (!enableSpecularReflection) ? 1 : 0;
			result |= maxRayDepth * 10;
			result |= passLimit * 1000;
			result |= maxShDegree * 10000;
			return result;
		}

		bool operator<(const CoronaTestProduct& other) const {
			if (productName < other.productName) return true;
			return (sortvalue() < other.sortvalue());
		}

		bool operator ==(const CoronaTestProduct& other) const {
			return (productName == other.productName && sortvalue() == other.sortvalue());
		}

		bool operator !=(const CoronaTestProduct& other) const {
			return (productName != other.productName || sortvalue() != other.sortvalue());
		}
	};

	void SSPHH_Application::Corona_GenerateSCN() {
		// Algorithm
		// generate export_corona_cubemap_sphl_XX.scn      (64x64) where XX is the index of the SPHL
		// generate export_corona_ground_truth.png (1280x720)
		// run corona to generate export_corona_cubemap.png      (64x64)
		// run corona to generate export_corona_ground_truth.png (1280x720)
		coronaScene.clearCache = true;
		for (size_t i = 0; i < 16; i++) {
			if (i >= sphls.size())
				break;

			std::ostringstream ostr;
			ostr << Uf::CoronaJob::exportPathPrefix << "export_corona_cubemap_sphl_"
				<< std::setw(2) << std::setfill('0') << (int)i << ".scn";
			coronaScene.writeCubeMapSCN(ostr.str(), ssg, sphls[i].position.xyz());
		}

		coronaScene.writeSCN(Uf::CoronaJob::exportPathPrefix +
							 "export_corona_ground_truth.scn", ssg);
		coronaScene.writeCubeMapSCN(Uf::CoronaJob::exportPathPrefix +
									"export_corona_ground_truth_cube.scn", ssg);
	}

	void SSPHH_Application::Corona_GenerateREF() {
		Corona_CheckCache();
		if (Interface.ssphh.enableREF) {
			double time0 = HFLOG_SECS_ELAPSED();
			std::string name = Fluxions::MakeREFName(
				Interface.sceneName,
				false,
				Interface.ssphh.enableHDR,
				Interface.ssphh.enableHQ,
				Interface.ssphh.enableKs,
				Interface.ssphh.REF_MaxRayDepth,
				Interface.ssphh.REF_PassLimit);
			Uf::CoronaJob job1(name, Uf::CoronaJob::Type::REF);
			if (Interface.ssphh.enableHQ)
				job1.EnableHQ();
			if (Interface.ssphh.enableHDR)
				job1.EnableHDR();
			job1.SetMaxRayDepth(Interface.ssphh.REF_MaxRayDepth);
			job1.SetPassLimit(Interface.ssphh.REF_PassLimit);
			job1.SetIgnoreCache(Interface.ssphh.REF_IgnoreCache);
			// send it out
			if (!ssphhUf.IsStopped())
				ssphhUf.ScatterJob(job1);
			else
				job1.Start(coronaScene, ssg);
			Interface.ssphh.lastREFPath = job1.GetOutputPath();
			Interface.ssphh.lastREFTime = HFLOG_SECS_ELAPSED() - time0;
		}

		if (Interface.ssphh.enableREFCubeMap) {
			double time0 = HFLOG_SECS_ELAPSED();
			std::string name = Fluxions::MakeREFName(
				Interface.sceneName,
				true,
				Interface.ssphh.enableHDR,
				Interface.ssphh.enableHQ,
				Interface.ssphh.enableKs,
				Interface.ssphh.REF_MaxRayDepth,
				Interface.ssphh.REF_PassLimit);
			Uf::CoronaJob job2(name, Uf::CoronaJob::Type::REF_CubeMap);
			if (Interface.ssphh.enableHQ)
				job2.EnableHQ();
			if (Interface.ssphh.enableHDR)
				job2.EnableHDR();
			job2.SetMaxRayDepth(Interface.ssphh.REF_MaxRayDepth);
			job2.SetPassLimit(Interface.ssphh.REF_PassLimit);
			job2.SetIgnoreCache(Interface.ssphh.REF_IgnoreCache);
			// send it out
			if (!ssphhUf.IsStopped())
				ssphhUf.ScatterJob(job2);
			else
				job2.Start(coronaScene, ssg);
			Interface.ssphh.lastREFCubeMapPath = job2.GetOutputPath();
			Interface.ssphh.lastREFCubeMapTime = HFLOG_SECS_ELAPSED() - time0;
		}
	}

	void SSPHH_Application::Corona_GenerateSphlVIZ() {
		Corona_CheckCache();

		// Algorithm
		// for every pair of lights i, j where i != j,
		//     generate a SCN that represents the

		double viz_t0 = HFLOG_SECS_ELAPSED();

		int count = 0;
		int numLights = (int)ssgUserData->ssphhLights.size();
		Interface.ssphh.viz_times.resize(numLights, 0.0);

		for (int sendLight = 0; sendLight < numLights; sendLight++) {
			if (rendererContext.rendererConfigs["default"].shaderDebugSphl >= 0 &&
				sendLight != rendererContext.rendererConfigs["default"].shaderDebugSphl)
				continue;

			auto& sphl = ssgUserData->ssphhLights[sendLight];
			sphl.vizgenLightProbes.resize(numLights);

			for (int recvLight = 0; recvLight < numLights; recvLight++) {
				if (recvLight == sendLight)
					continue;

				int mrd = Interface.ssphh.VIZ_MaxRayDepth;
				int pl = Interface.ssphh.VIZ_PassLimit;
				std::string viz_name;
				viz_name = Fluxions::MakeVIZName(Interface.sceneName,
												 recvLight,
												 sendLight,
												 false,
												 false,
												 Interface.ssphh.enableKs,
												 mrd,
												 pl);

				Uf::CoronaJob job(viz_name, Uf::CoronaJob::Type::VIZ, sendLight, recvLight);
				if (Interface.ssphh.enableHDR)
					job.EnableHDR();
				if (Interface.ssphh.enableHQ)
					job.EnableHQ();
				job.SetMaxRayDepth(mrd);
				job.SetPassLimit(mrd);
				job.SetIgnoreCache(Interface.ssphh.VIZ_IgnoreCache);
				job.SetImageDimensions(Interface.ssphh.LightProbeSize, Interface.ssphh.LightProbeSize);

				if (!ssphhUf.IsStopped()) {
					// send it out
					ssphhUf.ScatterJob(job);
				}
				else {
					job.Start(coronaScene, ssg);
					GI_ProcessJob(job);
					count++;
				}
			}
		}

		if (count)
			ssphh.VIZ();

		Interface.ssphh.lastVIZTime = HFLOG_SECS_ELAPSED() - viz_t0;
	}

	void SSPHH_Application::Corona_GenerateSphlINIT() {
		double timeElapsed = HFLOG_SECS_ELAPSED();
		ssphh.INIT(ssg);
		Interface.ssphh.lastINITTime = HFLOG_SECS_ELAPSED() - timeElapsed;
		DirtySPHLs();
	}

	void SSPHH_Application::Corona_GenerateSphlHIER() {
		double timeElapsed = HFLOG_SECS_ELAPSED();
		ssphh.VIZmix = Interface.ssphh.HierarchiesMix;
		ssphh.HIER(Interface.ssphh.HierarchiesIncludeSelf, Interface.ssphh.HierarchiesIncludeNeighbors, Interface.ssphh.MaxDegrees);
		Interface.ssphh.lastHIERTime = HFLOG_SECS_ELAPSED() - timeElapsed;
		DirtySPHLs();
	}

	void SSPHH_Application::Corona_GenerateSphlGEN() {
		Corona_CheckCache();

		// Algorithm
		// for every light i
		//     generate a SCN that represents the direct illumination from the location of the light
		//	   add contribution from neighboring lights
		//

		int count = 0;
		int numLights = (int)ssgUserData->ssphhLights.size();
		double gen_t0 = HFLOG_SECS_ELAPSED();
		Interface.ssphh.gen_times.resize(numLights, 0.0);
		for (int sendLight = 0; sendLight < numLights; sendLight++) {
			if (rendererContext.rendererConfigs["default"].shaderDebugSphl >= 0 &&
				sendLight != rendererContext.rendererConfigs["default"].shaderDebugSphl)
				continue;
			auto& sphl = ssgUserData->ssphhLights[sendLight];
			sphl.vizgenLightProbes.resize(numLights);

			//sphl.index = sendLight;
			int mrd = Interface.ssphh.GEN_MaxRayDepth;
			int pl = Interface.ssphh.GEN_PassLimit;
			Uf::CoronaJob job(Fluxions::MakeGENName(
				Interface.sceneName,
				sendLight,
				false,
				false,
				Interface.ssphh.enableKs,
				mrd,
				pl),
				Uf::CoronaJob::Type::GEN, sendLight);
			if (Interface.ssphh.enableHDR)
				job.EnableHDR();
			if (Interface.ssphh.enableHQ)
				job.EnableHQ();
			job.SetMaxRayDepth(mrd);
			job.SetPassLimit(pl);
			job.SetImageDimensions(Interface.ssphh.LightProbeSize, Interface.ssphh.LightProbeSize);
			job.SetIgnoreCache(Interface.ssphh.GEN_IgnoreCache);

			if (!ssphhUf.IsStopped()) {
				// send it out
				ssphhUf.ScatterJob(job);
			}
			else {
				job.Start(coronaScene, ssg);
				GI_ProcessJob(job);
				count++;
			}
		}

		if (count) {
			ssphh.GEN();
			DirtySPHLs();
		}

		Interface.ssphh.lastGENTime = HFLOG_SECS_ELAPSED() - gen_t0;
	}

	void SSPHH_Application::Corona_CheckCache() {
		if (coronaScene.enableKs != Interface.ssphh.enableKs) {
			coronaScene.enableKs = Interface.ssphh.enableKs;
			coronaScene.clearCache = true;
		}
	}

	void SSPHH_Application::Corona_DeleteCache() {
		std::vector<std::string> ext_to_delete{
			".scn",
			".exr",
			".ppm",
			"_hq.exr",
			"_hq.ppm",
			".json",
			"_tonemap.conf",
			"_01_Sprime.ppm",
			"_02_self.ppm"
			"_03_neighbor.ppm",
			"_01_Sprime.exr",
			"_02_self.exr"
			"_03_neighbor.exr",
			"_01_Sprime.json",
			"_02_self.json",
			"_03_neighbor.json"
		};

		for (int i = 0; i < MaxSphlLights; i++) {
			std::string base_filename = Fluxions::MakeGENName(Interface.sceneName, i);

			std::vector<std::string> files_to_delete{
				(base_filename + ".scn"),
				(base_filename + ".exr"),
				(base_filename + ".ppm"),
				(base_filename + "_hq.exr"),
				(base_filename + "_hq.ppm"),
				(base_filename + ".json"),
				(base_filename + "_tonemap.conf"),
				(base_filename + "_01_Sprime.ppm"),
				(base_filename + "_02_self.ppm"),
				(base_filename + "_03_neighbor.ppm"),
				(base_filename + "_01_Sprime.json"),
				(base_filename + "_02_self.json"),
				(base_filename + "_03_neighbor.json")
			};

			for (auto& ext : ext_to_delete) {
				std::string target = base_filename + ext;
				std::filesystem::remove(target);
			}

			// we count one more for the sun-to-sphl files
			for (int j = 0; j <= MaxSphlLights; j++) {
				std::string viz_base_filename = Fluxions::MakeVIZName(Interface.sceneName, i, j);

				for (auto& ext : ext_to_delete) {
					std::string target = viz_base_filename + ext;
					std::filesystem::remove(target);
				}
			}
		}

		const std::string& gt = Fluxions::MakeREFName(Interface.sceneName, false);
		const std::string& gt_cm = Fluxions::MakeREFName(Interface.sceneName, true);
		const std::string& gt_mrdpl = Fluxions::MakeREFName(Interface.sceneName, false, Interface.ssphh.enableHDR, false, Interface.ssphh.REF_MaxRayDepth, Interface.ssphh.REF_PassLimit);
		const std::string& gt_cm_mrdpl = Fluxions::MakeREFName(Interface.sceneName, true, Interface.ssphh.enableHDR, false, Interface.ssphh.REF_MaxRayDepth, Interface.ssphh.REF_PassLimit);

		std::vector<std::string> product_ext_to_delete{
			".scn",
			".exr",
			".ppm",
			".png",
			"_tonemap.conf",
			"CESSENTIAL_Direct.exr",
			"CESSENTIAL_Indirect.exr",
			"CESSENTIAL_Reflect.exr"
		};

		std::vector<std::string> products = {
			"ssphh_ground_truth",
			"ssphh_ground_truth_hq"
			"ssphh_ground_truth_cubemap",
			"ssphh_ground_truth_cubemap_hq",
			"ssphh_sky"
			"ssphh_sky_hq",
			"sky"
			"sky_hq",
			gt + "_hq",
			gt + "_hq_hdr",
			gt_cm + "_hq",
			gt_cm + "_hq_hdr",
			gt_mrdpl,
			gt_cm_mrdpl
		};

		for (auto& product : products) {
			for (auto& product_ext : product_ext_to_delete) {
				std::string target = product + product_ext;
				std::filesystem::remove(target);
			}
		}
	}

	void SSPHH_Application::Corona_GenerateTestProducts() {
		std::vector<int> maxRayDepths = { 25, 3, 1, 0 };
		std::vector<int> passes = { 1, 3, 25 };
		std::vector<int> degrees = { 9, 2 };

		//// Exhaustive statistics
		//vector<int> maxRayDepths = { 25, 10, 5, 4, 3, 2, 1, 0 };
		//vector<int> passes = { 1, 2, 3, 4, 5, 10, 25 };
		//vector<int> degrees = { 9, 2 };

		//vector<int> maxRayDepths = { 25, 10, 5, 4, 3, 2, 1, 0 };
		//vector<int> passes = { 50, 25, 10, 5, 4, 3, 2, 1 };
		//vector<int> degrees = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
		std::vector<int> specular = { 1 };
		std::map<std::string, CoronaTestProduct> coronaTestProductTimes;

		auto gmrd = Interface.ssphh.GEN_MaxRayDepth;
		auto vmrd = Interface.ssphh.VIZ_MaxRayDepth;
		auto rmrd = Interface.ssphh.REF_MaxRayDepth;
		auto gpl = Interface.ssphh.GEN_PassLimit;
		auto vpl = Interface.ssphh.VIZ_PassLimit;
		auto rpl = Interface.ssphh.REF_PassLimit;
		auto gic = Interface.ssphh.GEN_IgnoreCache;
		auto vic = Interface.ssphh.VIZ_IgnoreCache;
		auto ric = Interface.ssphh.REF_IgnoreCache;
		auto lastDegrees = Interface.ssphh.MaxDegrees;

		int totalProducts = (int)(specular.size() * maxRayDepths.size() * passes.size() * degrees.size());
		int count = 0;

		// Render Ground Truth First
		coronaScene.clearCache = true;
		coronaScene.enableKs = true;
		coronaScene.writeSCN(ssg.name_str(), ssg);
		for (auto& mrd : maxRayDepths) {
			for (auto& pl : passes) {
				Interface.ssphh.REF_MaxRayDepth = mrd;
				Interface.ssphh.REF_PassLimit = pl;
				Interface.ssphh.REF_IgnoreCache = Interface.ssphh.genProductsIgnoreCache;
				Interface.ssphh.LightProbeSize = 32;
				Interface.ssphh.enableShowSPHLs = false;
				Interface.ssphh.enableShowHierarchies = false;
				Interface.ssphh.enableKs = true;

				std::string ptname = GetPathTracerName(Interface.sceneName, "CRNA", true, mrd, pl);
				HFLOGINFO("Starting %s", ptname.c_str());

				//coronaScene.WriteMaterials(ssg, true);
				Corona_GenerateREF();
				coronaTestProductTimes[ptname] = { true, mrd, pl, 0, "REF" };
				coronaTestProductTimes[ptname].secsToRender = Interface.ssphh.lastREFTime;
			}
		}

		for (auto& ks : specular) {
			coronaScene.clearCache = true;
			coronaScene.enableKs = ks != 0;
			coronaScene.writeSCN(ssg.name_str(), ssg);
			for (auto& mrd : maxRayDepths) {
				for (auto& pl : passes) {
					Interface.ssphh.GEN_MaxRayDepth = mrd;
					Interface.ssphh.GEN_PassLimit = pl;
					Interface.ssphh.GEN_IgnoreCache = Interface.ssphh.genProductsIgnoreCache;
					Interface.ssphh.VIZ_MaxRayDepth = mrd;
					Interface.ssphh.VIZ_PassLimit = pl;
					Interface.ssphh.VIZ_IgnoreCache = Interface.ssphh.genProductsIgnoreCache;
					Interface.ssphh.REF_MaxRayDepth = mrd;
					Interface.ssphh.REF_PassLimit = pl;
					Interface.ssphh.REF_IgnoreCache = Interface.ssphh.genProductsIgnoreCache;
					Interface.ssphh.LightProbeSize = 32;
					Interface.ssphh.enableShowSPHLs = false;
					Interface.ssphh.enableShowHierarchies = false;
					Interface.ssphh.enableKs = ks;

					std::string ptname = GetPathTracerName(Interface.sceneName, "CRNA", ks, mrd, pl);
					std::string INITname = GetPathTracerName(Interface.sceneName, "INIT", ks, mrd, pl);
					std::string VIZname = GetPathTracerName(Interface.sceneName, "VIZ_", ks, mrd, pl);
					std::string GENname = GetPathTracerName(Interface.sceneName, "GEN_", ks, mrd, pl);
					std::string HIERname = GetPathTracerName(Interface.sceneName, "HIER", ks, mrd, pl);
					std::string SSHHname = GetPathTracerName(Interface.sceneName, "SSHH", ks, mrd, pl);
					HFLOGINFO("Starting %s", ptname.c_str());

					//coronaScene.WriteMaterials(ssg, true);
					//Corona_GenerateREF();
					//coronaTestProductTimes[make_tuple(ks, mrd, pl, 0, "REF")] = Interface.ssphh.lastREFTime;

					//coronaScene.WriteMaterials(ssg, ks);
					Corona_GenerateSphlINIT();
					coronaTestProductTimes[INITname] = { ks, mrd, pl, 0, "INIT" };
					coronaTestProductTimes[INITname].secsToRender = Interface.ssphh.lastINITTime;
					if (mrd == 3 && pl == 1) {
						Corona_GenerateSphlVIZ();
						coronaTestProductTimes[VIZname] = { ks, mrd, pl, 0, "VIZ" };
						coronaTestProductTimes[VIZname].secsToRender = Interface.ssphh.lastVIZTime;
					}

					Corona_GenerateSphlGEN();
					coronaTestProductTimes[GENname] = { ks, mrd, pl, 0, "GEN" };
					coronaTestProductTimes[GENname].secsToRender = Interface.ssphh.lastGENTime;

					for (auto d : degrees) {

						std::ostringstream name;
						name << "HIER" << d;
						Interface.ssphh.MaxDegrees = d;
						Corona_GenerateSphlHIER();
						coronaTestProductTimes[HIERname] = { ks, mrd, pl, d, name.str() };
						coronaTestProductTimes[HIERname].secsToRender = Interface.ssphh.lastHIERTime;

						Interface.saveScreenshot = false;
						double frameTime = 0.0;
						for (int i = 0; i < 10; i++) {
							OnRender3D();
							frameTime += my_hud_info.totalRenderTime;
						}
						frameTime /= 10.0;

						Interface.saveScreenshot = true;
						OnRender3D();
						coronaTestProductTimes[SSHHname] = { ks, mrd, pl, d, "SSHH" };
						coronaTestProductTimes[SSHHname].secsToRender = frameTime;
						Corona_GenerateCompareProduct(ks, mrd, pl, d);

						count++;
						float progress = (float)((double)count / (double)totalProducts);
						std::string pname = GetPathTracerSphlRenderName(Interface.sceneName, "SSHH", ks, mrd, pl, d);
						HFLOGINFO("%4d/%4d (%3.2f%% done) products -- finished %s", count, totalProducts, progress,
								  pname.c_str());
					}
				}
			}
		}

		std::ofstream fout(Interface.sceneName + "_stats.csv",
						   Interface.ssphh.genProductsIgnoreCache ? std::ios::out : std::ios::app);
		std::string dtg = HFLOG_DTG();
		fout << dtg << "\n" << "name,dtg,ks,mrd,pl,md,time,ptE,sphlE,d1E,d2E" << "\n";
		for (auto& [name, t] : coronaTestProductTimes) {
			t.dtg = dtg;
			t.pathTracerTotalEnergy = Interface.ssphh.lastPathTracerTotalEnergy;
			t.sphlRenderTotalEnergy = Interface.ssphh.lastSphlRenderTotalEnergy;
			t.diff1TotalEnergy = Interface.ssphh.lastDiff1TotalEnergy;
			t.diff2TotalEnergy = Interface.ssphh.lastDiff2TotalEnergy;
			t.writeln(fout);
		}
		fout.close();

		Interface.ssphh.GEN_MaxRayDepth = gmrd;
		Interface.ssphh.VIZ_MaxRayDepth = vmrd;
		Interface.ssphh.REF_MaxRayDepth = rmrd;
		Interface.ssphh.GEN_PassLimit = gpl;
		Interface.ssphh.VIZ_PassLimit = vpl;
		Interface.ssphh.REF_PassLimit = rpl;
		Interface.ssphh.GEN_IgnoreCache = gic;
		Interface.ssphh.VIZ_IgnoreCache = vic;
		Interface.ssphh.REF_IgnoreCache = ric;
		Interface.ssphh.MaxDegrees = lastDegrees;
	}

	void SSPHH_Application::Corona_GenerateCompareProduct(bool ks, int mrd, int pl, int md) {
		std::string pathtracer_name = GetPathTracerName(Interface.sceneName, "CRNA", ks, mrd, pl);
		std::string sphlrender_name = GetPathTracerSphlRenderName(Interface.sceneName, "SSHH", ks, mrd, pl, md);
		std::string stats_name = GetStatsName(Interface.sceneName, "STATS", ks, mrd, pl, md);

		Image3f image1;
		Image3f image2;
		image1.loadPPM(Interface.ssphh.lastREFPath);
		image2.loadPPM(Interface.ssphh.lastSphlRenderPath);

		PPMCompare ppmcompare;

		ppmcompare.Init(ks, mrd, pl, md);
		ppmcompare.Compare(image1, image2);
		ppmcompare.SaveResults(Interface.sceneName, pathtracer_name,
							   Interface.ssphh.ppmcompareGenPPMs,
							   Interface.ssphh.ppmcompareIgnoreCache);

		Interface.ssphh.lastPathTracerTotalEnergy = ppmcompare.image1stat.sumI;
		Interface.ssphh.lastSphlRenderTotalEnergy = ppmcompare.image2stat.sumI;
		Interface.ssphh.lastDiff1TotalEnergy = ppmcompare.diffstat.sumI;
		Interface.ssphh.lastDiff2TotalEnergy = ppmcompare.absdiffstat.sumI;

		Interface.ssphh.ppmcompareIgnoreCache = false;
	}

	void SSPHH_Application::CoronaEraseTestProducts() {}

} // namespace SSPHH