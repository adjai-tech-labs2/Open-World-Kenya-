# KENYA OPEN WORLD (KOW) - Game Engine Project

## Overview
Production-grade C++20 game engine for a Kenyan open-world game. Custom engine built on established low-level libraries (Vulkan, Jolt Physics, OpenAL, Lua) with proprietary architecture, rendering pipeline, scene system, and gameplay framework.

## Project Status
- **Phase**: Engine Foundation (Stage 1-8)
- **License**: Proprietary
- **Target**: Windows 11, Linux
- **Build System**: CMake + Ninja

## Architecture
The engine is organized into 20 major development modules:
1. Custom Game Engine (Core)
2. World Mapping & Geographic Intelligence
3. 3D World Construction
4. Character Creation & Human Simulation
5. Kenyan Language Intelligence
6. Character Intelligence & NPC AI
7. Vehicle Systems
8. Traffic & Transportation Intelligence
9. Mission & Story Engine
10. Combat & Interaction
11. Kenyan Culture & Lifestyle
12. Celebrity & Public Personality System
13. Economy & Business Simulation
14. Police, Law & Wanted System
15. Weather & Environmental Simulation
16. Audio & Music Production
17. UI/UX
18. Development Editor & Asset Pipeline
19. QA & Security
20. Distribution, Monetization & Live Operations

## Quick Start
```bash
# Build
mkdir build && cd build
cmake -G Ninja ..
ninja

# Run tests
ctest --output-on-failure
```

## Documentation
- [Architecture](docs/ARCHITECTURE.md)
- [Engine Specification](docs/ENGINE_SPECIFICATION.md)
- [Coding Standards](docs/CODING_STANDARDS.md)
- [Development Roadmap](docs/DEVELOPMENT_ROADMAP.md)
- [Dependency Register](docs/DEPENDENCY_REGISTER.md)