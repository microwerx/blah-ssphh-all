#include "pch.hpp"
#include <filesystem>
#include <ssphhapp.hpp>
#include <fluxions_ssphh_utilities.hpp>

namespace SSPHH {
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
			return os;
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

	std::string MakeSceneScnName(const std::string& product, int frame) {
		std::ostringstream ostr;
		ostr << Uf::CoronaJob::exportPathPrefix;
		ostr << product;
		ostr << "_" << std::setw(6) << std::setfill('0') << frame;
		ostr << ".scn";
		return ostr.str();
	}

	std::string MakeSceneScnNameSH(const std::string& product, int i) {
		std::ostringstream ostr;
		ostr << Uf::CoronaJob::exportPathPrefix << product << std::setw(2) << std::setfill('0') << i << ".scn";
		return ostr.str();
	}

	void SSPHH_Application::Corona_GenerateSCN() {
		// Algorithm
		// generate export_corona_cubemap_sphl_XX.scn      (64x64) where XX is the index of the SPHL
		// generate export_corona_ground_truth.png (1280x720)
		// run corona to generate export_corona_cubemap.png      (64x64)
		// run corona to generate export_corona_ground_truth.png (1280x720)
		ssg->requestedResolution.x = getWidth();
		ssg->requestedResolution.y = getHeight();

		for (int i = 0; i < MaxShLights; i++) {
			if (i >= sphls.size()) break;
			coronaScene.writeCubeMapSCN(MakeSceneScnNameSH("export_corona_cubemap_sphl_", i), ssg, sphls[i].position.xyz());
		}

		int frame = int(cameraAnimationTime * 10000);
		coronaScene.lastFrame = frame;
		coronaScene.writeSCN(MakeSceneScnName("export_corona_ground_truth", frame), ssg);
		coronaScene.writeCubeMapSCN(MakeSceneScnName("export_corona_ground_truth_cube", frame), ssg);
	}

	void SSPHH_Application::Corona_GenerateREF() {
		Corona_CheckCache();
		if (Interface->ssphh.enableREF) {
			Hf::StopWatch stopwatch;
			std::string REF_name = Fluxions::MakeREFName(
				Interface->sceneName,
				false,
				coronaScene.REF.enableHDR,
				coronaScene.REF.enableHQ,
				coronaScene.REF.enableSpecular,
				coronaScene.REF.maxRayDepth,
				coronaScene.REF.passLimit);
			Uf::CoronaJob job1(Interface->sceneName, REF_name, Uf::CoronaJob::Type::REF);
			if (coronaScene.currentConfig.enableHQ)
				job1.EnableHQ();
			if (coronaScene.currentConfig.enableHDR)
				job1.EnableHDR();
			job1.SetMaxRayDepth(coronaScene.REF.maxRayDepth);
			job1.SetPassLimit(coronaScene.REF.passLimit);
			job1.usePreviousRun(coronaScene.REF.usePreviousRun);
			// send it out
			if (!ssphhUf.IsStopped())
				ssphhUf.ScatterJob(job1);
			else
				job1.Start(coronaScene, ssg);
			Interface->ssphh.lastREFImagePath = job1.GetOutputPath();
			Interface->ssphh.lastREFTime = stopwatch.Stop_s();
		}

		if (Interface->ssphh.enableREFCubeMap) {
			Hf::StopWatch stopwatch;
			std::string REF_name = Fluxions::MakeREFName(
				Interface->sceneName,
				true,
				coronaScene.currentConfig.enableHDR,
				coronaScene.currentConfig.enableHQ,
				coronaScene.currentConfig.enableSpecular,
				coronaScene.REF.maxRayDepth,
				coronaScene.REF.passLimit);
			Uf::CoronaJob job2(Interface->sceneName, REF_name, Uf::CoronaJob::Type::REF_CubeMap);
			if (coronaScene.currentConfig.enableHQ)
				job2.EnableHQ();
			if (coronaScene.currentConfig.enableHDR)
				job2.EnableHDR();
			job2.SetMaxRayDepth(coronaScene.REF.maxRayDepth);
			job2.SetPassLimit(coronaScene.REF.passLimit);
			job2.usePreviousRun(coronaScene.REF.usePreviousRun);
			// send it out
			if (!ssphhUf.IsStopped())
				ssphhUf.ScatterJob(job2);
			else
				job2.Start(coronaScene, ssg);
			Interface->ssphh.lastREFCubeMapImagePath = job2.GetOutputPath();
			Interface->ssphh.lastREFCubeMapTime = stopwatch.Stop_s();
		}
	}

