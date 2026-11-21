#pragma once

#include "plugin_interface.h"
#include "Engine_classes.hpp"
#include "plugin_map.h"

// Every game function and every member of a game class the plugin takes from
// the loader's map for the running game build (see plugin_map.h), under one
// name each. Members are resolved once in OnPluginLoadHooks and read on the
// game thread with In(object), which is null when the object is null or the
// member did not resolve. A rename in the map is a one-line edit here. The byte
// pattern the plugin falls back on is in pause_controller.cpp.

#define KEEPTICKING_FIELD(Class, Member) SR_FIELD(decltype(SDK::Class::Member), Class, Member, 0)

namespace KeepTickingFields
{
	// The server's pause driver, which the plugin detours.
	inline constexpr const char* UpdatePause = "ACrGameStateBase::UpdatePause";

	// The idle check counts the net driver's client connections.
	inline auto WorldNetDriver             = KEEPTICKING_FIELD(UWorld, NetDriver);
	inline auto NetDriverClientConnections = KEEPTICKING_FIELD(UNetDriver, ClientConnections);

	// Reset to normal speed while nobody is online.
	inline auto WorldSettingsTimeDilation = KEEPTICKING_FIELD(AWorldSettings, TimeDilation);

	// Asks the loader for every member above. False when any did not resolve;
	// the loader has then recorded the failure and refuses the plugin.
	inline bool Resolve(IPluginSelf* self, IPluginHookScanner* scanner)
	{
		return PluginMap::ResolveFields(scanner, self,
			WorldNetDriver, NetDriverClientConnections, WorldSettingsTimeDilation);
	}
}
