#include "AIProcessData.h"
#include "AIProcessFunction.h"
#include "FormUtil.h"

namespace MaxsuAIProcessFixNamespace
{
#define SETTINGFILE_PATH "Data\\SKSE\\Plugins\\MaxsuAIProcessFixFiles\\AIProcessFixSettings.ini"
#define PATCHFILE_PATH 	"Data\\SKSE\\Plugins\\MaxsuAIProcessFixFiles\\AIProcessFixModPatch.ini"

	AIProDataHandler::AIProDataHandler()
	{
		_MESSAGE("Call AI Process Data Handler Constructor!");

	//------------------------------------Trigger Game Hours Settings---------------------------------------------------------

		auto get_trigger_hours = GetPrivateProfileIntA("Settings", "MinimumTriggerHours", -1, SETTINGFILE_PATH);

		if (2 <= get_trigger_hours && get_trigger_hours <= 6)
		{
			min_trigger_hours = get_trigger_hours;

			_MESSAGE("Set Minimum Trigger Hours to {} from INI", min_trigger_hours);
		}
		else
		{
			_MESSAGE("Can't get correct Minimum Trigger Hours setting from INI");
		}
	//-------------------------------------------------------------------------------------------------------------------------------



	//------------------------------------Enable Update After Waiting-----------------------------------------------------------------

		auto IsEnableUpdateAfterWaiting = GetPrivateProfileIntA("Settings", "EnableUpdateAfterWaiting", -1, SETTINGFILE_PATH);

		if (IsEnableUpdateAfterWaiting == 0 || IsEnableUpdateAfterWaiting == 1)
		{
			IsEnableUpdateAfterWaiting == 0 ? EnableUpdateAfterWaiting = false : EnableUpdateAfterWaiting = true;
			EnableUpdateAfterWaiting == true ? _MESSAGE("Set UpdateAfterWaiting to true") : _MESSAGE("Set UpdateAfterWaiting to false");
		}
		else
		{
			_MESSAGE("Can't get correct EnableUpdateAfterWaiting  setting from INI");
		}
	//---------------------------------------------------------------------------------------------------------------------------------------------



	//------------------------------------Enable Update After Sleeping---------------------------------------------------------------------------

		auto IsEnableUpdateAfterSleeping = GetPrivateProfileIntA("Settings", "EnableUpdateAfterSleeping", -1, SETTINGFILE_PATH);

		if (IsEnableUpdateAfterSleeping == 0 || IsEnableUpdateAfterSleeping == 1)
		{
			IsEnableUpdateAfterSleeping == 0 ? EnableUpdateAfterSleeping = false : EnableUpdateAfterSleeping = true;
			EnableUpdateAfterSleeping == true ? _MESSAGE("Set UpdateAfterSleeping to true") : _MESSAGE("Set UpdateAfterSleeping to false");
		}
		else
		{
			_MESSAGE("Can't get correct EnableUpdateAfterSleeping  setting from INI");
		}
	//---------------------------------------------------------------------------------------------------------------------------------------------



	//------------------------------------Enable Update After Fast Travel-------------------------------------------------------------------------

		auto IsEnableUpdateAfterFastTravel = GetPrivateProfileIntA("Settings", "EnableUpdateAfterFastTravel", -1, SETTINGFILE_PATH);

		if (IsEnableUpdateAfterFastTravel == 0 || IsEnableUpdateAfterFastTravel == 1)
		{
			IsEnableUpdateAfterFastTravel == 0 ? EnableUpdateAfterFastTravel = false : EnableUpdateAfterFastTravel = true;
			EnableUpdateAfterFastTravel == true ? _MESSAGE("Set UpdateAfterFastTravel to true") : _MESSAGE("Set UpdateAfterFastTravel to false");
		}
		else
		{
			_MESSAGE("Can't get correct EnableUpdateAfterFastTravel setting from INI");
		}
	//----------------------------------------------------------------------------------------------------------------------------------------------



	//------------------------------------Enable Update for Mod Added NPCs-------------------------------------------------------------------------

		auto IsEnableUpdateModAddNPCs = GetPrivateProfileIntA("Settings", "EnableUpdateModAddNPCs", -1, SETTINGFILE_PATH);

		if (IsEnableUpdateModAddNPCs == 0 || IsEnableUpdateModAddNPCs == 1)
		{
			IsEnableUpdateModAddNPCs == 0 ? EnableUpdateModAddNPCs = false : EnableUpdateModAddNPCs = true;
			EnableUpdateModAddNPCs == true ? _MESSAGE("Set UpdateModAddNPCs to true") : _MESSAGE("Set UpdateModAddNPCs to false");
		}
		else
		{
			_MESSAGE("Can't get correct EnableUpdateModAddNPCs setting from INI");
		}
		//----------------------------------------------------------------------------------------------------------------------------------------------





	//------------------------------------Generate AI Mod Name --------------------------------------------------------------------------------------

		GetPrivateProfileStringA("ModName", "CurrentAIModName", "None", CurrentAIModName, sizeof(CurrentAIModName), PATCHFILE_PATH);

		_MESSAGE("Get AI Overhaul Mod Name is : {}", CurrentAIModName);

	//----------------------------------------------------------------------------------------------------------------------------------------------
		//ActorsArray.reserve(actor_arryas_cap);

		//_MESSAGE("Reserve capcity for actor arrays!");

		_MESSAGE("Init AI Process Data Handler!");
	}



