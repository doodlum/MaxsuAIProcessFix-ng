#include "AIProcessEvent_FastTravel.h"
#include "AIProcessData.h"
#include "AIProcessFunction.h"


namespace MaxsuAIProcessFixNamespace
{

	EventResult FastTravelEndEventHandler::ProcessEvent(const RE::TESFastTravelEndEvent* a_event, RE::BSTEventSource<RE::TESFastTravelEndEvent>* a_eventSource)
	{

		_DMESSAGE("Fast Travel End");

		auto thiscalendar = RE::Calendar::GetSingleton();

		if (thiscalendar)
		{
			_DMESSAGE("Fast Travel End Time is {}", thiscalendar->GetCurrentGameTime());
		}
		else
		{
			_ERROR("Calendar not Found !");
			return EventResult::kContinue;
		}

		auto a_datahandler = AIProDataHandler::GetSingleton();

		if (!a_datahandler)
		{
			_ERROR("Not data handler FOUND!");
			return EventResult::kContinue;
		}

		auto lasting_time = thiscalendar->GetCurrentGameTime() - a_datahandler->GetLastRecordTime();

		if (EventsGuard::getIsUpdateReady() && lasting_time >= a_datahandler->GetMiunmiunTriggerHours())
		{
			EventsGuard::SetsUpdateReadyFlag(false);

			_DMESSAGE("Trigger Fast Travel Update!");

			UpdateActorsProcessPos();

			EventsGuard::SetsUpdateReadyFlag(true);
		}

		a_datahandler->SetLastRecordTime(thiscalendar->GetCurrentGameTime());

		return EventResult::kContinue;

	}

	void FastTravelEndEventHandler::UpdateActorsProcessPos()
	{
		auto processLists = RE::ProcessLists::GetSingleton();

		if (!processLists)
		{
			_DMESSAGE("Not ProcessLists Found!");
			return;
		}

		auto a_datahandler = AIProDataHandler::GetSingleton();

		auto LoadActorNums = processLists->numberHighActors;

		_DMESSAGE("High Process Actor Nums = {}", LoadActorNums);

		if (LoadActorNums == 0)
		{
			_DMESSAGE("Not Actor Load Found!");
			return;
		}

		_DMESSAGE("Start Updating Actor");

		unsigned int update_actor_nums = 0;
		std::unordered_map <RE::FormID, RE::BSFixedString> UpdatedActorsIDMap;

		for (auto& actorHandle : processLists->highActorHandles)
		{
			auto actorPtr = actorHandle.get();
			auto thisactor = actorPtr.get();

			if (!thisactor)
			{
				_ERROR("Actor Not Found!");
				continue;
			}

			if (IsUpdatableNPC(thisactor))
			{
				_DMESSAGE("Find a actor name {}, ID {}", thisactor->GetName(), thisactor->formID);

				if (!thisactor->Is3DLoaded())
				{
					_DMESSAGE("Actor is Not 3D Loaded, Stop Update!");
					continue;
				}

				if (!a_datahandler->IsEnableUpdateModNPC() && !IsVanillaForm(thisactor->GetFormID()))
				{
					_DMESSAGE("Actor is Not Vanilla Form, Stop Update!");
					continue;
				}

				auto aliaspack = thisactor->CheckForCurrentAliasPackage();

				if (aliaspack)
				{
					if (!IsFormInMod(aliaspack, a_datahandler->CurrentAIModName))
					{
						_DMESSAGE("Actor is a Alias!");
						continue;
					}
				}

				auto currentscene = thisactor->GetCurrentScene();

				if (currentscene)
				{
					_DMESSAGE("Actor is in a scene!");
					continue;
				}

				if (thisactor->AsActorState()->IsFlying())
				{
					_DMESSAGE("Actor is flying!");
					continue;
				}

				MoveToPackageLocation(thisactor);


				thisactor->UpdateActor3DPosition();

				update_actor_nums++;

				UpdatedActorsIDMap.emplace(thisactor->GetFormID(), thisactor->GetName());

				_DMESSAGE("Update Actor Pos!");
			}
		}

		for (auto thispair : UpdatedActorsIDMap)
		{
			_DMESSAGE("Successfully Update a actor ID {:x}, Name {}", thispair.first, thispair.second);
		}

		_DMESSAGE("Update {} Actor in Total!", update_actor_nums);
	}



	EventResult MapMenuEventHanlder::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
	{
		if (!a_event)
			return EventResult::kContinue;


		if (!a_event->opening)
		{
			if (strncmp(MAP_MENU_NAME, a_event->menuName.data(), sizeof(MAP_MENU_NAME)) == 0)
			{
				auto thiscalendar = RE::Calendar::GetSingleton();

				if (!thiscalendar)
				{
					_ERROR("Calendar not Found !");
					return EventResult::kContinue;
				}

				auto a_datahandler = AIProDataHandler::GetSingleton();

				if (!a_datahandler)
				{
					_ERROR("Not data handler FOUND!");
					return EventResult::kContinue;
				}

				a_datahandler->SetLastRecordTime(thiscalendar->GetCurrentGameTime());

				_DMESSAGE("Last Record Time is {}", a_datahandler->GetLastRecordTime());

				_DMESSAGE("Map Menu Close!");

			}

		}

		return EventResult::kContinue;
	}





}
