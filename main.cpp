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

    /*
derived units table [ format: { id thing primaryresources derivedunit_ifneeded turnsneeded } ]

101. outpost ( 10 oil , 2 metal ) 1                          // forward base to deploy troops, basic requirement for ground stuff
102. 10 mercenaries ( 2 oil , 1 outpost ) 3                 // cheap, fast disposable troops
103. 10 police ( 5 metal , 1 outpost ) 4                    // basic defensive garrison
104. 10 light_army_personnel ( 5 oil , 5 metal , 1 outpost ) 5  // light soldiers for offense or defense
105. 10 medium_army_personnel ( 8 oil , 8 metal , 1 outpost ) 5 // tougher infantry
106. 10 large_army_personnel ( 10 oil , 10 metal , 1 gold , 1 outpost ) 8 // heavy infantry squads
107. garage ( 12 oil , 15 metal , 1 outpost ) 2             // unlocks vehicle production
108. 5 pickup_truck ( 20 oil , 12 metal , 1 garage ) 3      // light, fast support vehicles
109. 3 light_tank ( 30 oil , 20 metal , 10 pickup_truck + 1 garage ) 2 // basic armored vehicle
110. 3 heavy_tank ( 45 oil , 50 metal , 25 pickup_truck + 1 garage ) 5 // big boi tank
111. 1 anti_air ( 20 oil , 10 metal , 1 light_tank + 10 light_army_personnel + 1 garage ) 4 // air defense
112. 1 airbase ( 50 oil , 50 metal , 3 gold , 1 garage + 1 anti_air ) 8 // unlocks aircraft
113. 1 a2a_plane ( 60 oil , 60 metal , 1 airbase ) 5        // fighter jet for air superiority
114. 1 small_bomber_plane ( 70 oil , 60 metal , 1 airbase ) 5 // tactical bomber
115. 1 super_bomber_plane ( 100 oil , 80 metal , 7 gold , 1 small_bomber_plane + 1 airbase ) 7 // big strategic bomber
116. 1 rocket ( 20 oil , 10 metal , 1 gold , 1 airbase ) 2  // basic ballistic missile
117. 1 missile ( 40 oil , 25 metal , 10 rare_earth , 1 rocket + 1 airbase ) 10 // upgraded missile
118. 1 nuke ( 1000 oil , 999 metal , 100 rare_earth , 500 uranium , 10 missile + 10 large_army_personnel + 1 airbase + 2 super_bomber_plane ) 34 // final wmd

// new extra units

119. artillery_battery ( 60 oil , 70 metal , 5 gold , 1 garage + 10 medium_army_personnel ) 6
    // long-range ground bombardment unit

120. mobile_sam ( 50 oil , 40 metal , 2 gold , 5 light_tank + 1 garage ) 5
    // mobile anti-air defense, can move with ground forces

121. commando_squad ( 20 oil , 30 metal , 3 gold , 1 outpost ) 4
    // stealthy, specialized infiltration or sabotage missions

122. drone_swarm ( 40 oil , 25 metal , 2 gold , 1 airbase ) 3
    // cheap, fast harassment or recon attack units

123. heavy_mech ( 80 oil , 120 metal , 10 gold , 3 heavy_tank + 1 garage ) 9
    // giant armored walker-type ground dominance unit

124. fortified_bunker ( 50 metal , 20 oil , 1 outpost ) 4
    // strong static defense, high garrison capacity

125. mobile_hq ( 90 oil , 80 metal , 5 gold , 1 outpost + 1 garage ) 7
    // deployable base, allows forward production and command

126. orbital_strike_sat ( 200 oil , 150 metal , 50 rare_earth , 10 gold , 1 airbase ) 12
    // global strike from orbit, very high cost, big payoff

127. chemical_warhead ( 80 oil , 50 metal , 20 rare_earth , 2 rocket + 1 airbase ) 8
    // area denial or mass infantry damage, weaker than nuke but faster

128. emp_generator ( 60 oil , 70 metal , 15 rare_earth , 1 airbase + 1 anti_air ) 9
    // disables vehicles/air units in region temporarily

129. armored_truck ( 40 oil , 45 metal , 1 garage + 5 pickup_truck ) 3
    // tougher logistics or troop transport

130. railgun_platform ( 150 oil , 200 metal , 15 gold , 1 garage + 1 airbase ) 14
    // ultra-long-range precision strike ground weapon

131. stealth_bomber ( 120 oil , 100 metal , 5 gold , 1 airbase + 1 small_bomber_plane ) 7
    // bypasses defenses, ideal for deep strikes

132. shock_trooper_unit ( 25 oil , 35 metal , 5 gold , 1 outpost + 1 commando_squad ) 5
    // elite infantry, strong vs other infantry and fortified positions
*/


    
    derivedUnit tankUnit = {
	101, 
	"tanks",
	500,
	80,
	{{1, 50}, {2, 100}},  // oil:50, metal:100
	{{102, 2}}            // 2 light vehicles needed
    };



    if (!mapExists(resource_path)){
	cout << " looks like a first run ; distributing resources " << endl;
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

