// the strategy game
// making it now
// i need a job dude fr
#include <bits/stdc++.h>
using namespace std;

struct Cell {
		int row;
		int col;
		int regionID;
		string regionName;
        int resourceID;
        int productionRate;
	};

// create an empty grid of 0s
vector<vector<int>> createGrid(int rows, int cols) {
    return vector<vector<int>>(rows, vector<int>(cols, 0));
}

// check if a rectangular area is unoccupied
bool isAreaFree(const vector<vector<int>>& grid, int r, int c, int h, int w) {
    if (r + h > grid.size() || c + w > grid[0].size()) return false;

    for (int i = r; i < r + h; ++i)
        for (int j = c; j < c + w; ++j)
            if (grid[i][j] != 0) return false;

    return true;
}

// fill a rectangular region with an ID
void fillRegion(vector<vector<int>>& grid, int r, int c, int h, int w, int id) {
    for (int i = r; i < r + h; ++i)
        for (int j = c; j < c + w; ++j)
            grid[i][j] = id;
}

// grab an unused name from the name list
string getRandomName(mt19937& rng, vector<string>& namePool, unordered_set<string>& used) {
    shuffle(namePool.begin(), namePool.end(), rng);
    for (const string& name : namePool) {
        if (!used.count(name)) {
            used.insert(name);
            return name;
        }
    }
    return "Unnamed";
}

// main region generation logic
pair<vector<vector<int>>, unordered_map<int, string>> makeRegion(int rows, int cols) {
    auto grid = createGrid(rows, cols);
    unordered_map<int, string> regionNames;
    unordered_set<string> usedNames;

    int maxSize = max(1, min(rows, cols) / 2);
    mt19937 rng(time(nullptr));

    uniform_int_distribution<int> randSize(1, maxSize);
    uniform_int_distribution<int> randRow(0, rows - 1);
    uniform_int_distribution<int> randCol(0, cols - 1);

    vector<string> namePool = {
        "zeta", "gamma", "helix", "nova", "crateris", "ironforge", "aether", "voidland",
        "delta", "xyron", "ashreach", "morrock", "kronos", "eidolon", "frostfell",
        "brimspire", "ebonreach", "cindergate"
    };

    int regionId = 1, tries = 500;
    while (tries--) {
        int r = randRow(rng), c = randCol(rng);
        int h = randSize(rng), w = randSize(rng);

        if (isAreaFree(grid, r, c, h, w)) {
            fillRegion(grid, r, c, h, w, regionId);
            regionNames[regionId] = getRandomName(rng, namePool, usedNames);
            regionId++;
        }
    }

    return {grid, regionNames};
}

// prints metadata for each region
void printRegionInfo(const vector<vector<int>>& grid, const unordered_map<int, string>& names) {
    struct Bounds { int minR = INT_MAX, minC = INT_MAX, maxR = INT_MIN, maxC = INT_MIN; };
    unordered_map<int, Bounds> regions;

    for (int r = 0; r < grid.size(); ++r)
        for (int c = 0; c < grid[0].size(); ++c)
            if (int id = grid[r][c]; id != 0) {
                auto& b = regions[id];
                b.minR = min(b.minR, r); b.minC = min(b.minC, c);
                b.maxR = max(b.maxR, r); b.maxC = max(b.maxC, c);
            }

    cout << "\n=== REGION LIST ===\n";
    for (auto& [id, b] : regions) {
        int h = b.maxR - b.minR + 1;
        int w = b.maxC - b.minC + 1;
        cout << "Region [" << id << "] [" << names.at(id) << "] at (" << b.minR << "," << b.minC
             << ") size: " << h << "x" << w << "\n";
    }
    cout << "Total regions: " << regions.size() << "\n";
}

