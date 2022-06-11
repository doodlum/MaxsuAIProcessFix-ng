#pragma once

namespace MaxsuAIProcessFixNamespace
{
	class AIProDataHandler;

//-------------------------------Inline Functions--------------------------------------------------------------

	static inline bool IsFormInMod(RE::TESForm* thisform, const char* modname)
	{
		if (!thisform)
		{
			_DMESSAGE("Not Form Found!");
			return false;
		}

		auto datahandler = RE::TESDataHandler::GetSingleton();

		if (!datahandler)
		{
			_DMESSAGE("Not data handler Found!");
			return false;

		}

		auto thismod = datahandler->LookupModByName(modname);

		if (!thismod)
		{
			//_DMESSAGE("Not mod {} Found!", modname);
			return false;
		}

		return thismod->IsFormInMod(thisform->formID) ? true : false;

	}

	static inline bool IsVanillaForm(UInt32 thisformID)
	{
		return thisformID < 0x05000000 ? true : false;
	}

	static inline bool IsUpdatableNPC(const RE::Actor* thisactor)
	{
		return (thisactor && !thisactor->IsInCombat() && !thisactor->IsDead(false) && IsPersistent(thisactor) &&\
			   !thisactor->IsDisabled() && !thisactor->IsMarkedForDeletion()&& !thisactor->IsDeleted() && thisactor->CalculateCachedOwnerIsNPC() && !thisactor->IsDynamicForm() && thisactor->currentProcess);
	}
//------------------------------------------------------------------------------------------------------------


}
