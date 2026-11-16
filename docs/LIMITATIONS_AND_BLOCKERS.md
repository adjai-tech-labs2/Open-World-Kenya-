# Limitations and Blockers (Foundation Delivery)

## Build/Verification Blocker (Exact)
- **CMake configure attempt:** `cmake -S . -B build`
- **Result:** Failed during compiler detection.
  - Generator selected: `NMake Makefiles`
  - Error: `CMAKE_CXX_COMPILER not set, after EnableLanguage`
  - Cause detail: running `'nmake' '-?'` failed with "no such file or directory" (NMake/MSVC toolchain not available)
- **Root cause:** No C++20 compiler found on PATH (`cl`, `g++`, `clang++`, `gcc` absent). Ninja also not installed.
- **Impact:** Cannot configure, compile, link, run `kow_sandbox`, or execute CTest. Source is scaffolded and intended to be build-ready once a toolchain is installed.
- **Next step:** Install a C++20 compiler (MSVC `cl` via Visual Studio Build Tools, or `clang++`/`g++` via LLVM/MinGW), ensure on PATH, then re-run `cmake -S . -B build`.

## Toolchain/Environment Constraints
- CMake 4.4.4.0 and Git 2.56.0.4 available.
- RAM ~7.78 GiB, Intel i5-8365U, Intel UHD 620 (no discrete GPU indicated): suitable for lightweight CPU builds/tests only; large asset builds and real-time rendering backends will be constrained.

## Unimplemented by Design (No False Claims)
The following are **interfaces and/or null stubs only**; no functional backends are implemented or tested:
- **Renderer/graphics backend:** `IRenderer`/`NullRenderer`, resource handles only. No Vulkan/DirectX/OpenGL device, no draw calls, no shading, no "polished renderer".
- **Physics:** `IPhysicsWorld`/`NullPhysicsWorld` stub. No rigid bodies, collision, vehicle dynamics.
- **Audio:** `IAudioSystem`/`NullAudioSystem` stub. No playback, decoding, or mixing.
- **Scripting:** `IScriptingRuntime`/`NullScriptingRuntime` stub. No VM integration or script execution.
- **Input/platform windowing:** `IInput`/`NullInput` stub. No SDL/window/event loop backend.
- **Assets:** `IAssetManager`/`NullAssetManager` registration-only stub. No file loading, parsing, textures, meshes, audio assets.
- **World streaming/geometry:** `WorldManager` supports logical sector registration/state only. `NairobiWorld` registers logical district sectors (no terrain, road meshes, buildings, LOD/streaming). `TrafficSystem` implements stub light-cycle logic only. `EnvironmentSystem` implements stub weather/time only.

## Explicitly Not Built (Requested Scope vs Reality)
Per project rules (no fake implementations, no downloads, no invented APIs), the following were **not** created as functional content/systems:
- Nairobi city map ingestion, full Nairobi build, districts beyond logical sector placeholders, terrain, or real geospatial conversion pipeline.
- Roads, junctions, traffic network meshes, navigation, or traffic simulation beyond stub light cycles.
- People/NPCs, character models/rigs/animations, crowds, or AI behaviors beyond stub character specs.
- Cars, matatus, boda bodas, vehicle models, driving/physics beyond stub vehicle specs.
- Street lights, signage, props, buildings/interiors, weather, day-night rendering beyond stub environment state.
- A "complete" game engine or playable world. This delivery is a **documented, test-targeted C++20 foundation scaffold** pending compiler verification.

## Responsible API/Network Note
- Web research was performed for Nairobi/OSM/Geofabrik/ODbL sources (documented in `docs/NAIROBI_DATA_RESEARCH.md`). No external runtime APIs were connected in code; no keys, downloads, or unverified integrations were added.
- Central activation command provided as a code entry point: `kow_sandbox` (initializes modules, prints diagnostics, shuts down cleanly) — subject to successful build.

## Project Readiness
- Source tree, CMake targets, core/ECS/scene/world/game implementations, subsystem interfaces, sandbox, and CTest-intended tests are in place. Project is ready for the next setup step: install C++20 compiler, configure, build, and run tests.
