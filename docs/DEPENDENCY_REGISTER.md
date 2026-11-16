# KENYA OPEN WORLD ENGINE - DEPENDENCY REGISTER

## Required Dependencies

### Graphics
- **Vulkan SDK**: Graphics API (Windows/Linux)
  - Version: 1.3+
  - Purpose: Rendering backend
  - Status: Not installed
  - Install: https://vulkan.org/download/

### Windowing & Input
- **SDL3**: Window, input, platform abstraction
  - Version: 3.0+
  - Purpose: Window management, input handling
  - Status: Not installed
  - Install: https://www.libsdl.org/download-release.php

### Physics
- **Jolt Physics**: Rigid body simulation
  - Version: 4.0+
  - Purpose: Collision, dynamics
  - Status: Not installed
  - Install: https://github.com/jrouwe/JoltPhysics

### Audio
- **OpenAL**: Audio playback
  - Version: 1.1+
  - Purpose: 3D audio, mixing
  - Status: Not installed
  - Install: https://www.openal.org/

### Scripting
- **Lua**: Scripting language
  - Version: 5.4+
  - Purpose: Gameplay scripting
  - Status: Not installed
  - Install: https://www.lua.org/

### Testing
- **Google Test**: Unit testing framework
  - Version: 1.14+
  - Purpose: Automated tests
  - Status: Not installed
  - Install: Via CMake FetchContent

## Build Tools
- **CMake**: 4.4.4 (installed)
- **Ninja**: 1.13.2 (installed)
- **Git**: 2.56.0.4 (installed)
- **Python**: 3.x (installed)

## Compiler Status
- **LLVM/Clang**: Installing (v19.1.2)
- **MSVC**: Not available
- **GCC**: Not available

## Next Steps
1. Complete LLVM installation
2. Add clang++ to PATH
3. Configure CMake to use Clang
4. Build and test
5. Install runtime dependencies (Vulkan, SDL3, etc.)