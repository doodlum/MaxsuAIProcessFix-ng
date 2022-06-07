#pragma once

#include "RE/NetImmerse/NiSmartPointer.h"


namespace RE
{
	class TESObjectREFR;


	struct TESFurnitureEvent
	{
	public:
		enum class FurnitureEventType
		{
			kEnter = 0,
			kExit = 1
		};


		// members
		NiPointer<TESObjectREFR>							actor;			  // 00
		NiPointer<TESObjectREFR>							targetFurniture;  // 08
		FurnitureEventType									type;			  // 10
	};


	STATIC_ASSERT(sizeof(TESFurnitureEvent) == 0x18);
}
