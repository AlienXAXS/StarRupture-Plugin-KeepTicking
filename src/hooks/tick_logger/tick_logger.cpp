#include "tick_logger.h"
#include "plugin_helpers.h"
#include "plugin_interface.h"
#include <windows.h>

namespace Hooks::TickLogger
{
	using FUpdatePause = void*(__fastcall*)(void*);

	static HookHandle   g_updatePauseHandle = nullptr;
	static FUpdatePause Original_UpdatePause = nullptr;
	static uintptr_t    g_imageBase = 0;

	static void* __fastcall Detour_UpdatePause(void* thisPtr)
	{
		return nullptr;
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
