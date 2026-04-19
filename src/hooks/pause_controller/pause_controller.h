#pragma once

struct IPluginHookUtils;
struct IPluginScanner;

namespace Hooks::PauseController
{
	void Install(IPluginScanner* scanner, IPluginHookUtils* hookUtils);
	void Uninstall(IPluginHookUtils* hookUtils);
}