// ascii map printing, with colors
void printMap(const vector<vector<int>>& grid) {
    cout << "\n=== MAP VIEW ===\n";
    vector<string> colors = {
        "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m",
        "\033[91m", "\033[92m", "\033[93m", "\033[94m", "\033[95m", "\033[96m"
    };

    unordered_map<int, string> regionColors;
    for (auto& row : grid) {
        for (int cell : row) {
            if (cell == 0) {
                cout << "\033[90m" << setw(3) << '.' << "\033[0m";
            } else {
                if (!regionColors.count(cell))
                    regionColors[cell] = colors[cell % colors.size()];
                cout << regionColors[cell] << setw(3) << cell << "\033[0m";
            }
        }
        cout << "\n";
    }
}


bool mapExists(const string& path) {
    ifstream fin(path);
    return fin.good();  
}

bool askUserSatisfied(){
	string ans;
	cout << " are you satisfied with the map ? (y/n) " << endl;
	cin >> ans;
	return ans == "y" || ans == "Y" ;
}

bool saveMap(const string& path,const vector<vector<int>>& grid, const unordered_map<int,string>& regionNames){
	ofstream fout(path);
	if (!fout) return false;

	int rows = grid.size();
	int cols = grid[0].size();
	fout << rows << " " << cols << "\n";

	for (const auto& row : grid ){
		for (int cell : row ){
			fout << cell << " ";
		}
		fout << "\n";
	}

	for (const auto& [id,name] : regionNames ){
		fout << id << ":" << name << "\n";
	}

	fout.close();
	return true;
}



bool loadMap(const string& path, int& rows, int& cols, vector<vector<int>>& grid, vector<Cell>& cells) {
    ifstream fin(path);
    if (!fin) {
        cerr << "Error: Could not open map file: " << path << endl;
        return false;
    }

    fin >> rows >> cols;
    if (fin.fail() || rows <= 0 || cols <= 0) {
        cerr << "Error: Invalid or corrupt map dimensions in " << path << endl;
        return false;
    }

    grid.assign(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fin >> grid[i][j];
            if (fin.fail()) {
                cerr << "Error: Corrupt map data at row " << i << ", col " << j << endl;
                return false;
            }
        }
    }

    unordered_map<int, string> regionMap;
    string line;
    // Use a loop that also checks for stream errors
    while (getline(fin, line)) {
        if (line.empty()) continue;

        size_t pos = line.find(":");
        if (pos == string::npos) {
            cerr << "Warning: Malformed region line, skipping: " << line << endl;
            continue;
        }

        try {
            int id = stoi(line.substr(0, pos));
            string name = line.substr(pos + 1);
            regionMap[id] = name;
        } catch (const std::exception& e) {
            cerr << "Warning: Could not parse region line, skipping: " << line << endl;
        }
    }

    cells.clear();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int id = grid[i][j];
            string name = regionMap.count(id) ? regionMap[id] : "Unknown";

            cells.push_back({i, j, id, name, 0, 0}); // Init resources to 0
        }
    }

    return true;
}

struct derivedUnit {
    int id;  // unique id
    string name;
    int health;
    int damage;
    unordered_map<int, int> primaryResourcesNeeded;  // primary res id → qty
    unordered_map<int, int> derivedUnitsNeeded;
    int buildTime;
};

