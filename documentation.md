# Strategy Game Map Generator & Resource System

This C++ program generates, displays, and saves a 2D game map composed of distinct, named regions. It also manages player profiles, resource production, and a turn-based game system.

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
2.  **Profile Check**: It checks for `data/profile.txt`.
    *   **If `data/profile.txt` is not found**:
        *   It checks for `data/map.txt`.
            *   **If `data/map.txt` is not found**: It enters a map creation loop, prompting the user for dimensions, generating a map, and saving it once the user is satisfied.
            *   **If `data/map.txt` is found**: It loads the map, distributes resources randomly across the map cells, and saves this to `data/resources.txt`. It then prompts the user to select a starting region.
        *   A new `profile.txt` is created, storing the chosen region, initial resources (all zero), and setting the turn count to 0.
    *   **If `data/profile.txt` is found**:
        *   The game starts by loading the player's profile, including their region, current resources, and turn number.
        *   It displays the current game state: resources, production rates, and a list of resources in the player's region.
        *   The program will then be ready to advance to the next turn (this part of the logic is not yet fully implemented in `main`).

## Core Functions

-   `main()`: The main entry point that orchestrates the program flow.
-   `makeRegion(rows, cols)`: Generates a grid with random rectangular regions.
-   `loadMap(...)`: Reads map data from `data/map.txt`.
-   `saveMap(...)`: Writes the map grid and region names to `data/map.txt`.
-   `printMap(grid)`: Renders a color-coded ASCII map.
-   `printRegionInfo(...)`: Displays a summary of each region.
-   `distributeResource(cells)`: Assigns primary resources with random production rates to cells within each region and saves this to `data/resources.txt`. It also prints a catalog of all buildable units.
-   `askUserForRegion(...)`: Prompts the user to select a starting region from the list of available regions.
-   `saveProfile(...)`: Creates or updates `data/profile.txt` with the player's current game state.
-   `loadResources(...)`: Reads the player's current resource totals from `data/profile.txt`.
-   `calculateProduction(...)`: Calculates the total resource production per turn for a specific region by reading `data/resources.txt`.
-   `advanceTurn(...)`: Loads the current profile, calculates and adds the next turn's resource production, increments the turn counter, and saves the updated profile.
-   `printRegionResources(...)`: Displays a summary of the resources and their production rates for the player's chosen region.
-   `gameStart(...)`: Loads an existing game from `data/profile.txt` and displays the player's current status.

## Data Structures

-   `struct Cell`: Represents a single cell on the map.
-   `struct derivedUnit`: Represents a buildable unit with stats and requirements.
-   `struct PlayerResources`: Holds the player's current totals for each primary resource.

## File Formats

### `data/map.txt`

1.  `rows cols`
2.  Grid data (region IDs)
3.  `id:name` for each region

### `data/resources.txt`

A text file where each line represents a resource-producing cell.
-   Format: `row col resource_id production_rate`
-   Example: `5 10 1 15` (Cell at (5,10) produces 15 units of resource 1 per turn).

### `data/profile.txt`

A key-value store for the player's game state.
-   Example:
    ```
    starting_region_id: 3
    starting_region_name: helix
    turns: 0
    oil: 0
    metal: 0
    gold: 0
    rare_earth: 0
    uranium: 0
    ```