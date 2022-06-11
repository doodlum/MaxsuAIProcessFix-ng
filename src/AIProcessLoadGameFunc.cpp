#include "AIProcessLoadGameFunc.h"
#include "AIProcessData.h"
#include "AIProcessEvent_SleepWait.h"
#include "AIProcessEvent_FastTravel.h"


namespace MaxsuAIProcessFixNamespace
{

	FastTravelEndEventHandler	g_fasttravelhandler;

	MapMenuEventHanlder		    g_mapmenuhandler;



	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kDataLoaded)
		{
			_MESSAGE("Data Load CallBack Trigger!");

			auto ai_handler = AIProDataHandler::GetSingleton();

			if (!ai_handler)
			{
				_ERROR("AI Process Data Handler NOT FOUND!");
				return;
			}

			RE::ScriptEventSourceHolder* thissource;

			auto ScriptEventSource = thissource->GetSingleton();

			if (!ScriptEventSource)
			{
				_ERROR("ScriptEventSource NOT FOUND!");
				return;
			}


			auto UIhandler = RE::UI::GetSingleton();

			if (!UIhandler)
			{
				_ERROR("UIhandler NOT FOUND!");
				return;
			}

			UIhandler->AddEventSink(&g_mapmenuhandler);

			_MESSAGE("Register MapMenu OpenClose Event");


			if (ai_handler->IsEnableUpdateAfterWaiting())
			{

				SleepWaitStartHandle<RE::TESWaitStartEvent>::RegisterSink("Wait");

				SleepWaitStopHandle<RE::TESWaitStopEvent>::RegisterSink("Wait");

				_MESSAGE("Register Wait Start Stop Event");
			}

			if (ai_handler->IsEnableUpdateAfterSleeping())
			{
				SleepWaitStartHandle<RE::TESSleepStartEvent>::RegisterSink("Sleep");

				SleepWaitStopHandle<RE::TESSleepStopEvent>::RegisterSink("Sleep");

				_MESSAGE("Register Sleep Start Stop Event");
			}

			if (ai_handler->IsEnableUpdateAfterFastTravel() && !REL::Module::IsVR)
			{
				ScriptEventSource->AddEventSink(&g_fasttravelhandler);
				_MESSAGE("Register Fast Travel Event");
			}
		}
		else if (msg->type == SKSE::MessagingInterface::kPostLoadGame || msg->type == SKSE::MessagingInterface::kNewGame)
		{
			_MESSAGE("Start Loading into the Game");

			auto ai_handler = AIProDataHandler::GetSingleton();

			if (!ai_handler)
			{
				_ERROR("AI Process Data Handler NOT FOUND!");
				return;
			}

			auto thiscalendar = RE::Calendar::GetSingleton();

			if (!thiscalendar)
			{
				_ERROR("Not calendar handler FOUND!");
				return;
			}

			ai_handler->SetLastRecordTime(thiscalendar->GetCurrentGameTime());

			_DMESSAGE("Last Record Time is {}", ai_handler->GetLastRecordTime());

		}
		else if (msg->type == SKSE::MessagingInterface::kSaveGame)
		{
			_MESSAGE("Saving the Game!");

			auto ai_handler = AIProDataHandler::GetSingleton();

			if (!ai_handler)
			{
				_ERROR("AI Process Data Handler NOT FOUND!");
				return;
			}

			if (ai_handler->GetActorsMapSize() > 0)
			{
				_FATALERROR("Abnormal! AI updating procecure broken in saving! Please report to the mod authour! (MaxsuAIProcessFix.DLL)");

				MessageBox(NULL, L"Abnormal! Caught a fatal error during saving! Please report to the mod author!", L"Error Occured: MaxsuAIProcessFix.DLL", MB_OK | MB_ICONHAND);

				abort();
			}
		}
	}


}