	void SSPHH_Application::Corona_GenerateSphlVIZ() {
		Corona_CheckCache();

		// Algorithm
		// for every pair of lights i, j where i != j,
		//     generate a SCN that represents the

		Hf::StopWatch stopwatch;

		int count = 0;
		int numLights = (int)ssgUserData->ssphhLights.size();
		Interface->ssphh.viz_times.resize(numLights, 0.0);

		for (int sendLight = 0; sendLight < numLights; sendLight++) {
			if (rendererContext->rendererConfigs["default"]->shaderDebugSphl >= 0 &&
				sendLight != rendererContext->rendererConfigs["default"]->shaderDebugSphl)
				continue;

			auto& sphl = ssgUserData->ssphhLights[sendLight];
			sphl->vizgenLightProbes.resize(numLights);

			for (int recvLight = 0; recvLight < numLights; recvLight++) {
				if (recvLight == sendLight)
					continue;

				int mrd = coronaScene.VIZ.maxRayDepth;
				int pl = coronaScene.VIZ.passLimit;
				std::string viz_name;
				viz_name = Fluxions::MakeVIZName(Interface->sceneName,
												 recvLight,
												 sendLight,
												 false,
												 false,
												 coronaScene.currentConfig.enableSpecular,
												 mrd,
												 pl);

				Uf::CoronaJob job(Interface->sceneName, viz_name, Uf::CoronaJob::Type::VIZ, sendLight, recvLight);
				if (coronaScene.currentConfig.enableHDR)
					job.EnableHDR();
				if (coronaScene.currentConfig.enableHQ)
					job.EnableHQ();
				job.SetMaxRayDepth(mrd);
				job.SetPassLimit(mrd);
				job.usePreviousRun(coronaScene.VIZ.usePreviousRun);
				job.SetImageDimensions(Interface->ssphh.LightProbeSize, Interface->ssphh.LightProbeSize);

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

		Interface->ssphh.lastVIZTime = stopwatch.Stop_s();
	}

	void SSPHH_Application::Corona_GenerateSphlINIT() {
		Hf::StopWatch stopwatch;
		ssphh.INIT(ssg);
		Interface->ssphh.lastINITTime = stopwatch.Stop_s();
		DirtySPHLs();
	}

	void SSPHH_Application::Corona_GenerateSphlHIER() {
		Hf::StopWatch stopwatch;
		ssphh.VIZmix = Interface->ssphh.HierarchiesMix;
		ssphh.HIER(Interface->ssphh.HierarchiesIncludeSelf, Interface->ssphh.HierarchiesIncludeNeighbors, Interface->ssphh.MaxDegrees);
		Interface->ssphh.lastHIERTime = stopwatch.Stop_s();
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
		Hf::StopWatch stopwatch;
		Interface->ssphh.gen_times.resize(numLights, 0.0);
		for (int sendLight = 0; sendLight < numLights; sendLight++) {
			if (rendererContext->rendererConfigs["default"]->shaderDebugSphl >= 0 &&
				sendLight != rendererContext->rendererConfigs["default"]->shaderDebugSphl)
				continue;
			auto& sphl = ssgUserData->ssphhLights[sendLight];
			sphl->vizgenLightProbes.resize(numLights);

			//sphl->index = sendLight;
			int mrd = coronaScene.GEN.maxRayDepth;
			int pl = coronaScene.GEN.passLimit;
			Uf::CoronaJob job(Interface->sceneName,
							  Fluxions::MakeGENName(
								  Interface->sceneName,
								  sendLight,
								  false,
								  false,
								  coronaScene.currentConfig.enableSpecular,
								  mrd,
								  pl),
							  Uf::CoronaJob::Type::GEN, sendLight);
			if (coronaScene.currentConfig.enableHDR)
				job.EnableHDR();
			if (coronaScene.currentConfig.enableHQ)
				job.EnableHQ();
			job.SetMaxRayDepth(mrd);
			job.SetPassLimit(pl);
			job.SetImageDimensions(Interface->ssphh.LightProbeSize, Interface->ssphh.LightProbeSize);
			job.usePreviousRun(coronaScene.GEN.usePreviousRun);

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

		Interface->ssphh.lastGENTime = stopwatch.Stop_s();
	}

	void SSPHH_Application::Corona_CheckCache() {
		//if (coronaScene.currentConfig.enableSpecular != coronaScene.currentConfig.enableSpecular) {
		//	coronaScene.currentConfig.enableSpecular = coronaScene.currentConfig.enableSpecular;
		//	coronaScene.currentConfig.clearCache = true;
		//}
	}

	void SSPHH_Application::Corona_EraseCache() {
		int filesRemoved = 0;
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
			std::string base_filename = Fluxions::MakeGENName(Interface->sceneName, i);

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
				if (std::filesystem::remove(target)) {
					filesRemoved++;
				}
			}

			// we count one more for the sun-to-sphl files
			for (int j = 0; j <= MaxSphlLights; j++) {
				std::string viz_base_filename = Fluxions::MakeVIZName(Interface->sceneName, i, j);

				for (auto& ext : ext_to_delete) {
					std::string target = viz_base_filename + ext;
					if (std::filesystem::remove(target)) {
						filesRemoved++;
					}
				}
			}
		}

		const std::string& gt = Fluxions::MakeREFName(Interface->sceneName, false);
		const std::string& gt_cm = Fluxions::MakeREFName(Interface->sceneName, true);
		const std::string& gt_mrdpl = Fluxions::MakeREFName(Interface->sceneName,
															false,
															coronaScene.currentConfig.enableHDR,
															false,
															coronaScene.REF.maxRayDepth,
															coronaScene.REF.passLimit);
		const std::string& gt_cm_mrdpl = Fluxions::MakeREFName(Interface->sceneName,
															   true,
															   coronaScene.currentConfig.enableHDR,
															   false,
															   coronaScene.REF.maxRayDepth,
															   coronaScene.REF.passLimit);

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
				if (std::filesystem::remove(target)) {
					filesRemoved++;
				}
			}
		}

