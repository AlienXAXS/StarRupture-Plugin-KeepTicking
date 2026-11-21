# Update notes

## Unreleased

- KeepTicking now takes its game values from the mod loader's version object map (the version map), so the same DLL runs on every supported server build. If the loader has no entry for your server build, the plugin is refused and the log says why.
- The three game members the idle check uses (the net driver, its client connections and the world's time dilation) are read through the game's reflection data instead of fixed offsets.
- Building the plain `Debug` or `Release` configuration now stops with a clear error that names the Server configurations, instead of quietly building against the wrong game files.
- Needs mod loader {LOADER_VERSION} or newer.
