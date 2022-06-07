#include "version.h"
#include "AIProcessLoadGameFunc.h"
#include "MaxsuLogsBackUp.h"


#define LOG_PATH L"\\My Games\\Skyrim Special Edition\\SKSE\\MaxsuAIProcessFix.log"

static const unsigned int backupfiles_num = 3;


extern "C" DLLEXPORT bool APIENTRY SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	
	MaxsuLogsBackUpNamespace::CreateBackUpLogs(MaxsuLogsBackUpNamespace::GetLogAbsolutetPath(LOG_PATH) , backupfiles_num);

	SKSE::Logger::OpenRelative(FOLDERID_Documents, LOG_PATH);
	SKSE::Logger::SetPrintLevel(SKSE::Logger::Level::kDebugMessage);
	SKSE::Logger::SetFlushLevel(SKSE::Logger::Level::kDebugMessage);
	SKSE::Logger::UseLogStamp(true);

	_MESSAGE("MaxsuAIProcessFix v%s", MYFP_VERSION_VERSTRING);

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "MaxsuAIProcessFix";
	a_info->version = MYFP_VERSION_MAJOR;

	if (a_skse->IsEditor()) {
		_FATALERROR("Loaded in editor, marking as incompatible!\n");
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		_FATALERROR("Unsupported runtime version %s!\n", ver.GetString().c_str());
		return false;
	}

	return true;
}


extern "C" DLLEXPORT bool APIENTRY SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{

	if (!SKSE::Init(a_skse)) 
	{
		_DMESSAGE("SKSE Not loaded!");
		return false;
	}

	_MESSAGE("MaxsuAIProcessFix loaded");


#ifdef TEST_PAPYRUS_FUNC

	auto papyrus = SKSE::GetPapyrusInterface();

	if (!papyrus)
	{
		_DMESSAGE("Papyrus Interface Not Found!");
		return false;
	}

	if (papyrus->Register(MaxsuAIProcessFixNamespace::RegisterFuncs))
	{
		_MESSAGE("Papyrus Functions Register");
	}

#endif



	auto g_message = SKSE::GetMessagingInterface();

	if (!g_message)
	{
		_ERROR("Messaging Interface Not Found!");
		return false;
	}

	if (g_message->RegisterListener(MaxsuAIProcessFixNamespace::EventCallback))
	{	
		_MESSAGE("Register Event Call Back!");
		return true;
	}

	return false;
}