		Interface->ssphh.cacheFilesRemoved = filesRemoved;
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

		auto gmrd = coronaScene.GEN.maxRayDepth;
		auto vmrd = coronaScene.VIZ.maxRayDepth;
		auto rmrd = coronaScene.REF.maxRayDepth;
		auto gpl = coronaScene.GEN.passLimit;
		auto vpl = coronaScene.VIZ.passLimit;
		auto rpl = coronaScene.REF.passLimit;
		auto gic = coronaScene.GEN.clearCache;
		auto vic = coronaScene.VIZ.clearCache;
		auto ric = coronaScene.REF.clearCache;
		auto lastDegrees = Interface->ssphh.MaxDegrees;

		int totalProducts = (int)(specular.size() * maxRayDepths.size() * passes.size() * degrees.size());
		int count = 0;

		// Render Ground Truth First
		coronaScene.currentConfig.clearCache = true;
		coronaScene.currentConfig.enableSpecular = true;
		coronaScene.writeSCN(ssg->name_str(), ssg);
		for (auto& mrd : maxRayDepths) {
			for (auto& pl : passes) {
				coronaScene.REF.maxRayDepth = mrd;
				coronaScene.REF.passLimit = pl;
				coronaScene.REF.clearCache = Interface->ssphh.genProductsIgnoreCache;
				Interface->ssphh.LightProbeSize = 32;
				Interface->ssphh.enableShowSPHLs = false;
				Interface->ssphh.enableShowHierarchies = false;
				coronaScene.currentConfig.enableSpecular = true;

				std::string ptname = GetPathTracerName(Interface->sceneName, "CRNA", true, mrd, pl);
				HFLOGINFO("Starting %s", ptname.c_str());

				//coronaScene.WriteMaterials(ssg, true);
				Corona_GenerateREF();
				coronaTestProductTimes[ptname] = { true, mrd, pl, 0, "REF" };
				coronaTestProductTimes[ptname].secsToRender = Interface->ssphh.lastREFTime;
			}
		}