	void AIProDataHandler::GetLoadActors()	//Using when sleeping or waiting start
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

		//------------------------Record Result-----------------------------/

		unsigned int update_actornums = 0;

		//-----------------------------------------------------------------/


		for (auto& actorHandle : processLists->highActorHandles)
		{
			auto actorPtr = actorHandle.get();
			auto thisactor = actorPtr.get();


			if (ActorsMap.size() >= a_datahandler->actor_arryas_cap)
			{
				_DMESSAGE("Actor verctor amounts reaches to maxminum limts");
				return;
			}

			if (IsUpdatableNPC(thisactor))
			{
				auto thispackage = thisactor->GetActorRuntimeData().currentProcess->currentPackage.package;

				if (thispackage)
				{
					ActorsMap.emplace(thisactor->GetFormID(), std::make_shared<ActorAIProData>(thispackage->formID,true));

					_DMESSAGE("ActorMap Size is {}", ActorsMap.size());

					_DMESSAGE("Add a Loaded actor ID {}, Name {}", FormUtil::GetIdentifierFromForm(thisactor), thisactor->GetName());

					update_actornums++;
				}
			}
		}

		_DMESSAGE("Find {} Loaded actors to update in total", update_actornums);
	}


	void AIProDataHandler::AddthisLaterInActor(RE::Actor* thisactor)
	{
		if (IsUpdatableNPC)
		{
			if (ActorsMap.count(thisactor->formID) == 0)
			{
				auto thispackage = thisactor->GetActorRuntimeData().currentProcess->currentPackage.package;

				if (thispackage)
				{
					ActorsMap.emplace(thisactor->GetFormID(), std::make_shared<ActorAIProData>(thispackage->formID, false));

					_DMESSAGE("ActorMap Size is {}", ActorsMap.size());

					_DMESSAGE("Add a LaterIn actor ID {}, Name {}", FormUtil::GetIdentifierFromForm(thisactor), thisactor->GetName());
				}
			}
		}
	}


	void AIProDataHandler::UpdateActorsPosInMap()
	{
		//------------------------Record Result-----------------------------/

		unsigned int update_actornums = 0;

		std::unordered_map <RE::FormID, RE::BSFixedString> UpdatedActorsIDMap;

		//-----------------------------------------------------------------/

		_DMESSAGE("Start Updating Actor");

		std::unordered_map<RE::FormID, std::shared_ptr<ActorAIProData>>::iterator a_it;

		for (a_it = ActorsMap.begin(); a_it != ActorsMap.end(); a_it++)
		{
			auto thisactor = RE::TESForm::LookupByID<RE::Actor>(a_it->first);

			if (thisactor)
			{
				_DMESSAGE("Find a actor ID {}, Name {}", FormUtil::GetIdentifierFromForm(thisactor), thisactor->GetName());

				if (UpdateActorFunc::UpdatethisActorPos(thisactor, a_it->second->PackageId, a_it->second->IsLoaded))
				{
					_DMESSAGE("Update actor ID {}, Name {}", FormUtil::GetIdentifierFromForm(thisactor), thisactor->GetName());
					update_actornums++;
					UpdatedActorsIDMap.emplace(thisactor->formID, thisactor->GetName());
				}
				else
				{
					_DMESSAGE("Fail to Update actor ID {}, Name {}", FormUtil::GetIdentifierFromForm(thisactor), thisactor->GetName());
				}
			}
			else
			{
				_ERROR("Actor Not Found!");
			}
		}

		ActorsMap.clear();

		for (auto thispair : UpdatedActorsIDMap)
		{
			_DMESSAGE("Successfully Update a actor ID {}, Name {}", FormUtil::GetIdentifierFromFormID(thispair.first), thispair.second);
		}

		_DMESSAGE("Update {} actors in total", update_actornums);
	}

}
