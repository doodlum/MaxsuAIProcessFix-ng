#pragma once

namespace MaxsuAIProcessFixNamespace
{
	using EventResult = RE::BSEventNotifyControl;

	class FastTravelEndEventHandler : public RE::BSTEventSink <RE::TESFastTravelEndEvent>
	{

	public:
		virtual EventResult ProcessEvent(const RE::TESFastTravelEndEvent* a_event, RE::BSTEventSource<RE::TESFastTravelEndEvent>* a_eventSource);

	private:
		void UpdateActorsProcessPos();	//Update Actor Pos after FastTravel

	};


	class MapMenuEventHanlder : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
	{

	public:
		virtual EventResult ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource);

	};

}