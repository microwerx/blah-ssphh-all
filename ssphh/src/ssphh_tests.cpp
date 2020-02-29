#include "pch.hpp"
#ifdef _WIN32
#include <direct.h>		// for _mkdir
#endif
#include <ssphhapp.hpp>

namespace SSPHH
{
	void SSPHH_Application::Test()
	{
		if (Interface.tests.bTestSPHLs) {
			TestSPHLs();
			Interface.tests.bTestSPHLs = false;
		}
	}

	void SSPHH_Application::TestSPHLs()
	{
		int &test = Interface.tests.saveSphlOBJ;
		FilePathInfo fpi("output");
		if (fpi.notFound()) {
			std::error_code ec;
			std::filesystem::create_directory("output", std::filesystem::current_path(), ec);
			fpi.reset("output");
		}		
		if (!fpi.isDirectory()) {
			HFLOGERROR("Path 'output' is not a directory");
			return;
		}
		test = 0;
		size_t count = 0;
		for (auto &sphl : ssgUserData->ssphhLights) {
			std::ostringstream ostr;
			ostr << ssg->name() << "_sphl" << count;
			if (!sphl.saveOBJ("output", ostr.str())) {
				HFLOGWARN("sphl.saveOBJ() failed to save %s", ostr.str().c_str());
				break;
			}
			count++;
		}

		if (count != ssgUserData->ssphhLights.size()) {
			HFLOGERROR("Did not save all the SPHLs as OBJ/MTL files");
			test = -1;
		}
		else {
			test = 1;
		}
	}
}
