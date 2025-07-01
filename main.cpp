// the strategy game
// making it now
// i need a job dude fr
#include <bits/stdc++.h>
using namespace std;



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

void loadMap(const string& path, int& rows,int& cols){
	cout << " ====== NOW LOADING =====" << endl;
	cout << " ||||| " << path << " ||||| " << endl;

	ifstream fin(path);
	if (!fin){
		cerr << " failed to open file " << endl;
		return;
	}
	fin >> rows >> cols;
	if (fin.fail()){
		cerr << " failed to read data, possible corruption " << endl;
		return;
	}
	if (rows<=0 || cols <=0){
		cerr << " bad dimensions , delete the map file at " << path << " and restart " << endl;
	}
	
	vector<vector<int>> grid(rows, vector<int>(cols));

	for (int i = 0; i < rows; i++) {
	    for (int j = 0; j < cols; j++) {
	        fin >> grid[i][j];
	    }
	}

	for (int i = 0; i < rows; i++) {
	    for (int j = 0; j < cols; j++) {
	        cout << grid[i][j] << " ";
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
	int rows = 0,cols=0;
        cout << "map found... loading\n";
        loadMap(map_path,rows,cols);
    }

    return 0;
}

