# Strategy Game Map Generator

This C++ program generates, displays, and saves a 2D game map composed of distinct, named regions. If a map file already exists, it loads the map data instead of generating a new one.

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
        *   It loads the map data from the file (`loadMap`).
        *   It then calls `distributeResource`, which appears to be a placeholder or work-in-progress for assigning resources to the map cells.

## Core Functions

-   `main()`: The main entry point that orchestrates the program flow.
-   `makeRegion(rows, cols)`: Generates a grid and populates it with randomly sized and positioned rectangular regions. Each region is assigned a unique ID and a random name from a predefined list.
-   `loadMap(...)`: Reads map dimensions, grid data, and region names from `data/map.txt`.
-   `saveMap(...)`: Writes the current map grid and region names to `data/map.txt`.
-   `printMap(grid)`: Renders a color-coded ASCII representation of the map in the console.
-   `printRegionInfo(...)`: Displays a summary of each region, including its ID, name, position, and size.
-   `distributeResource(cells)`: A function intended to handle the logic for placing resources on the map. It currently contains placeholder data structures for primary resources and derived military units.

## Data Structures

-   `struct Cell`: Represents a single cell on the map, containing its row, column, region ID, and region name.
-   `vector<vector<int>>`: A 2D vector used as the primary grid structure, where each integer represents a region ID (0 for unoccupied).

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
