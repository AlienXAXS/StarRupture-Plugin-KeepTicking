#include "sdk_helpers.h"
#include "plugin_helpers.h"
#include "keepticking_fields.h"

namespace SDKHelpers
{
	SDK::UWorld* GetWorld()
	{
		// The plugin SDK's world getter: it takes GWorld from the loader's
		// globals when the loader has them, and the Game-SDK's otherwise.
		SDK::UWorld* World = PluginSdkRuntime::GetWorld();

		if (!World)
		{
			LOG_DEBUG("[SDK] UWorld::GetWorld() returned null");
			return nullptr;
		}

		return World;
	}

	SDK::UNetDriver* GetNetDriver(SDK::UWorld* World)
	{
		if (!World)
			return nullptr;

		SDK::UNetDriver** netDriver = KeepTickingFields::WorldNetDriver.In(World);
		return netDriver ? *netDriver : nullptr;
	}

	int32_t GetPlayerCount()
	{
		SDK::UWorld* World = GetWorld();
		if (!World)
		{
			LOG_DEBUG("[SDK] Cannot get player count - World is null");
			return -1;
		}

		// Try to get NetDriver
		SDK::UNetDriver* NetDriver = GetNetDriver(World);
		if (!NetDriver)
		{
			LOG_DEBUG("[SDK] Cannot get player count - NetDriver is null (might be listen server or single player)");
			return -1;
		}

		// Get client connections array
		// ClientConnections is TArray<UNetConnection*>
		const auto* connections = KeepTickingFields::NetDriverClientConnections.In(NetDriver);
		return connections ? connections->Num() : -1;
	}

	int32_t GetLocalPlayerCount(SDK::UWorld* World)
	{
		if (!World)
			return -1;

		// Access game instance
		SDK::UGameInstance* GameInstance = World->OwningGameInstance;
		if (!GameInstance)
		{
			LOG_DEBUG("[SDK] Cannot get local player count - GameInstance is null");
			return -1;
		}

		// Count local players
		int32_t localPlayerCount = GameInstance->LocalPlayers.Num();

		LOG_DEBUG("[SDK] Found %d local players via GameInstance", localPlayerCount);
		return localPlayerCount;
	}
}
