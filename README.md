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


## NOTE
I may have not pushed all the repo files to Github. I may have ommited the datasets that have Nairobi's maps. Below, the description from where you can acquire the files is present.


# Nairobi GIS Database — Dataset Inventory and Sources

## 1. Overview

This directory uses geospatial datasets derived from OpenStreetMap (OSM).

The GIS datasets are intentionally excluded from Git version control because:
- Some files are larger than GitHub's 100 MB individual file limit.
- The datasets are downloadable independently.
- Keeping large geospatial files outside Git reduces repository size.
- Developers can download the datasets when setting up the project.

**Primary data provider:** Geofabrik  
**Original data source:** OpenStreetMap  
**Geographic coverage:** Kenya / Nairobi project data  
**Data format:** ESRI Shapefile

## 2. Official Download Sources

### Primary provider — Geofabrik

- Kenya download page: https://download.geofabrik.de/africa/kenya.html
- Latest Kenya shapefile archive: https://download.geofabrik.de/africa/kenya-latest-free.shp.zip
- Latest Kenya GeoPackage archive: https://download.geofabrik.de/africa/kenya-latest-free.gpkg.zip
- Kenya OSM PBF data: https://download.geofabrik.de/africa/kenya-latest.osm.pbf

The Shapefile archive contains the GIS layers used by this project.

### Alternative data source — OpenStreetMap

- OpenStreetMap: https://www.openstreetmap.org/
- OpenStreetMap copyright and licensing: https://www.openstreetmap.org/copyright

## 3. Dataset Inventory

Each shapefile layer consists of five associated files:

| Extension | Purpose |
|---|---|
| `.shp` | Geographic feature geometry |
| `.shx` | Geometry index |
| `.dbf` | Attribute table |
| `.prj` | Coordinate reference system information |
| `.cpg` | Character encoding information |

### 3.1 Administrative Areas

**Layer:** `gis_osm_adminareas_a_free_1`

Files:
- `gis_osm_adminareas_a_free_1.shp`
- `gis_osm_adminareas_a_free_1.shx`
- `gis_osm_adminareas_a_free_1.dbf`
- `gis_osm_adminareas_a_free_1.prj`
- `gis_osm_adminareas_a_free_1.cpg`

### 3.2 Buildings

**Layer:** `gis_osm_buildings_a_free_1`

Files:
- `gis_osm_buildings_a_free_1.shp`
- `gis_osm_buildings_a_free_1.shx`
- `gis_osm_buildings_a_free_1.dbf`
- `gis_osm_buildings_a_free_1.prj`
- `gis_osm_buildings_a_free_1.cpg`

### 3.3 Land Use

**Layer:** `gis_osm_landuse_a_free_1`

Files:
- `gis_osm_landuse_a_free_1.shp`
- `gis_osm_landuse_a_free_1.shx`
- `gis_osm_landuse_a_free_1.dbf`
- `gis_osm_landuse_a_free_1.prj`
- `gis_osm_landuse_a_free_1.cpg`

### 3.4 Natural Areas — Polygon Features

**Layer:** `gis_osm_natural_a_free_1`

Files:
- `gis_osm_natural_a_free_1.shp`
- `gis_osm_natural_a_free_1.shx`
- `gis_osm_natural_a_free_1.dbf`
- `gis_osm_natural_a_free_1.prj`
- `gis_osm_natural_a_free_1.cpg`

### 3.5 Natural Features

**Layer:** `gis_osm_natural_free_1`

Files:
- `gis_osm_natural_free_1.shp`
- `gis_osm_natural_free_1.shx`
- `gis_osm_natural_free_1.dbf`
- `gis_osm_natural_free_1.prj`
- `gis_osm_natural_free_1.cpg`

### 3.6 Places — Polygon Features

**Layer:** `gis_osm_places_a_free_1`

Files:
- `gis_osm_places_a_free_1.shp`
- `gis_osm_places_a_free_1.shx`
- `gis_osm_places_a_free_1.dbf`
- `gis_osm_places_a_free_1.prj`
- `gis_osm_places_a_free_1.cpg`

### 3.7 Places — Point Features

**Layer:** `gis_osm_places_free_1`

Files:
- `gis_osm_places_free_1.shp`
- `gis_osm_places_free_1.shx`
- `gis_osm_places_free_1.dbf`
- `gis_osm_places_free_1.prj`
- `gis_osm_places_free_1.cpg`

### 3.8 Places of Worship — Polygon Features

**Layer:** `gis_osm_pofw_a_free_1`

Files:
- `gis_osm_pofw_a_free_1.shp`
- `gis_osm_pofw_a_free_1.shx`
- `gis_osm_pofw_a_free_1.dbf`
- `gis_osm_pofw_a_free_1.prj`
- `gis_osm_pofw_a_free_1.cpg`

### 3.9 Places of Worship — Point Features

**Layer:** `gis_osm_pofw_free_1`

Files:
- `gis_osm_pofw_free_1.shp`
- `gis_osm_pofw_free_1.shx`
- `gis_osm_pofw_free_1.dbf`
- `gis_osm_pofw_free_1.prj`
- `gis_osm_pofw_free_1.cpg`

### 3.10 Points of Interest — Polygon Features

**Layer:** `gis_osm_pois_a_free_1`

Files:
- `gis_osm_pois_a_free_1.shp`
- `gis_osm_pois_a_free_1.shx`
- `gis_osm_pois_a_free_1.dbf`
- `gis_osm_pois_a_free_1.prj`
- `gis_osm_pois_a_free_1.cpg`

### 3.11 Points of Interest — Point Features

