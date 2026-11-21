#pragma once

#include "plugin_interface.h"
#include "Engine_classes.hpp"
#include "plugin_map.h"

#include <cstdint>

// Every game function the plugin takes from the loader's map for the running
// game build (see plugin_map.h), and the three members it reads, under one name
// each. Members are resolved once in OnPluginLoadHooks and read on the game
// thread with In(object), which is null when the object is null or the member
// did not resolve. A rename in the map is a one-line edit here. The byte pattern
// the plugin falls back on is in pause_controller.cpp. The rest of the Engine
// the plugin touches (UWorld::K2_GetWorldSettings, UWorld::GetWorld) is not a
// game class and stays compiled in.

// A member of an Engine class with this plugin's own row for the build it was
// compiled on: the offset the Game-SDK has, which equals the loader's row. A
// build the map has no row for is refused, not read at a stale offset.
#define KEEPTICKING_FIELD(Class, Member) \
	SR_FIELD(decltype(SDK::Class::Member), Class, Member, PLUGIN_FIELD_NATIVE)

namespace KeepTickingFields
{
	// The server's pause driver, which the plugin detours.
	inline constexpr const char* UpdatePause = "ACrGameStateBase::UpdatePause";

	// The idle check counts the net driver's client connections.
	inline auto WorldNetDriver             = KEEPTICKING_FIELD(UWorld, NetDriver);
	inline auto NetDriverClientConnections = KEEPTICKING_FIELD(UNetDriver, ClientConnections);

	// Reset to normal speed while nobody is online.
	inline auto WorldSettingsTimeDilation = KEEPTICKING_FIELD(AWorldSettings, TimeDilation);

	// The game build the compiled offsets above were verified on: the plugin's
	// own row for a loader or build whose map has none.
	inline constexpr uint32_t kVerifiedCL = 127004;

	// Asks the loader for one member, with this plugin's own row as the fallback.
	template <class T>
	bool ResolveNative(IPluginHookScanner* scanner, const IPluginSelf* self, PluginField<T>& field)
	{
		const PluginFieldCandidate rows[] = { { kVerifiedCL, field.builtOffset, field.size, 0, nullptr } };
		return PluginMap::ResolveField(scanner, self, field, rows, 1);
	}

	// Asks the loader for every member above. False when any did not resolve;
	// the loader has then recorded the failure and refuses the plugin.
	inline bool Resolve(IPluginSelf* self, IPluginHookScanner* scanner)
	{
		bool ok = ResolveNative(scanner, self, WorldNetDriver);
		ok = ResolveNative(scanner, self, NetDriverClientConnections) && ok;
		return ResolveNative(scanner, self, WorldSettingsTimeDilation) && ok;
	}
}