void distributeResource(vector<Cell>& cells) {
    string resource_path = "data/resources.txt";
    if (!mapExists(resource_path)){

    unordered_map<int, string> primaryResources = {
        {1, "oil"},         
        {2, "metal"},       
        {3, "gold"},        
        {4, "rare_earth"},  
        {5, "uranium"}      
    };

    // --- Resource Distribution Logic ---
    cout << "\n=== DISTRIBUTING RESOURCES RANDOMLY AND FAIRLY ===\n";

    // 1. Group cells by their region ID for easier processing.
    unordered_map<int, vector<Cell*>> cellsByRegion;
    for (Cell& cell : cells) {
        if (cell.regionID != 0) {
            cellsByRegion[cell.regionID].push_back(&cell);
        }
    }

    // 2. Set up our random number generators.
    mt19937 rng(time(nullptr));
    uniform_int_distribution<int> randProduction(1, 20);

    // 3. Go through each region and assign its unique resources.
    for (auto const& [regionID, regionCells] : cellsByRegion) {
        // Create a list of all possible primary resource IDs.
        vector<int> availableResources;
        for (auto const& [resId, resName] : primaryResources) {
            availableResources.push_back(resId);
        }

        // Shuffle the resources to make the assignment random.
        shuffle(availableResources.begin(), availableResources.end(), rng);

        // Deal one unique resource to each cell, until we run out of cells or resources.
        for (size_t i = 0; i < regionCells.size() && i < availableResources.size(); ++i) {
            Cell* cellToUpdate = regionCells[i];
            cellToUpdate->resourceID = availableResources[i];
            cellToUpdate->productionRate = randProduction(rng);
        }
    }

    // 4. Save the final resource map to a file.
    string resource_path = "data/resources.txt";
    ofstream fout(resource_path);
    if (fout) {
        // A header makes the file easier to understand later.
        fout << "# row col resource_id production_rate\n";
        for (const Cell& cell : cells) {
            if (cell.resourceID != 0) {
                fout << cell.row << " " << cell.col << " " << cell.resourceID << " " << cell.productionRate << "\n";
            }
        }
        fout.close();
        cout << "Resource map saved to " << resource_path << "\n";
    } else {
        cerr << "Error: Could not open " << resource_path << " for writing.\n";
    }


    // All derived units based on your specifications
    vector<derivedUnit> allUnits = {
        // Basic infrastructure
        {101, "outpost", 500, 0, {{1, 10}, {2, 2}}, {}, 1},
        
        // Infantry units
        {102, "mercenaries", 100, 10, {{1, 2}}, {{101, 1}}, 3},
        {103, "police", 120, 15, {{2, 5}}, {{101, 1}}, 4},
        {104, "light_army_personnel", 150, 20, {{1, 5}, {2, 5}}, {{101, 1}}, 5},
        {105, "medium_army_personnel", 200, 30, {{1, 8}, {2, 8}}, {{101, 1}}, 5},
        {106, "large_army_personnel", 280, 45, {{1, 10}, {2, 10}, {3, 1}}, {{101, 1}}, 8},
        
        // Vehicle infrastructure and units
        {107, "garage", 400, 0, {{1, 12}, {2, 15}}, {{101, 1}}, 2},
        {108, "pickup_truck", 80, 12, {{1, 20}, {2, 12}}, {{107, 1}}, 3},
        {109, "light_tank", 300, 60, {{1, 30}, {2, 20}}, {{107, 1}, {108, 10}}, 2},
        {110, "heavy_tank", 500, 100, {{1, 45}, {2, 50}}, {{107, 1}, {108, 25}}, 5},
        
        // Air defense and aviation
        {111, "anti_air", 250, 40, {{1, 20}, {2, 10}}, {{107, 1}, {109, 1}, {104, 10}}, 4},
        {112, "airbase", 800, 0, {{1, 50}, {2, 50}, {3, 3}}, {{107, 1}, {111, 1}}, 8},
        {113, "a2a_plane", 200, 80, {{1, 60}, {2, 60}}, {{112, 1}}, 5},
        {114, "small_bomber_plane", 250, 120, {{1, 70}, {2, 60}}, {{112, 1}}, 5},
        {115, "super_bomber_plane", 400, 200, {{1, 100}, {2, 80}, {3, 7}}, {{112, 1}, {114, 1}}, 7},
        
        // Missiles and WMDs
        {116, "rocket", 100, 50, {{1, 20}, {2, 10}, {3, 1}}, {{112, 1}}, 2},
        {117, "missile", 150, 100, {{1, 40}, {2, 25}, {4, 10}}, {{112, 1}, {116, 1}}, 10},
        {118, "nuke", 300, 2000, {{1, 1000}, {2, 999}, {4, 100}, {5, 500}}, {{112, 1}, {115, 2}, {106, 10}, {117, 10}}, 34},
        
        // Extended units
        {119, "artillery_battery", 350, 90, {{1, 60}, {2, 70}, {3, 5}}, {{107, 1}, {105, 10}}, 6},
        {120, "mobile_sam", 280, 45, {{1, 50}, {2, 40}, {3, 2}}, {{107, 1}, {109, 5}}, 5},
        {121, "commando_squad", 80, 35, {{1, 20}, {2, 30}, {3, 3}}, {{101, 1}}, 4},
        {122, "drone_swarm", 60, 25, {{1, 40}, {2, 25}, {3, 2}}, {{112, 1}}, 3},
        {123, "heavy_mech", 700, 150, {{1, 80}, {2, 120}, {3, 10}}, {{107, 1}, {110, 3}}, 9},
        {124, "fortified_bunker", 600, 10, {{1, 20}, {2, 50}}, {{101, 1}}, 4},
        {125, "mobile_hq", 450, 5, {{1, 90}, {2, 80}, {3, 5}}, {{101, 1}, {107, 1}}, 7},
        {126, "orbital_strike_sat", 200, 300, {{1, 200}, {2, 150}, {3, 10}, {4, 50}}, {{112, 1}}, 12},
        {127, "chemical_warhead", 120, 150, {{1, 80}, {2, 50}, {4, 20}}, {{112, 1}, {116, 2}}, 8},
        {128, "emp_generator", 300, 0, {{1, 60}, {2, 70}, {4, 15}}, {{112, 1}, {111, 1}}, 9},
        {129, "armored_truck", 150, 20, {{1, 40}, {2, 45}}, {{107, 1}, {108, 5}}, 3},
        {130, "railgun_platform", 400, 250, {{1, 150}, {2, 200}, {3, 15}}, {{107, 1}, {112, 1}}, 14},
        {131, "stealth_bomber", 280, 140, {{1, 120}, {2, 100}, {3, 5}}, {{112, 1}, {114, 1}}, 7},
        {132, "shock_trooper_unit", 180, 55, {{1, 25}, {2, 35}, {3, 5}}, {{101, 1}, {121, 1}}, 5}
    };

    // Create a lookup map for unit IDs to names for efficient access.
    unordered_map<int, string> unitIdToName;
    for (const auto& unit : allUnits) {
        unitIdToName[unit.id] = unit.name;
    }

    // Print all derived units
    cout << "\n=== ALL DERIVED UNITS ===" << endl;
    for (const auto& unit : allUnits) {
        cout << "\nID: " << unit.id << " - " << unit.name << endl;
        cout << "Health: " << unit.health << " | Damage: " << unit.damage << " | Build Time: " << unit.buildTime << " turns" << endl;
        
        cout << "Resources: ";
        bool first = true;
        for (const auto& [resId, qty] : unit.primaryResourcesNeeded) {
            if (!first) cout << ", ";
            cout << primaryResources.at(resId) << "(" << qty << ")";
            first = false;
        }
        
        
        if (!unit.derivedUnitsNeeded.empty()) {
        // if there are prerequisites, print label first
            cout << " | Prerequisites: ";

            bool first = true; // used to control comma formatting
            for (const auto& [reqId, quantity] : unit.derivedUnitsNeeded) {
                if (!first) {
                    cout << ", ";
                }

                // Use the efficient lookup map.
                if (unitIdToName.count(reqId)) {
                    cout << unitIdToName.at(reqId) << "(" << quantity << ")";
                } else {
                    // Fallback in case unit is not found (defensive)
                    cout << "UnknownUnit(" << quantity << ")";
                }

            // after first element is printed, set flag so next iterations add comma
                first = false;
            }
        }
            cout << endl;
    }
    }
    cout << " resources have already been distributed \n\n\n\n\n\n\n";
}

