#include "AIProcessLoadGameFunc.h"

void Load()
{
#ifdef TEST_PAPYRUS_FUNC

	auto papyrus = SKSE::GetPapyrusInterface();

	if (!papyrus) {
		logger::debug("Papyrus Interface Not Found!");
		return false;
	}

	if (papyrus->Register(MaxsuAIProcessFixNamespace::RegisterFuncs)) {
		logger::info("Papyrus Functions Register");
	}

#endif

	auto g_message = SKSE::GetMessagingInterface();

	if (!g_message) {
		logger::error("Messaging Interface Not Found!");
	}

	if (g_message->RegisterListener(MaxsuAIProcessFixNamespace::EventCallback)) {
		logger::info("Register Event Call Back!");
	}
}