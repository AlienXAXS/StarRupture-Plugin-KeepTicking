#pragma once

#include "plugin_interface.h"

namespace KeepTickingConfig
{
	static const ConfigEntry CONFIG_ENTRIES[] = {
		{
			"General",
			"Enabled",
			ConfigValueType::Boolean,
			"true",
			"Enable or disable the plugin."
		}
	};

	static const ConfigSchema SCHEMA = {
		CONFIG_ENTRIES,
		sizeof(CONFIG_ENTRIES) / sizeof(ConfigEntry)
	};

	class Config
	{
	public:
		static void Initialize(IPluginSelf* self)
		{
			s_self = self;
			if (s_self)
				s_self->config->InitializeFromSchema(s_self, &SCHEMA);
		}

		static bool IsPluginEnabled()
		{
			return s_self ? s_self->config->ReadBool(s_self, "General", "Enabled", true) : false;
		}

	private:
		static IPluginSelf* s_self;
	};
}
