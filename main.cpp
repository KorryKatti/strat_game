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



void loadMap(const string& path, int& rows, int& cols, vector<vector<int>>& grid, vector<Cell>& cells) {
    cout << " ====== NOW LOADING =====" << endl;
    cout << " ||||| " << path << " ||||| " << endl;

    ifstream fin(path);
    if (!fin) {
        cerr << " failed to open file " << endl;
        return;
    }

    fin >> rows >> cols;
    if (fin.fail()) {
        cerr << " failed to read data, possible corruption " << endl;
        return;
    }
    if (rows <= 0 || cols <= 0) {
        cerr << " bad dimensions , delete the map file at " << path << " and restart " << endl;
        return;
    }

    grid.assign(rows, vector<int>(cols));  // ← use passed-in grid

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fin >> grid[i][j];
            cout << " found " << grid[i][j] << " at " << i << " " << j << endl;
        }
    }

    cout << " regions saved with their location , now printing map " << endl;
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    cout << " ======================================================= " << endl;
    cout << "==================== WORLD MAP ===================== " << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
    cout << " initializing regionId[s] and name[s] " << endl;

    unordered_map<int, string> regionMap;

    string line;
    getline(fin, line); // skip leftover newline

    while (getline(fin, line)) {
        if (line.empty()) continue;

        size_t pos = line.find(":");
        if (pos == string::npos) {
            cout << " bad format found " << endl;
            continue;
        }

        cout << line << endl;

        int id = stoi(line.substr(0, pos));
        string name = line.substr(pos + 1);

        regionMap[id] = name;
        cout << name << endl;
    }

    cells.clear(); // ← clear passed-in cells

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int id = grid[i][j];
            string name = regionMap.count(id) ? regionMap[id] : "Unknown";

            cells.push_back({i, j, id, name});
        }
    }
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
    
    unordered_map<int, string> primaryResources = {
        {1, "oil"},         
        {2, "metal"},       
        {3, "gold"},        
        {4, "rare_earth"},  
        {5, "uranium"}      
    };

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
            cout << " | Prerequisites: ";
            first = true;
            for (const auto& [reqId, qty] : unit.derivedUnitsNeeded) {
                if (!first) cout << ", ";
                // Find the unit name by ID
                auto it = find_if(allUnits.begin(), allUnits.end(), 
                                [reqId](const derivedUnit& u) { return u.id == reqId; });
                if (it != allUnits.end()) {
                    cout << it->name << "(" << qty << ")";
                }
                first = false;
            }
        }
        cout << endl;
    }
}



int main() {
    std::filesystem::create_directories("data");
    string map_path = "data/map.txt";

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
        loadMap(map_path, rows, cols, grid, cells);

        distributeResource(cells);
    }

    return 0;
}