int askUserForRegion(const unordered_map<int,string>& regionNames){
	std::cout << "\n=== SELECT YOUR STARTING REGION ===\n";
	// list all regions
	for (const auto& [id,name] : regionNames){
		cout << "[" << id << "] " << name << "\n";
	}
	int choice;
	std::cout << "\nEnter region ID: \n";
	cin >> choice;

	if (regionNames.count(choice)) {
		return choice;
	}
	else {
		cerr << "Invalid region ID!\n";
		return -1;
	}
}

bool saveProfile(const string& path,int regionID, const string& regionName){
	ofstream fout(path);
	if (!fout) return false;

	fout << "starting_region_id: " << regionID << "\n";
	fout << "starting_region_name: " << regionName << endl;
	fout << "turns: 0"<<endl;

	fout.close();
	return true;
}

void gameStart(const string& profile_path){
	ifstream fin(profile_path);
	if (!fin){
		cerr << "error: could not open profile file\n";
		return;
	}

	int regionID = -1;
	string regionName;
	int turns = -1;
	string line;

	while (getline(fin,line)){
		if (line.find("starting_region_id:") != string::npos) {
			regionID = stoi(line.substr(line.find(":") + 1));
		}
		else if (line.find("starting_region_name:") != string::npos){
			regionName = line.substr(line.find(":") + 1);
		}
		else if (line.find("turns:") != string::npos){
			turns = stoi(line.substr(line.find(":") + 1));
		}
	}
	
	if (regionID != -1 && !regionName.empty() && turns != -1){
		std::cout << "\n=== WELCOME TO THE GAME ===\n";
		std::cout << "Your starting region ID: " << regionID << "\n";
		std::cout << "Your starting region name: " <<  regionName << "\n";
		std::cout << " Current turn: " << turns << "\n\n";
	}
	else{
		cerr << "Error : corrupted profile fata \n";
	}
}

