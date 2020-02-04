#include "pch.hpp"
#include <ssphhapp.hpp>
#include <ssphh_unicornfish.hpp>
#include <SphlJob.hpp>

namespace Uf {
	constexpr size_t CoronaJobSize = sizeof(Uf::CoronaJob);

	void DoOldWorker(const char* endpoint, const char* service, Unicornfish* context) {
		if (!context)
			return;
		context->SetUIMessage(Unicornfish::NodeType::Worker, "started");
		Uf::Worker worker;
		bool result = worker.ConnectToBroker(endpoint, service);
		while (result && !context->IsStopped()) {
			Uf::Message reply;
			if (worker.WaitRequest()) {
				Uf::Message request = worker.GetRequest();
				std::string jobName = request.PopString();
				Uf::CoronaJob job;
				request.PopMem(&job, CoronaJobSize);
				auto& frame = request.PopFrame();
				memcpy(&job, frame.GetData(), frame.SizeInBytes());

				reply = request;
				reply.Push("working");
				reply.Push(jobName);
				worker.SendReply(reply);

				std::string messageStr = "working ";
				messageStr += jobName;
				context->SetUIMessage(Unicornfish::NodeType::Worker, messageStr);

				// do the job!
				ssphh_widget_ptr->RunJob(job);
				job.MarkJobFinished();

				reply = request;
				reply.Push(&job, CoronaJobSize);
				reply.Push("finished");
				reply.Push(jobName);
				worker.SendReply(reply);
				memset(&job, 0, CoronaJobSize);

				context->SetUIMessage(Unicornfish::NodeType::Worker, "waiting");
			}
		}
		worker.Disconnect();
		HFLOGINFO("worker: okay, quitting -- was doing \"%s\"", service);
		context->SetUIMessage(Unicornfish::NodeType::Worker, "stopped");
	}

	void DoWorker(const char* endpoint, const char* service, Unicornfish* context) {
		if (!context)
			return;
		context->SetUIMessage(Unicornfish::NodeType::Worker, "started");
		Uf::Worker worker;
		bool result = worker.ConnectToBroker(endpoint, service);
		while (result && !context->IsStopped()) {
			Uf::Message reply;
			if (worker.WaitRequest()) {
				Uf::Message request = worker.GetRequest();
				std::string jobName = request.PopString();
				std::string jsonSphlJob = request.PopString();
				SphlJob sphlJob;
				sphlJob.parseJSON(jsonSphlJob);
				Uf::CoronaJob job;
				job.FromString(sphlJob.meta_coronaJob);
				Uf::CoronaJob job1;
				request.PopMem(&job1, CoronaJobSize);
				auto& frame = request.PopFrame();
				memcpy(&job, frame.GetData(), frame.SizeInBytes());

				reply = request;
				reply.Push("working");
				reply.Push(jobName);
				worker.SendReply(reply);

				std::string messageStr = "working ";
				messageStr += jobName;
				context->SetUIMessage(Unicornfish::NodeType::Worker, messageStr);

				// do the job!
				ssphh_widget_ptr->RunJob(job);
				job.MarkJobFinished();

				// report the results
				sphlJob.meta_coronaJob = job.ToString();
				job.CopySPHToSph4f(sphlJob.sphl);

				// The Stack is (from top)
				// frame
				// jobName: string
				// status: string
				// sphlJob: JSON string
				// coronaJob: CoronaJob
				reply = request;
				reply.Push(&job, CoronaJobSize);
				reply.Push(sphlJob.toJSON());
				reply.Push("finished");
				reply.Push(jobName);
				worker.SendReply(reply);
				memset(&job, 0, CoronaJobSize);

				context->SetUIMessage(Unicornfish::NodeType::Worker, "waiting");
			}
		}
		worker.Disconnect();
		HFLOGINFO("worker: okay, quitting -- was doing \"%s\"", service);
		context->SetUIMessage(Unicornfish::NodeType::Worker, "stopped");
	}
}
