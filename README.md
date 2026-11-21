# KeepTicking — StarRupture Server Plugin

Prevents a [StarRupture](https://store.steampowered.com/app/1631270/StarRupture/) dedicated server from sleeping when no players are online. Without this, all machines on the server stop producing items the moment the last player disconnects.

**Target:** Dedicated server only

---

## Installation

1. Download the latest release ZIP from the [Releases](../../releases) page:
   - `KeepTicking-Server-*.zip`

2. Extract into your game's `Binaries\Win64\` folder. The ZIP contains a `Plugins\` folder — it will sit alongside your existing `dwmapi.dll`.

3. After the first launch, edit `Plugins\config\KeepTicking.ini` and set `Enabled=1`.

> **Requires [StarRupture-ModLoader](https://github.com/AlienXAXS/StarRupture-ModLoader) v1.25.0 or newer** to be installed first.

---

## Troubleshooting

| Problem | Solution |
|---|---|
| Server still sleeping | Confirm `Enabled=1` is set in `Plugins\config\KeepTicking.ini`. |
| Plugin not loading | Check `modloader.log` in `Binaries\Win64\` for errors. |
| Plugin refused after a game update | KeepTicking takes its game addresses and member offsets from the mod loader's version object map (the version map). The three game members it reads come from the game's own data on any server build. The address of the pause function comes from the version map, and KeepTicking falls back on its own byte pattern, which matches CL 127004, the current 0.3.5 server. If neither finds it, as can happen on a newer server build the version map has no entry for, the plugin is refused. `modloader.log` then has `[HookScan] 'KeepTicking' will NOT be loaded -- PluginInit will not be called.`, with the reason listed above it. Update the mod loader. |

On a dedicated server, keep the mod loader up to date. It carries the version map rows for each server build.

---

## Building from Source

Requires Visual Studio 2022 and the [StarRupture-Plugin-SDK](https://github.com/AlienXAXS/StarRupture-Plugin-SDK).

Clone the repo, open `KeepTicking_Plugin.sln`, and build the `Server Release|x64` configuration. The output DLL will be placed in `build\Server Release\Plugins\`.

Only the Server configurations build. The plain `Debug` and `Release` configurations stop with an error that names the Server ones.

---

## Credits

Original mod by [@AlienXAXS](https://github.com/AlienXAXS).

Maintained by [@msantoro12](https://github.com/msantoro12).

---

## Disclaimer

Use at your own risk. The authors are not responsible for any damage caused by using this software.
