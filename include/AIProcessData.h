#pragma once
#include "AIProcessEvent_SleepWait.h"

namespace MaxsuAIProcessFixNamespace
{
#define MOD_NAME_LENGTH 255

	extern class MoveAttachEventHandler;

	SleepWaitStartHandle<RE::TESWaitStartEvent>;
	SleepWaitStartHandle<RE::TESSleepStartEvent>;

	SleepWaitStopHandle<RE::TESWaitStopEvent>;
	SleepWaitStopHandle<RE::TESSleepStopEvent>;

	class UpdateActorFunc
	{

	protected:

		static inline bool UpdatethisActorPos(RE::Actor* thisactor, RE::FormID LastPackageID, bool IsLoaded)
		{
			if (IsLoaded)
				return UpdateThisLoadedActor(thisactor, LastPackageID);
			else
				return UpdateThisLaterInActor(thisactor, LastPackageID);
		}

	private:

		static bool MoveToEditorLocation(RE::Actor* thisactor);

		static bool UpdateThisLoadedActor(RE::Actor* thisactor, RE::FormID LastPackageID);

		static bool UpdateThisLaterInActor(RE::Actor* thisactor, RE::FormID LastPackageID);

	};


	class AIProDataHandler : public UpdateActorFunc
	{
		using EventResult = RE::BSEventNotifyControl;

		struct ActorAIProData
		{
			const RE::FormID	PackageId;
			const bool		    IsLoaded;

			explicit ActorAIProData(RE::FormID thisId, bool b_Loaded) : PackageId(thisId), IsLoaded(b_Loaded) {};
		};

	private:

		AIProDataHandler();

		~AIProDataHandler() = default;

		AIProDataHandler(const AIProDataHandler&) = delete;

		AIProDataHandler(AIProDataHandler&&) = delete;

		AIProDataHandler& operator= (const AIProDataHandler&) = delete;

		AIProDataHandler& operator= (AIProDataHandler&&) = delete;


	//------------------ Variable Data------------------------

		float			 a_lastrecordtime = 0.f;

		unsigned int	 min_trigger_hours = 2;

		const float		 one_hour_value = 0.0416f;

		bool			 EnableUpdateAfterWaiting = true;

		bool			 EnableUpdateAfterSleeping = true;

		bool			 EnableUpdateAfterFastTravel = true;

		bool			 EnableUpdateModAddNPCs = true;

	//--------------------------------------------------------



	//-----------------Container---------------------------------

		std::unordered_map <RE::FormID, std::shared_ptr<ActorAIProData>> ActorsMap;

	//-------------------------------------------------------------


	public:

		char CurrentAIModName[MOD_NAME_LENGTH] = { "" };

		const unsigned int actor_arryas_cap = 100;


		static AIProDataHandler* GetSingleton()
		{
			static AIProDataHandler singleton;
			return &singleton;
		}



		inline float	GetLastRecordTime() const { return a_lastrecordtime;}

		inline void		SetLastRecordTime(float a_time) { a_lastrecordtime = a_time;}

		inline float	GetMiunmiunTriggerHours() const { return min_trigger_hours * one_hour_value;}



		inline bool		IsEnableUpdateAfterWaiting() const { return EnableUpdateAfterWaiting;}

		inline bool		IsEnableUpdateAfterSleeping() const { return EnableUpdateAfterSleeping;}

		inline bool		IsEnableUpdateAfterFastTravel() const { return EnableUpdateAfterFastTravel;}

		inline bool		IsEnableUpdateModNPC() const{ return EnableUpdateModAddNPCs;}


		inline size_t   GetActorsMapSize() const { return ActorsMap.size();}

		void		    ClearActorsMap(){ ActorsMap.clear();}
		

		friend void SleepWaitStartHandle<RE::TESWaitStartEvent>::GetLoadActors();
		friend void SleepWaitStartHandle<RE::TESSleepStartEvent>::GetLoadActors();


		friend void MoveAttachEventHandler::AddthisLaterInActor(AIProDataHandler* thishandler, RE::Actor* thisactor);

		void UpdateActorsPosInMap();  //Update Actor Pos after Waiting or Sleeping

	private:
			void GetLoadActors();								//Get Loaded Actors when sleeping or waiting start
			void AddthisLaterInActor(RE::Actor* thisactor);		//Add a Later In Actor During sleeping or waiting Process
			

			

	};

}