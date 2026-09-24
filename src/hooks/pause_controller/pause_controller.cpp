#include "pause_controller.h"
#include "plugin_helpers.h"
#include "plugin_interface.h"
#include "sdk_helpers.h"
#include <windows.h>

namespace Hooks::PauseController
{
	using FUpdatePause = void*(__fastcall*)(void*);

	// Class::Function  UpdatePause (server pause driver)
	static constexpr const char* kUpdatePausePattern =
		"40 53 55 56 41 56 41 57 48 83 EC ?? 45 33 FF 48 8B E9";

	static HookHandle   g_updatePauseHandle = nullptr;
	static FUpdatePause Original_UpdatePause = nullptr;
	static uintptr_t    g_imageBase = 0;

	// Resolved during OnPluginLoadHooks; 0 means the pattern missed on this build.
	static uintptr_t    g_updatePauseAddr = 0;

	static void* __fastcall Detour_UpdatePause(void* thisPtr)
	{
		int32_t playerCount = SDKHelpers::GetPlayerCount();

		if (playerCount <= 0)
		{
			// No players online - skip pause logic and ensure time dilation is normal
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

	void Resolve(IPluginSelf* self, IPluginHookScanner* scanner)
	{
		if (!self || !scanner)
			return;

		// Optional: a miss leaves the plugin loaded but inert, which is what the
		// old scan-at-init path did. The loader still lists it for the user.
		//
		// FUNCTION_START: a detour is written over this address, so it has to be
		// a function's primary entry with room for the 14-byte jump. The
		// prologue here (40 53 55 56 41 56 41 57 ...) is an ordinary one that
		// plenty of other functions share, which is precisely when a match
		// needs checking against the exception directory rather than trusting
		// the bytes.
		PluginScanRequest req = PLUGIN_SCAN_REQUEST_INIT;
		req.hookName = "UpdatePause";
		req.pattern  = kUpdatePausePattern;
		req.kind     = PLUGIN_SCAN_FUNCTION_START;
		req.flags    = PLUGIN_SCAN_FLAG_OPTIONAL;

		g_updatePauseAddr = scanner->Resolve(self, &req);
	}

	void Install(IPluginHookUtils* hookUtils)
	{
		if (!hookUtils)
		{
			LOG_ERROR("[KeepTicking] hookUtils is null");
			return;
		}

		g_imageBase = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));

		const uintptr_t addr = g_updatePauseAddr;
		if (!addr)
		{
			LOG_ERROR("[KeepTicking] UpdatePause unresolved - server pause will NOT be suppressed");
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
