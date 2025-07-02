# Strategy Game Map Generator & Resource System

This C++ program generates, displays, and saves a 2D game map composed of distinct, named regions. If a map file already exists, it loads the map data and displays a catalog of available in-game units.

## How to Compile and Run

You can compile the program using a C++ compiler like g++.

```bash
g++ main.cpp -o a.out -std=c++17
```

To run the compiled program:

```bash
./a.out
```

## Program Flow

1.  **Directory Creation**: On startup, the program ensures a `data/` directory exists.
2.  **Map Check**: It checks for the existence of `data/map.txt`.
    *   **If `data/map.txt` is not found**:
        *   It prompts the user for the desired map dimensions (rows and columns).
        *   It enters a loop to generate a new map (`makeRegion`).
        *   It displays the generated map (`printMap`) and a list of its regions (`printRegionInfo`).
        *   It asks the user if they are satisfied with the map.
        *   If the user is satisfied, it saves the map to `data/map.txt` (`saveMap`) and exits the loop.
        *   If not, it regenerates the map.
    *   **If `data/map.txt` is found**:
        *   It loads the map data from the file (`loadMap`), populating the grid and a vector of `Cell` objects.
        *   It then calls `distributeResource`, which defines and displays a comprehensive list of all available military units, their stats, and their build requirements (resources and prerequisite units).

## Core Functions

-   `main()`: The main entry point that orchestrates the program flow.
-   `makeRegion(rows, cols)`: Generates a grid and populates it with randomly sized and positioned rectangular regions. Each region is assigned a unique ID and a random name from a predefined list.
-   `loadMap(...)`: Reads map dimensions, grid data, and region names from `data/map.txt`. It uses this data to populate the game's grid and a `vector<Cell>` containing detailed information for each cell.
-   `saveMap(...)`: Writes the current map grid and region names to `data/map.txt`.
-   `printMap(grid)`: Renders a color-coded ASCII representation of the map in the console.
-   `printRegionInfo(...)`: Displays a summary of each region, including its ID, name, position, and size.
-   `distributeResource(cells)`: This function initializes data for the game's economy and military. It defines a list of primary resources (e.g., oil, metal) and a detailed catalog of `derivedUnit` structures. Each unit has stats (health, damage), build time, and resource/unit prerequisites. The function's current implementation prints this entire catalog to the console for review; it does not yet assign resources or units to the map itself.

## Data Structures

-   `struct Cell`: Represents a single cell on the map, containing its row, column, region ID, and the corresponding region name.
-   `struct derivedUnit`: Represents a military or infrastructure unit that can be built. It includes:
    *   `id`: A unique identifier.
    *   `name`: The unit's name.
    *   `health`, `damage`, `buildTime`: Core stats for the unit.
    *   `primaryResourcesNeeded`: A map of required primary resource IDs and the quantity needed.
    *   `derivedUnitsNeeded`: A map of required prerequisite unit IDs and the quantity needed, establishing a tech tree.

## Map File Format (`data/map.txt`)

The map data is stored in a simple text format:

1.  The first line contains the map dimensions: `rows cols`.
2.  The next `rows` lines contain the grid data, with each integer being a region ID, separated by spaces.
3.  The remaining lines define the region names in the format `id:name`.

**Example `data/map.txt`:**
```
10 20
0 0 1 1 0 0 ...
0 0 1 1 0 0 ...
...
1:zeta
2:gamma
...
```
