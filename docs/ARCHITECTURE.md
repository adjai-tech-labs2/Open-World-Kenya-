# KENYA OPEN WORLD ENGINE - ARCHITECTURE

## Overview
Production-grade C++20 game engine for Kenyan open-world game. Built on established low-level libraries with proprietary architecture.

## Design Principles
1. **Modular Architecture**: Independent, testable modules with clean interfaces
2. **C++20 Modernity**: Coroutines, concepts, modules-ready code
3. **Platform Abstraction**: Unified interface across Windows/Linux
4. **Resource Management**: RAII, handles, explicit ownership
5. **Fail-Fast**: Validation, assertions, structured error handling
6. **No Hidden Dependencies**: Explicit dependency injection

## Module Architecture

### Core Layer (engine/core)
- **Application**: Lifecycle, initialization, shutdown
- **Configuration**: Validated settings, config files
- **Logging**: Severity levels, sinks, async logging
- **Error Handling**: Result types, exceptions, diagnostics
- **Time**: Frame timing, real-time, fixed timestep
- **Filesystem**: Virtual FS, path handling, streaming

### Graphics Layer (engine/graphics)
- **Renderer**: Abstraction over Vulkan/D3D/Metal
- **Device Management**: GPU resources, pipelines
- **Materials**: Shader management, PBR
- **Meshes**: Vertex data, GPU buffers
- **Textures**: Image resources, samplers

### Scene Layer (engine/scene)
- **Scene Manager**: Scene lifecycle, transitions
- **Spatial Partitioning**: Octrees, BVH
- **Level of Detail**: Distance-based optimization
- **Culling**: Frustum, occlusion

### ECS Layer (engine/ecs)
- **Entity IDs**: Generation counters, handle validation
- **Component Storage**: Sparse sets, archetypes
- **Systems**: Processing pipelines
- **Queries**: Component access, filtering

### Physics Layer (engine/physics)
- **World**: Jolt integration, collision
- **Rigid Bodies**: Dynamics, constraints
- **Character Controller**: Movement, collision
- **Raycasting**: Queries, picking

### Audio Layer (engine/audio)
- **Device Management**: OpenAL context
- **Mixing**: 3D audio, effects
- **Sources**: Voice, music, SFX
- **Formats**: Decoding, streaming

### Asset Layer (engine/assets)
- **Manager**: Loading, caching, unloading
- **Handles**: Reference counting
- **Importers**: Format-specific parsing
- **Streaming**: Demand-based loading

### Scripting Layer (engine/scripting)
- **Lua Runtime**: Script execution
- **Bindings**: C++ <-> Lua bridge
- **Security**: Sandboxing, limits
- **Debugging**: Stack inspection

### World Layer (engine/world)
- **Sectors**: Spatial partitioning
- **Streaming**: Load/unload based on position
- **Terrain**: Height maps, textures
- **Roads**: Networks, junctions

### Platform Layer (engine/platform)
- **Windowing**: SDL3 integration
- **Input**: Keyboard, mouse, gamepad
- **Filesystem**: OS-specific paths
- **Threading**: Job system, async

## Data Flow
1. **Initialization**: Config -> Modules -> Systems
2. **Frame Loop**: Input -> Game Logic -> Render -> Present
3. **Shutdown**: Cleanup -> State Save -> Exit

## Dependency Graph
```
Core (no deps)
  ↑
Platform (Core)
  ↑
Graphics (Platform, Core)
  ↑
Scene (Graphics, ECS, Core)
  ↑
Assets (Filesystem, Core)
  ↑
World (Scene, Assets, Core)
```

## Concurrency Model
- Main thread: Game logic, rendering
- Worker threads: Asset loading, physics, audio
- Job system: Parallel tasks
- Message passing: Thread-safe communication

## Memory Management
- **Allocators**: Pool, arena, system
- **Assets**: Reference counting, LRU cache
- **Graphics**: Resource pools, recycling
- **Profiling**: Allocation tracking, leak detection

## Extensibility Points
1. Renderer backend (Vulkan, D3D, Metal)
2. Physics backend (Jolt, custom)
3. Audio backend (OpenAL, SDL)
4. Scripting language (Lua, Python)
5. Input devices (custom)
6. Asset formats (custom importers)