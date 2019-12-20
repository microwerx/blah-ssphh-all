#include <ssphhapp.hpp>
#include <hatchetfish_stopwatch.hpp>

namespace SSPHH
{
	void SSPHH_Application::SSG_ReloadScene() {
		ssg.reset();
		Hf::StopWatch stopwatch;
		SSG_LoadScene();
		stopwatch.Stop();
		HFLOGINFO("SSG reload took %4.2f milliseconds", stopwatch.GetMillisecondsElapsed());
		Interface.lastScenegraphLoadTime = stopwatch.GetMillisecondsElapsed();
	}
}