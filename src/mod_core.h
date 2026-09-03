#pragma once

#include "plugin_interface.h"

// Core mod logic — installs hooks against addresses already resolved in the
// plugin's OnPluginLoadHooks export.

namespace ModCore
{
	// Installs hooks/patches. Patterns are resolved earlier, during
	// OnPluginLoadHooks; nothing here may scan.
	void Initialize(IPluginHooks * hooks);

	// Cleanup — remove hooks, restore patches.
	void Shutdown();
}