**Layer:** `gis_osm_pois_free_1`

Files:
- `gis_osm_pois_free_1.shp`
- `gis_osm_pois_free_1.shx`
- `gis_osm_pois_free_1.dbf`
- `gis_osm_pois_free_1.prj`
- `gis_osm_pois_free_1.cpg`

### 3.12 Protected Areas

**Layer:** `gis_osm_protected_areas_a_free_1`

Files:
- `gis_osm_protected_areas_a_free_1.shp`
- `gis_osm_protected_areas_a_free_1.shx`
- `gis_osm_protected_areas_a_free_1.dbf`
- `gis_osm_protected_areas_a_free_1.prj`
- `gis_osm_protected_areas_a_free_1.cpg`

### 3.13 Railways

**Layer:** `gis_osm_railways_free_1`

Files:
- `gis_osm_railways_free_1.shp`
- `gis_osm_railways_free_1.shx`
- `gis_osm_railways_free_1.dbf`
- `gis_osm_railways_free_1.prj`
- `gis_osm_railways_free_1.cpg`

### 3.14 Roads

**Layer:** `gis_osm_roads_free_1`

Files:
- `gis_osm_roads_free_1.shp`
- `gis_osm_roads_free_1.shx`
- `gis_osm_roads_free_1.dbf`
- `gis_osm_roads_free_1.prj`
- `gis_osm_roads_free_1.cpg`

### 3.15 Traffic Areas — Polygon Features

**Layer:** `gis_osm_traffic_a_free_1`

Files:
- `gis_osm_traffic_a_free_1.shp`
- `gis_osm_traffic_a_free_1.shx`
- `gis_osm_traffic_a_free_1.dbf`
- `gis_osm_traffic_a_free_1.prj`
- `gis_osm_traffic_a_free_1.cpg`

### 3.16 Traffic Features

**Layer:** `gis_osm_traffic_free_1`

Files:
- `gis_osm_traffic_free_1.shp`
- `gis_osm_traffic_free_1.shx`
- `gis_osm_traffic_free_1.dbf`
- `gis_osm_traffic_free_1.prj`
- `gis_osm_traffic_free_1.cpg`

### 3.17 Transport Areas — Polygon Features

**Layer:** `gis_osm_transport_a_free_1`

Files:
- `gis_osm_transport_a_free_1.shp`
- `gis_osm_transport_a_free_1.shx`
- `gis_osm_transport_a_free_1.dbf`
- `gis_osm_transport_a_free_1.prj`
- `gis_osm_transport_a_free_1.cpg`

### 3.18 Transport Features

**Layer:** `gis_osm_transport_free_1`

Files:
- `gis_osm_transport_free_1.shp`
- `gis_osm_transport_free_1.shx`
- `gis_osm_transport_free_1.dbf`
- `gis_osm_transport_free_1.prj`
- `gis_osm_transport_free_1.cpg`

### 3.19 Water Areas — Polygon Features

**Layer:** `gis_osm_water_a_free_1`

Files:
- `gis_osm_water_a_free_1.shp`
- `gis_osm_water_a_free_1.shx`
- `gis_osm_water_a_free_1.dbf`
- `gis_osm_water_a_free_1.prj`
- `gis_osm_water_a_free_1.cpg`

### 3.20 Waterways

**Layer:** `gis_osm_waterways_free_1`

Files:
- `gis_osm_waterways_free_1.shp`
- `gis_osm_waterways_free_1.shx`
- `gis_osm_waterways_free_1.dbf`
- `gis_osm_waterways_free_1.prj`
- `gis_osm_waterways_free_1.cpg`

## 4. Dataset Summary

| Category | Count |
|---|---:|
| GIS layer families | 20 |
| Shapefile geometry files (`.shp`) | 20 |
| Geometry index files (`.shx`) | 20 |
| Attribute database files (`.dbf`) | 20 |
| Projection files (`.prj`) | 20 |
| Encoding files (`.cpg`) | 20 |
| **Total excluded GIS files** | **100** |

## 5. Download and Installation

Download the latest Kenya Shapefile archive:

https://download.geofabrik.de/africa/kenya-latest-free.shp.zip

Extract the archive and locate the required layers.

Place the relevant files inside:

```text
Open-World(Kenya)/
└── NairobiDB/
    ├── gis_osm_buildings_a_free_1.shp
    ├── gis_osm_buildings_a_free_1.shx
    ├── gis_osm_buildings_a_free_1.dbf
    ├── gis_osm_buildings_a_free_1.prj
    ├── gis_osm_buildings_a_free_1.cpg
    └── ...
```

**Important:** Keep all five components of each shapefile together. A `.shp` file alone is not a complete dataset.

The exact available layers may differ between provider releases. Check the extracted archive before assuming every historical filename is present.

## 6. Licensing and Attribution

The underlying geographic data comes from OpenStreetMap contributors.

OpenStreetMap data is distributed under the Open Database License (ODbL).

When using or redistributing the data, provide appropriate attribution:

> © OpenStreetMap contributors

OpenStreetMap copyright and licensing information:

https://www.openstreetmap.org/copyright

Geofabrik download and licensing information:

https://www.geofabrik.de/data/download.html

## 7. Git Version Control Policy

The GIS datasets are excluded from Git because they are independently downloadable.

The following files must remain outside Git tracking:

- `.shp`
- `.shx`
- `.dbf`
- `.prj`
- `.cpg`

The `.gitignore` file contains the corresponding exclusions.

Do not commit large GIS datasets to the source repository.

Keep dataset acquisition instructions and setup scripts version-controlled so that other developers can reproduce the environment.
