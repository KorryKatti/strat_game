# Beginner's Guide to the C++ Strategy Game Code (`main.cpp`)

Welcome! This guide breaks down the C++ code in `main.cpp` to help you understand how it works, even if you're new to programming. We'll go through it step-by-step.

## 1. The Setup: Includes and Namespace

```cpp
#include <bits/stdc++.h>
using namespace std;
```

-   `#include <bits/stdc++.h>`: This is a convenient but non-standard header file that includes almost all the standard library headers for you (like `<iostream>` for printing, `<vector>` for dynamic arrays, `<string>` for text, etc.). In larger projects, it's better to include only what you need, but for a small project like this, it's fine.
-   `using namespace std;`: This line tells the compiler that we want to use elements from the "standard" (`std`) namespace. This lets us write `cout` instead of `std::cout`, or `vector` instead of `std::vector`, making the code shorter.

## 2. Storing Data: `struct`

The code defines custom data types using `struct` to organize related variables.

### `struct Cell`

```cpp
struct Cell {
    int row;
    int col;
    int regionID;
    string regionName;
};
```

-   A `struct` is like a blueprint for creating an object. This one, `Cell`, is designed to hold all the information about a single square (or "cell") on our game map.
-   `int row; int col;`: The coordinates of the cell on the map grid.
-   `int regionID;`: A number that identifies which region this cell belongs to. `0` means it's unoccupied.
-   `string regionName;`: The name of the region (e.g., "zeta", "ironforge").

### `struct derivedUnit`

```cpp
struct derivedUnit {
    int id;
    string name;
    int health;
    int damage;
    unordered_map<int, int> primaryResourcesNeeded;
    unordered_map<int, int> derivedUnitsNeeded;
    int buildTime;
};
```

-   This `struct` is a blueprint for all the military units and buildings in the game.
-   `id`, `name`, `health`, `damage`, `buildTime`: These are the basic stats for a unit.
-   `unordered_map<int, int> primaryResourcesNeeded;`: This is a dictionary (or "map") that stores what raw materials are needed to build this unit. The first `int` is the resource ID, and the second `int` is the quantity.
-   `unordered_map<int, int> derivedUnitsNeeded;`: This map stores the prerequisite buildings or units needed. For example, you might need a `garage` (another unit) before you can build a `light_tank`.

## 3. Core Functions: Building the World

These are the functions that do the heavy lifting, from creating the map to saving it.

### `createGrid`, `isAreaFree`, `fillRegion`

-   `vector<vector<int>> createGrid(int rows, int cols)`: This function creates an empty 2D grid (a vector of vectors) of a given size, filling it with `0`s.
-   `bool isAreaFree(...)`: Before placing a new region, this function checks a rectangular area on the grid to make sure it's not already occupied by another region. It returns `true` if the area is empty (all `0`s).
-   `void fillRegion(...)`: Once an area is confirmed to be free, this function "paints" it by filling the specified rectangle on the grid with a given `regionId`.

### `makeRegion`: The Map Generator

```cpp
pair<vector<vector<int>>, unordered_map<int, string>> makeRegion(int rows, int cols) { ... }
```

This is the heart of the map generation.
-   It returns a `pair`, which is a standard C++ object that holds two different items. Here, it returns both the final `grid` and the `regionNames` map.
-   It uses a random number generator (`mt19937`) to decide the position and size of new regions.
-   It runs a loop (`while (tries--)`) that repeatedly tries to place a new rectangular region in a random spot.
-   If `isAreaFree` returns true, it calls `fillRegion` and assigns the new region a random name from a predefined `namePool`.

### `printMap` and `printRegionInfo`

-   These functions are for displaying the map to the user.
-   `printMap` iterates through the grid and prints each cell's ID. It uses special text codes (`"[31m"`) to add color to the output in the terminal, making it easier to see the different regions.
-   `printRegionInfo` calculates the boundaries of each region and prints a summary list showing their ID, name, location, and size.

### `saveMap` and `loadMap`: File Handling

-   `bool saveMap(...)`: This function saves the generated map to `data/map.txt`. It writes the dimensions first, then the grid data, and finally the list of region IDs and names. It uses an `ofstream` (output file stream).
-   `void loadMap(...)`: If `data/map.txt` already exists, this function reads it. It uses an `ifstream` (input file stream) to read the dimensions, grid, and region names, populating the variables passed into it.

## 4. The Main Logic: `main()`

```cpp
int main() { ... }
```

This is the entry point of the program. Here is its logical flow:

1.  **Create Directory**: `std::filesystem::create_directories("data");` ensures the `data` folder exists, preventing errors when trying to save the map file.
2.  **Check for Map**: `if (!mapExists(map_path))` checks if `data/map.txt` exists.
3.  **No Map Found (Generation Loop)**:
    -   If the map doesn't exist, it asks the user for the map size.
    -   It enters a `while (true)` loop.
    -   Inside the loop, it calls `makeRegion` to generate a new map, then `printRegionInfo` and `printMap` to show it to the user.
    -   It asks the user `if (askUserSatisfied())`. If the user types `y`, it calls `saveMap` and `break`s out of the loop.
    -   If the user is not satisfied, the loop repeats, generating a new map.
4.  **Map Found (Loading)**:
    -   If the map file *does* exist, it calls `loadMap` to read the file and populate the `grid` and `cells` variables.
    -   Finally, it calls `distributeResource(cells)`, which currently just prints a list of all the possible units in the game.

This structure allows the program to either create a new world from scratch or load a previously saved one, providing a foundation for a strategy game.