		for (auto& ks : specular) {
			coronaScene.currentConfig.clearCache = true;
			coronaScene.currentConfig.enableSpecular = ks != 0;
			coronaScene.writeSCN(ssg->name_str(), ssg);
			for (auto& mrd : maxRayDepths) {
				for (auto& pl : passes) {
					coronaScene.GEN.maxRayDepth = mrd;
					coronaScene.GEN.passLimit = pl;
					coronaScene.GEN.clearCache = Interface->ssphh.genProductsIgnoreCache;
					coronaScene.VIZ.maxRayDepth = mrd;
					coronaScene.VIZ.passLimit = pl;
					coronaScene.VIZ.clearCache = Interface->ssphh.genProductsIgnoreCache;
					coronaScene.REF.maxRayDepth = mrd;
					coronaScene.REF.passLimit = pl;
					coronaScene.REF.clearCache = Interface->ssphh.genProductsIgnoreCache;
					Interface->ssphh.LightProbeSize = 32;
					Interface->ssphh.enableShowSPHLs = false;
					Interface->ssphh.enableShowHierarchies = false;
					coronaScene.currentConfig.enableSpecular = ks;

					std::string ptname = GetPathTracerName(Interface->sceneName, "CRNA", ks, mrd, pl);
					std::string INITname = GetPathTracerName(Interface->sceneName, "INIT", ks, mrd, pl);
					std::string VIZname = GetPathTracerName(Interface->sceneName, "VIZ_", ks, mrd, pl);
					std::string GENname = GetPathTracerName(Interface->sceneName, "GEN_", ks, mrd, pl);
					std::string HIERname = GetPathTracerName(Interface->sceneName, "HIER", ks, mrd, pl);
					std::string SSHHname = GetPathTracerName(Interface->sceneName, "SSHH", ks, mrd, pl);
					HFLOGINFO("Starting %s", ptname.c_str());

					//coronaScene.WriteMaterials(ssg, true);
					//Corona_GenerateREF();
					//coronaTestProductTimes[make_tuple(ks, mrd, pl, 0, "REF")] = Interface->ssphh.lastREFTime;

					//coronaScene.WriteMaterials(ssg, ks);
					Corona_GenerateSphlINIT();
					coronaTestProductTimes[INITname] = { ks, mrd, pl, 0, "INIT" };
					coronaTestProductTimes[INITname].secsToRender = Interface->ssphh.lastINITTime;
					if (mrd == 3 && pl == 1) {
						Corona_GenerateSphlVIZ();
						coronaTestProductTimes[VIZname] = { ks, mrd, pl, 0, "VIZ" };
						coronaTestProductTimes[VIZname].secsToRender = Interface->ssphh.lastVIZTime;
					}

					Corona_GenerateSphlGEN();
					coronaTestProductTimes[GENname] = { ks, mrd, pl, 0, "GEN" };
					coronaTestProductTimes[GENname].secsToRender = Interface->ssphh.lastGENTime;

					for (auto d : degrees) {

						std::ostringstream name;
						name << "HIER" << d;
						Interface->ssphh.MaxDegrees = d;
						Corona_GenerateSphlHIER();
						coronaTestProductTimes[HIERname] = { ks, mrd, pl, d, name.str() };
						coronaTestProductTimes[HIERname].secsToRender = Interface->ssphh.lastHIERTime;

						Interface->saveScreenshot = false;
						double frameTime = 0.0;
						for (int i = 0; i < 10; i++) {
							OnRender3D();
							frameTime += my_hud_info.totalRenderTime;
						}
						frameTime /= 10.0;

						Interface->saveScreenshot = true;
						OnRender3D();
						coronaTestProductTimes[SSHHname] = { ks, mrd, pl, d, "SSHH" };
						coronaTestProductTimes[SSHHname].secsToRender = frameTime;
						Corona_GenerateCompareProduct(ks, mrd, pl, d);

						count++;
						float progress = (float)((double)count / (double)totalProducts);
						std::string pname = GetPathTracerSphlRenderName(Interface->sceneName, "SSHH", ks, mrd, pl, d);
						HFLOGINFO("%4d/%4d (%3.2f%% done) products -- finished %s", count, totalProducts, progress,
								  pname.c_str());
					}
				}
			}
		}

