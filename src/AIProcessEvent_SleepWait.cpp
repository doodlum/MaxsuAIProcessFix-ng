#include "AIProcessEvent_SleepWait.h"
#include "AIProcessFunction.h"
#include "AIProcessData.h"
#include <string.h>
#include <Windows.h>

namespace MaxsuAIProcessFixNamespace
{
	extern class MoveAttachEventHandler;
	

	EventResult MoveAttachEventHandler::ProcessEvent(const RE::TESMoveAttachDetachEvent* a_event, RE::BSTEventSource<RE::TESMoveAttachDetachEvent>* a_eventSource)
	{
		if (!a_event || !a_eventSource)
		{
			_ERROR("Event source NOT FOUND!");
			return EventResult::kContinue;
		}

		auto UIhandler = RE::UI::GetSingleton();

		if (!UIhandler)
		{
			_ERROR("UIhandler NOT FOUND!");
			return EventResult::kContinue;
		}

		if (!UIhandler->IsMenuOpen(SLEEP_WAIT_MENU_NAME) || !UIhandler->GameIsPaused() || !EventsGuard::getIsWaitSleepStart())
		{
			_DMESSAGE("Sleep Wait Menu Closed! Remove MoveAttach Event!");
			a_eventSource->RemoveEventSink(this);
			return EventResult::kContinue;
		}

		if (a_event->isCellAttached){
			auto target = a_event->movedRef.get();

			if (!target)
			{
				_ERROR("MoveRef Target NOT FOUND!");
				return EventResult::kContinue;
			}

			auto thisactor = target->As<RE::Actor>();

			if (!thisactor || thisactor->formType != RE::FormType::ActorCharacter)
			{
				_DMESSAGE("MoveRef Is Not Actor!");
				return EventResult::kContinue;
			}

			auto a_datahandler = AIProDataHandler::GetSingleton();

			if (!a_datahandler)
			{
				_ERROR("Not data handler FOUND!");

				return EventResult::kContinue;
			}

			this->AddthisLaterInActor(a_datahandler, thisactor);
		} 

		return EventResult::kContinue;
	}


	void MoveAttachEventHandler::AddthisLaterInActor(AIProDataHandler* thishandler, RE::Actor* thisactor)
	{
		thishandler->AddthisLaterInActor(thisactor);
	}


	void MoveAttachEventHandler::RegisterSink()
	{
		auto Scripthanlder = RE::ScriptEventSourceHolder::GetSingleton();

		if (!Scripthanlder)
		{
			_ERROR("Not Script handler FOUND!");
			return;
		}

		Scripthanlder->AddEventSink(MoveAttachEventHandler::GetSingleton());
	}

	void MoveAttachEventHandler::RemoveSink()
	{
		auto Scripthanlder = RE::ScriptEventSourceHolder::GetSingleton();

		if (!Scripthanlder)
		{
			_ERROR("Not Script Source handler FOUND!");
			return;
		}

		auto MoveAttachSource = Scripthanlder->GetEventSource<RE::TESMoveAttachDetachEvent>();

		if (!MoveAttachSource)
		{
			_ERROR("Not MoveAttachSource handler FOUND!");
			return;
		}

		MoveAttachSource->RemoveEventSink(MoveAttachEventHandler::GetSingleton());
	}

}