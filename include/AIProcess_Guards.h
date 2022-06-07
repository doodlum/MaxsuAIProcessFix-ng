#pragma once

namespace MaxsuAIProcessFixNamespace
{
	class EventsGuard
	{
	public:

		static void	SetsUpdateReadyFlag(bool flag) { IsUpdateReady = flag; }
		static bool	getIsUpdateReady() { return IsUpdateReady; }

		static void	SetWaitSleepStartFlag(bool flag) { IsWaitSleepStart = flag; }
		static bool	getIsWaitSleepStart() { return IsWaitSleepStart; }

	private:

		static bool		IsUpdateReady;
		static bool     IsWaitSleepStart;
	};

}