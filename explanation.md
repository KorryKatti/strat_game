# Beginner's Guide to the C++ Strategy Game Code (`main.cpp`)

Welcome! This guide breaks down the C++ code in `main.cpp` to help you understand how it works. We'll go through it step-by-step.

## 1. The Setup: Includes and Namespace

```cpp
#include <bits/stdc++.h>
using namespace std;
```

-   `#include <bits/stdc++.h>`: A convenient header that includes most standard library headers.
-   `using namespace std;`: Lets us use standard library components like `cout` and `vector` without the `std::` prefix.

## 2. Storing Data: `struct`

The code uses `struct` to create custom data types for organizing variables.

### `struct Cell`
Holds information about a single square on the map: its coordinates (`row`, `col`), the `regionID` it belongs to, and the `regionName`.

### `struct derivedUnit`
Defines a blueprint for military units and buildings, including stats (`id`, `name`, `health`, `damage`, `buildTime`) and build requirements (`primaryResourcesNeeded`, `derivedUnitsNeeded`).

### `struct PlayerResources`

```cpp
struct PlayerResources {
    int oil = 0;
    int metal = 0;
    int gold = 0;
    int rare_earth = 0;
    int uranium = 0;
};
```

-   This simple `struct` is used to keep track of the player's inventory of resources. Each resource is an `int` that holds the current amount the player has.

## 3. Core Functions: Building the World

### Map Generation (`createGrid`, `isAreaFree`, `fillRegion`, `makeRegion`)
These functions work together to create a new game map with randomly placed, named regions.

### Display (`printMap`, `printRegionInfo`)
These functions render the map and its region data to the console, using color to make it readable.

### File Handling (`saveMap`, `loadMap`, `saveProfile`, `loadResources`)
-   `saveMap`/`loadMap`: Handle saving and loading the main map file (`data/map.txt`).
-   `saveProfile`: Saves the player's progress (like their chosen region and resources) to `data/profile.txt`.
-   `loadResources`: Reads the player's resource inventory from `data/profile.txt`.

### Gameplay Logic

-   `distributeResource(cells)`: After a map is loaded, this function randomly assigns resource types (like oil, metal) and production rates to the cells within each region. It saves this information to `data/resources.txt`.

-   `askUserForRegion(...)`: If a new game is started, this function lists all the regions on the map and asks the player to choose one to start in.

-   `calculateProduction(...)`: This function reads the `data/resources.txt` file and adds up the total production rate for each type of resource within a player's chosen region. For example, it might find that the player's region produces 50 oil and 30 metal per turn.

-   `advanceTurn(...)`: This is the function that moves the game forward. It does the following:
    1.  Loads the player's current resources from their profile.
    2.  Calls `calculateProduction` to see how many new resources were generated this turn.
    3.  Adds the new resources to the player's totals.
    4.  Increments the turn counter.
    5.  Saves all the updated information back to `data/profile.txt`.

-   `gameStart(...)`: If a `profile.txt` file already exists, this function is called to load the saved game. It displays a welcome message, the player's current resources, and their production rates per turn.

## 4. The Main Logic: `main()`

The `main()` function controls the overall flow of the program:

1.  **Setup**: It ensures a `data/` directory exists.
2.  **Check for Profile**: It looks for `data/profile.txt`.
3.  **Existing Game**: If a profile exists, it calls `gameStart()` to load the game and display the player's status.
4.  **New Game**: If no profile exists, it begins the new game setup:
    -   It first tries to load `data/map.txt`. If the map doesn't exist, it guides the user through creating one.
    -   Once the map is ready, it calls `distributeResource` to populate the world with resources.
    -   It then calls `askUserForRegion` to let the player choose their starting location.
    -   Finally, it creates the initial `profile.txt` by calling `saveProfile`, and the game is ready.

This structure creates a complete, playable loop where a player can start a new game, generate a unique world, and begin accumulating resources turn by turn.