		std::ofstream fout(Interface->sceneName + "_stats.csv",
			(Interface->ssphh.genProductsIgnoreCache ? std::ios::out : std::ios::app));
		std::string dtg = HFLOG_DTG();
		fout << dtg << "\n" << "name,dtg,ks,mrd,pl,md,time,ptE,sphlE,d1E,d2E" << "\n";
		for (auto& [name, t] : coronaTestProductTimes) {
			t.dtg = dtg;
			t.pathTracerTotalEnergy = Interface->ssphh.lastPathTracerTotalEnergy;
			t.sphlRenderTotalEnergy = Interface->ssphh.lastSphlRenderTotalEnergy;
			t.diff1TotalEnergy = Interface->ssphh.lastDiff1TotalEnergy;
			t.diff2TotalEnergy = Interface->ssphh.lastDiff2TotalEnergy;
			t.writeln(fout);
		}
		fout.close();

		coronaScene.GEN.maxRayDepth = gmrd;
		coronaScene.VIZ.maxRayDepth = vmrd;
		coronaScene.REF.maxRayDepth = rmrd;
		coronaScene.GEN.passLimit = gpl;
		coronaScene.VIZ.passLimit = vpl;
		coronaScene.REF.passLimit = rpl;
		coronaScene.GEN.clearCache = gic;
		coronaScene.VIZ.clearCache = vic;
		coronaScene.REF.clearCache = ric;
		Interface->ssphh.MaxDegrees = lastDegrees;
	}

	void SSPHH_Application::Corona_GenerateCompareProduct(bool ks, int mrd, int pl, int md) {
		std::string pathtracer_name = GetPathTracerName(Interface->sceneName, "CRNA", ks, mrd, pl);
		std::string sphlrender_name = GetPathTracerSphlRenderName(Interface->sceneName, "SSHH", ks, mrd, pl, md);
		std::string stats_name = GetStatsName(Interface->sceneName, "STATS", ks, mrd, pl, md);

		Image3f image1;
		Image3f image2;
		HFLOGDEBUG("NEED TO REDO THIS CODE!!!");
		return;
		//image1.loadPPM(Interface->ssphh.lastREFImagePath);
		//image2.loadPPM(Interface->ssphh.lastSphlRenderImagePath);

		PPMCompare ppmcompare;

		ppmcompare.Init(ks, mrd, pl, md);
		ppmcompare.Compare(image1, image2);
		ppmcompare.SaveResults(Interface->sceneName, pathtracer_name,
							   Interface->ssphh.ppmcompareGenPPMs,
							   Interface->ssphh.ppmcompareIgnoreCache);

		Interface->ssphh.lastPathTracerTotalEnergy = ppmcompare.image1stat.sumI;
		Interface->ssphh.lastSphlRenderTotalEnergy = ppmcompare.image2stat.sumI;
		Interface->ssphh.lastDiff1TotalEnergy = ppmcompare.diffstat.sumI;
		Interface->ssphh.lastDiff2TotalEnergy = ppmcompare.absdiffstat.sumI;

		Interface->ssphh.ppmcompareIgnoreCache = false;
	}

	void SSPHH_Application::Corona_EraseTestProducts() {}

} // namespace SSPHH
