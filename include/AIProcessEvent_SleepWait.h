#pragma once
#include "AIProcess_Guards.h"

namespace MaxsuAIProcessFixNamespace
{
#define MAP_MENU_NAME "MapMenu"
#define SLEEP_WAIT_MENU_NAME "Sleep/Wait Menu"

	extern class AIProDataHandler;

	class MoveAttachEventHandler;

	using EventResult = RE::BSEventNotifyControl;

	template <typename Event>
	class SleepWaitStartHandle : public RE::BSTEventSink<Event>
	{
		static_assert(std::is_same_v<Event, RE::TESWaitStartEvent> || std::is_same_v<Event, RE::TESSleepStartEvent>, "Template Class SleepWaitStartHandle DataType Wrong!");

	public:
		virtual EventResult ProcessEvent(const Event* a_event, RE::BSTEventSource<Event>* a_eventSource)
		{
			_DMESSAGE("%s Start", this->EventName);

			EventsGuard::SetWaitSleepStartFlag(true);

			auto thiscalendar = RE::Calendar::GetSingleton();

			if (!thiscalendar) {
				_ERROR("Not calendar handler FOUND!");
				return EventResult::kContinue;
			}

			auto a_datahandler = AIProDataHandler::GetSingleton();

			if (!a_datahandler) {
				_ERROR("Not data handler FOUND!");
				return EventResult::kContinue;
			}

			a_datahandler->SetLastRecordTime(thiscalendar->GetCurrentGameTime());

			_DMESSAGE("Last Record Time is %f", a_datahandler->GetLastRecordTime());

			if (a_datahandler->GetActorsMapSize() > 0) {
				_ERROR("Abnormality! AI updating procecure broken in %s start!", this->EventName);
				return EventResult::kContinue;
			}

			if (EventsGuard::getIsUpdateReady()) {
				GetLoadActors();
				MoveAttachEventHandler::RegisterSink();
			} else {
				_ERROR("Abnormality! Not Update Ready in %s start!", this->EventName);
				return EventResult::kContinue;
			}

			return EventResult::kContinue;
		}

		static void RegisterSink(const std::string EventName)
		{
			auto ScriptHolder = RE::ScriptEventSourceHolder::GetSingleton();

			if (!ScriptHolder) {
				_ERROR("Not ScriptHolder handler Found!");
				return;
			}

			static SleepWaitStartHandle<Event> SWStart_handler(EventName);
			ScriptHolder->AddEventSink(&SWStart_handler);
			_MESSAGE(("Register %sStart Event Sink!"), SWStart_handler.EventName.c_str());
		}

		void GetLoadActors()
		{
			auto a_datahandler = AIProDataHandler::GetSingleton();
			if (a_datahandler)
				a_datahandler->GetLoadActors();
		}

	private:
		SleepWaitStartHandle(const std::string Event) :
			EventName(Event){};
		~SleepWaitStartHandle() = default;

		const std::string EventName;
	};


	template <typename Event>
	class SleepWaitStopHandle : public RE::BSTEventSink<Event>
	{
		static_assert(std::is_same_v<Event, RE::TESWaitStopEvent> || std::is_same_v<Event, RE::TESSleepStopEvent>, "Template Class SleepWaitStopHandle DataType Wrong!");

	public:
		virtual EventResult ProcessEvent(const Event* a_event, RE::BSTEventSource<Event>* a_eventSource)
		{
			_DMESSAGE("%s Finish", this->EventName);

			EventsGuard::SetWaitSleepStartFlag(false);

			MoveAttachEventHandler::RemoveSink();

			auto UIhandler = RE::UI::GetSingleton();

			if (!UIhandler) {
				_ERROR("UIhandler NOT FOUND!");
				return EventResult::kContinue;
			}

			auto thiscalendar = RE::Calendar::GetSingleton();

			if (!thiscalendar) {
				_ERROR("Not calendar handler FOUND!");
				return EventResult::kContinue;
			}

			auto a_datahandler = AIProDataHandler::GetSingleton();

			if (!a_datahandler) {
				_ERROR("Not data handler FOUND!");
				return EventResult::kContinue;
			}

			auto lasting_time = thiscalendar->GetCurrentGameTime() - a_datahandler->GetLastRecordTime();

			_MESSAGE("Last Record Time is %f, Current Game Time is %f", a_datahandler->GetLastRecordTime(), thiscalendar->GetCurrentGameTime());

			_MESSAGE("%s Lasting Time is %f", this->EventName, lasting_time);

			if (EventsGuard::getIsUpdateReady() /* && UIhandler->GameIsPaused()*/) {
				EventsGuard::SetsUpdateReadyFlag(false);

				if (lasting_time >= a_datahandler->GetMiunmiunTriggerHours()) {
					_MESSAGE("Update Actor Pos Trigger!");

					a_datahandler->UpdateActorsPosInMap();

					EventsGuard::SetsUpdateReadyFlag(true);
				} else {
					a_datahandler->ClearActorsMap();

					EventsGuard::SetsUpdateReadyFlag(true);

					_MESSAGE("Clear Actor Map");
				}

			} else {
				a_datahandler->ClearActorsMap();

				EventsGuard::SetsUpdateReadyFlag(true);

				_ERROR(("Abnormality! AI updating procecure broken in %s stop!"), this->EventName);
			}

			a_datahandler->SetLastRecordTime(thiscalendar->GetCurrentGameTime());

			return EventResult::kContinue;
		};


		static void RegisterSink(const std::string EventName)
		{
			auto ScriptHolder = RE::ScriptEventSourceHolder::GetSingleton();

			if (!ScriptHolder) {
				_ERROR("Not ScriptHolder handler Found!");
				return;
			}

			static SleepWaitStopHandle<Event> SWStop_handler(EventName);

			ScriptHolder->AddEventSink(&SWStop_handler);

			_MESSAGE(("Register %sStop Event Sink!"), SWStop_handler.EventName.c_str());
		}

	private:
		SleepWaitStopHandle(const std::string Event) :
			EventName(Event){};
		~SleepWaitStopHandle() = default;

		const std::string EventName;
	};


	class MoveAttachEventHandler : public RE::BSTEventSink<RE::TESMoveAttachDetachEvent>
	{
	public:
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESMoveAttachDetachEvent* a_event, RE::BSTEventSource<RE::TESMoveAttachDetachEvent>* a_eventSource);

		static void RegisterSink();

		static void RemoveSink();

		void AddthisLaterInActor(AIProDataHandler* thishandler, RE::Actor* thisactor);	//Add Actors that loaded into current cells durng waiting\sleeping process.

	private:
		MoveAttachEventHandler() = default;
		~MoveAttachEventHandler() = default;

		static MoveAttachEventHandler* GetSingleton()
		{
			static MoveAttachEventHandler moverefevent_hanlder;
			return &moverefevent_hanlder;
		}
	};

}