int main() {
    std::filesystem::create_directories("data");
    string map_path = "data/map.txt";
    string profile_path = "data/profile.txt";
    if (!mapExists(profile_path)){
            if (!mapExists(map_path)) {
            cout << "no map found... entering creation loop\n";
            int rows, cols;
            cout << "map size? (rows cols): ";
            cin >> rows >> cols;

            while (true) {
                auto [grid, regionNames] = makeRegion(rows, cols);
                printRegionInfo(grid, regionNames);
                printMap(grid);

                if (askUserSatisfied()) {
                    if (saveMap(map_path, grid, regionNames)) {
                        cout << "map saved to " << map_path << "\n";
                        break;
                    } else {
                        cerr << "failed to save map!\n";
                    }
                }
                cout << "\n--- remaking map ---\n";
            }
        } else {
            int rows = 0, cols = 0;
            vector<vector<int>> grid;
            vector<Cell> cells;

            cout << "map found... loading\n";
            if (loadMap(map_path, rows, cols, grid, cells)) {
                cout << "Map loaded successfully.\n";
                printMap(grid);
                distributeResource(cells);
                unordered_map<int, string> regionNames;
                for (const Cell& cell : cells){
                if (cell.regionID != 0 && !regionNames.count(cell.regionID)) {
                    regionNames[cell.regionID] = cell.regionName;	
                }
                }

                int chosenRegion = askUserForRegion(regionNames);
                if (chosenRegion != -1 ){
                string profilePath = "data/profile.txt";
                if (saveProfile(profilePath, chosenRegion, regionNames.at(chosenRegion))){
                    std::cout << " Profile saved to " << profilePath << "\n";
                }
                else{
                    cerr << "Failed to save profile \n" << endl;
                }
                    }
            } else {
                cerr << "Failed to load map. Please check or delete the map file and restart.\n";
                return 1; // Exit with an error code
            }
        }
    }
    else {
       gameStart(profile_path); 
    }



    return 0;
}
