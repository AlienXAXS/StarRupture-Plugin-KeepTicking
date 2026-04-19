#include "pause_controller.h"
#include "plugin_helpers.h"
#include "plugin_interface.h"
#include "sdk_helpers.h"
#include <windows.h>

namespace Hooks::PauseController
{
	using FUpdatePause = void*(__fastcall*)(void*);

	static HookHandle   g_updatePauseHandle = nullptr;
	static FUpdatePause Original_UpdatePause = nullptr;
	static uintptr_t    g_imageBase = 0;

	static void* __fastcall Detour_UpdatePause(void* thisPtr)
	{
		int32_t playerCount = SDKHelpers::GetPlayerCount();

		if (playerCount <= 0)
		{
			// No players online — skip pause logic and ensure time dilation is normal
			SDK::UWorld* World = SDKHelpers::GetWorld();
			if (World)
			{
				SDK::AWorldSettings* WorldSettings = World->K2_GetWorldSettings();
				if (WorldSettings)
					WorldSettings->TimeDilation = 1.0f;
			}
			return nullptr;
		}

		return Original_UpdatePause(thisPtr);
	}

	void Install(IPluginScanner* scanner, IPluginHookUtils* hookUtils)
	{
		if (!scanner || !hookUtils)
		{
			LOG_ERROR("[KeepTicking] scanner or hookUtils is null");
			return;
		}

		g_imageBase = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));

		const uintptr_t addr = scanner->FindPatternInMainModule(
			"40 53 55 56 41 56 41 57 48 83 EC ?? 45 33 FF 48 8B E9");

		if (!addr)
		{
			LOG_ERROR("[KeepTicking] UpdatePause pattern not found — server pause will NOT be suppressed");
			return;
		}

		g_updatePauseHandle = hookUtils->Install(
			addr,
			reinterpret_cast<void*>(Detour_UpdatePause),
			reinterpret_cast<void**>(&Original_UpdatePause)
		);

		if (g_updatePauseHandle)
			LOG_INFO("[KeepTicking] UpdatePause suppressed (abs: 0x%llX, RVA: 0x%llX)", addr, addr - g_imageBase);
		else
			LOG_ERROR("[KeepTicking] Pattern found but hook install failed (abs: 0x%llX)", addr);
	}

	void Uninstall(IPluginHookUtils* hookUtils)
	{
		if (g_updatePauseHandle && hookUtils)
		{
			hookUtils->Remove(g_updatePauseHandle);
			g_updatePauseHandle = nullptr;
			Original_UpdatePause = nullptr;
			LOG_INFO("[KeepTicking] UpdatePause hook removed");
		}
	}
}
