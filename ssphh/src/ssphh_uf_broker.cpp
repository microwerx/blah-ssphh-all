#include "pch.hpp"
#include <ssphh_unicornfish.hpp>

namespace Uf {
	void DoBroker(const char* endpoint, Unicornfish* context) {
		if (!context) return;
		Uf::Broker broker;
		context->SetUIMessage(Unicornfish::NodeType::Broker, "started");
		bool result = broker.Create(endpoint);
		while (result && !context->IsStopped()) {
			result = broker.RunLoop();
			std::ostringstream ostr;
			ostr << "Workers Total/Waiting/Requests: ";
			ostr << broker.GetNumWorkers() << "/";
			ostr << broker.GetNumWaitingWorkers() << "/";
			ostr << broker.GetNumRequests();
			context->SetUIMessage(Unicornfish::NodeType::Broker, ostr.str());
		}
		if (!result) {
			HFLOGINFO("%s(): broker: error!");
		}
		broker.Delete();
		HFLOGINFO("broker: okay, quitting");
		context->SetUIMessage(Unicornfish::NodeType::Broker, "stopped");
	}
}
