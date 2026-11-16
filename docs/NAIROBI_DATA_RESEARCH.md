# Nairobi Data Research (Web, 2026-11-10)

## Sources Identified
- **OpenStreetMap (OSM):** Primary open geographic data source for roads, buildings, amenities, transport features. https://www.openstreetmap.org/
- **Geofabrik Download Server:** Provides daily-updated regional OSM extracts in `.osm.pbf` (and other formats). Continent/country/region selection available; Kenya/Africa extracts are distributed via Geofabrik. https://download.geofabrik.de/ and https://download.geofabrik.de/africa.html (Kenya extract typically under Africa listings).
- **Routeable/vector derivatives:** Geofabrik offers routeable shape files/GeoPackages for graph/routing use cases (commercial service offerings documented separately). https://www.geofabrik.de/data/routeable-vector-data.html

## Licensing (Critical for Commercial Use)
- OSM-derived data on Geofabrik is provided under **Open Database License 1.0 (ODbL)**.
- Requirements (high level): attribute OpenStreetMap contributors as data source; databases derived from ODbL data must be shared under the same license (share-alike) when distributed as a derived database. Exact obligations depend on how data is adapted, extracted, and distributed in the product pipeline.
- Geofabrik public extracts omit user names/IDs/changeset IDs; full-metadata extracts are restricted to OSM contributors.

## Project Implications
- **Permitted direction:** Use OSM/Geofabrik-sourced geographic data with proper attribution and a reviewed ODbL compliance strategy (especially for derived databases/assets shipped or offered).
- **Not permitted here:** Bulk downloading, importing, or converting Nairobi datasets was not performed (no dependency/data downloads executed). No `google maps` extraction; Google Maps terms restrict bulk extraction/derivative map databases for commercial games.
- **Engine readiness:** `kow::world::IWorldManager`/`WorldManager` and asset interfaces provide logical registration points for future sector/map ingestion once data and pipeline are adopted under documented license compliance.

## Current State in Repository
- No Nairobi map data, road meshes, building geometry, people/character assets, vehicles, or street-light assets exist in this workspace.
- `WorldManager` registers a logical placeholder sector `Nairobi_CBD_Prototype` (coord 0,0) to validate sector management interfaces only; this is **not** built city geometry, roads, traffic, or population simulation.
