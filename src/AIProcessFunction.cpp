#include "AIProcessFunction.h"
#include "AIProcessData.h"


namespace MaxsuAIProcessFixNamespace
{

	bool UpdateActorFunc::MoveToEditorLocation(RE::Actor* thisactor)
	{
		if (!thisactor)
		{
			_ERROR("Actor not found when MoveToEditorLocation!");
			return false;

		}

		auto this_parentcell = thisactor->GetParentCell();

		if (!this_parentcell)
		{
			_ERROR("Actor Parent Cell Not Found!");
			return false;

		}

		if (!this_parentcell->IsInteriorCell())		// Can not Call "MoveToEditorLocation" Function When in Exterior,will cause CTD.
		{
			_DMESSAGE("Actor is in Exterior! Skip MoveToEditorLocation!");
			return false;
		}

		auto thisEditorLoc = thisactor->editorLocation;

		if (!thisEditorLoc)
		{
			_ERROR("Editor Location Not Found!");
			return false;

		}

		auto targhandler = thisEditorLoc->worldLocMarker;

		auto targMarker = targhandler.get().get();

		if (!targMarker)
		{
			_DMESSAGE("Target Marker Not Found!");
			return false;
		}

		if (targMarker->GetParentCell() == this_parentcell)		// Not need to call "MoveToEditorLocation" Function to reset actor, using MoveToNearestNavmesh Instead
		{
			_DMESSAGE("Target Marker is in the Same Cell with Actor!");
			return false;
		}

		thisactor->MoveTo(targMarker);
		_DMESSAGE("Move Actor to Editor Location!");

		return true;

	}

	bool UpdateActorFunc::UpdateThisLoadedActor(RE::Actor* thisactor, RE::FormID LastPackageID)
	{
		auto a_datahandler = AIProDataHandler::GetSingleton();

		if (!thisactor)
		{
			_DMESSAGE("Actor not found!");
			return false;
		}

		if (thisactor->IsDeleted())
		{
			_DMESSAGE("Actor is deleted!");
			return false;
		}

		if (!(thisactor->formFlags & RE::TESForm::RecordFlags::kPersistent) != 0)
		{
			_DMESSAGE("Actor is Not Persistent!");
			return false;
		}

		if (!a_datahandler->IsEnableUpdateModNPC() && !IsVanillaForm(thisactor->GetFormID()))
		{
			_DMESSAGE("Actor is Not Vanilla Form, Stop Update!");
			return false;
		}

		if (!thisactor->Is3DLoaded() || thisactor->IsDisabled())
		{
			_DMESSAGE("Actor not Loaded 3D!");
			return false;

		}

		auto cur_process = thisactor->currentProcess;

		if (!cur_process)
		{
			_DMESSAGE("Current Pocess not found!");
			return false;
		}

		auto cur_package = cur_process->currentPackage.package;

		if (!cur_package)
		{
			_DMESSAGE("Current Package not found!");
			return false;
		}

		RE::FormID cur_packgeID;

		auto aliaspack = thisactor->CheckForCurrentAliasPackage();

		if (aliaspack)
		{
			if (!IsFormInMod(aliaspack, a_datahandler->CurrentAIModName)){
				_DMESSAGE("Actor is a Alias!");
				return false;
			}

			cur_packgeID = aliaspack->formID;

		} else
			cur_packgeID = cur_package->formID;


		_DMESSAGE("current AI Package ID is %x, Last AI Pakcage ID is %x", cur_packgeID, LastPackageID);

		if (cur_packgeID == LastPackageID) {
			_DMESSAGE("Actor AI Package remain the same, Skip Position Update!");
			return false;
		}

		if (!MoveToEditorLocation(thisactor)) {
			if (MoveToNearestNavmesh(thisactor, 100.f))
				_DMESSAGE("Move Actor To Nearest Navemesh Position!");
			else {
				_DMESSAGE("Fail To Reset Actor AI State!");
				return false;
			}
		}

		auto currentscene = thisactor->GetCurrentScene();

		if (currentscene)
		{
			_DMESSAGE("Actor is in a scene!");
			return false;
		}

		if (thisactor->IsFlying())
		{
			_DMESSAGE("Actor is flying!");
			return false;

		}

		if (thisactor->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal)		//	Actor is Standing
		{
			_DMESSAGE("Updating a Actor!");

			MoveToPackageLocation(thisactor);

			thisactor->UpdateActor3DPosition();

			_DMESSAGE("Update Actor To Pos!");

		}

		return true;
	}

	bool UpdateActorFunc::UpdateThisLaterInActor(RE::Actor* thisactor, RE::FormID LastPackageID)
	{
		auto a_datahandler = AIProDataHandler::GetSingleton();

		if (!thisactor)
		{
			_DMESSAGE("Actor not found!");
			return false;
		}

		if (thisactor->IsDeleted())
		{
			_DMESSAGE("Actor is deleted!");
			return false;
		}

		if (!(thisactor->formFlags & RE::TESForm::RecordFlags::kPersistent) != 0)
		{
			_DMESSAGE("Actor is Not Persistent!");
			return false;
		}

		if (!a_datahandler->IsEnableUpdateModNPC() && !IsVanillaForm(thisactor->GetFormID()))
		{
			_DMESSAGE("Actor is Not Vanilla Form, Stop Update!");
			return false;
		}

		if (thisactor->IsDisabled())
		{
			_DMESSAGE("Actor Is Disabled!");
			return false;

		}

		auto cur_process = thisactor->currentProcess;

		if (!cur_process)
		{
			_DMESSAGE("Current Pocess not found!");
			return false;
		}

		if (cur_process->processLevel != RE::PROCESS_TYPE::kHigh)
		{
			_DMESSAGE("Actor Current AI is Not in High Level! Skip Update!");
			return false;
		}

		auto cur_package = cur_process->currentPackage.package;

		if (!cur_package) {
			_DMESSAGE("Current Package not found!");
			return false;
		}

		auto cur_cell = thisactor->GetParentCell();

		if (!cur_cell)
		{
			_ERROR("Actor Current Cell Not Found!");
			return false;
		}

		if (!cur_cell->IsAttached())
		{
			_DMESSAGE("Actor Current Cell is Not Attached!");
			return false;
		}

		auto currentscene = thisactor->GetCurrentScene();

		if (currentscene)
		{
			_DMESSAGE("Actor is in a scene!");
			return false;
		}

		if (thisactor->IsFlying())
		{
			_DMESSAGE("Actor is flying!");
			return false;
		}

		MoveToPackageLocation(thisactor, false);

		_DMESSAGE("Update Actor To Pos!");

		return true;
	}
}
