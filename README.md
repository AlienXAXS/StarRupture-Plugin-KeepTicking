# KeepTicking — StarRupture Server Plugin

Prevents a [StarRupture](https://store.steampowered.com/app/1631270/StarRupture/) dedicated server from sleeping when no players are online. Without this, all machines on the server stop producing items the moment the last player disconnects.

**Target:** Dedicated server only

---

## Installation

1. Download the latest release ZIP from the [Releases](../../releases) page:
   - `KeepTicking-Server-*.zip`

2. Extract into your game's `Binaries\Win64\` folder. The ZIP contains a `Plugins\` folder — it will sit alongside your existing `dwmapi.dll`.

3. After the first launch, edit `Plugins\config\KeepTicking.ini` and set `Enabled=1`.

> **Requires [StarRupture-ModLoader](https://github.com/AlienXAXS/StarRupture-ModLoader)** to be installed first.

---

## Troubleshooting

| Problem | Solution |
|---|---|
| Server still sleeping | Confirm `Enabled=1` is set in `Plugins\config\KeepTicking.ini`. |
| Plugin not loading | Check `modloader.log` in `Binaries\Win64\` for errors. |

---

## Building from Source

Requires Visual Studio 2022 and the [StarRupture-Plugin-SDK](https://github.com/AlienXAXS/StarRupture-Plugin-SDK).

Clone the repo, open `KeepTicking_Plugin.sln`, and build the `Server Release|x64` configuration. The output DLL will be placed in `build\Server Release\Plugins\`.

---

## Disclaimer

Use at your own risk. The authors are not responsible for any damage caused by using this software.
