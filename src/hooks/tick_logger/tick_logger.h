#pragma once

struct IPluginHookUtils;
struct IPluginScanner;

namespace Hooks::TickLogger
{
	void Install(IPluginScanner* scanner, IPluginHookUtils* hookUtils);
	void Uninstall(IPluginHookUtils* hookUtils);
}
