#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
#include <algorithm>
#include <string>
#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <bitset>
#include <climits>
#include <cstring>
#include <limits.h>
#include <queue>
#include <iomanip>
#include <sstream>

using namespace std;
#define ndpro \
ios_base::sync_with_stdio(false);\
cin.tie(0);\
cout.tie(0);
#define all(x) (x).begin(),(x).end()
#define rall(x) (x).rbegin(),(x).rend()
#define int long long
#define ll long long
#define endl '\n'
int const mod = 1e9 + 7;
const int dx[] = { 0, 1, 0, -1 };
const int dy[] = { 1, 0, -1, 0 };


class Assembler {
private:
    vector<string> code;
    map<string, int> symbols;
    map<string, string> mri;
    map<string, string> rri;
    map<string, string> ioi;
    map<int, string> bin;
    int locationCounter = 0;

public:
    Assembler() {
        mri = {
            {"AND", "0010"},
            {"ADD", "0011"},
            {"LDA", "0100"},
            {"STA", "0101"},
            {"BUN", "0110"},
            {"BSA", "0111"},
            {"ISZ", "1000"}
        };

        rri = {
            {"CLA", "0111100000000000"},
            {"CLE", "0111010000000000"},
            {"CMA", "0111001000000000"},
            {"CME", "0111000100000000"},
            {"CIR", "0111000010000000"},
            {"CIL", "0111000001000000"},
            {"INC", "0111000000100000"},
            {"SPA", "0111000000010000"},
            {"SNA", "0111000000001000"},
            {"SZA", "0111000000000100"},
            {"SZE", "0111000000000010"},
            {"HLT", "0111000000000001"}
        };

        ioi = {
            {"INP", "1111100000000000"},
            {"OUT", "1111010000000000"},
            {"SKI", "1111001000000000"},
            {"SKO", "1111000100000000"},
            {"ION", "1111000010000000"},
            {"IOF", "1111000001000000"}
        };
    }

    void firstPass() {
        for (string line : code) {
            string label, instruction;
            istringstream iss(line);
            iss >> label;

            if (label.back() == ',') {
                label.pop_back();
                symbols[label] = locationCounter;
                iss >> instruction;
            }
            else {
                instruction = label;
            }

            if (instruction == "ORG") {
                int address;
                iss >> hex >> address;
                locationCounter = address;
            }
            else{
                locationCounter++;
            }
        }
    }

    void secondPass() {
        locationCounter = 0;
        for (string line : code) {
            string label, instruction;
            istringstream iss(line);
            iss >> label;

            if (label.back() == ',') {
                label.pop_back();
                iss >> instruction;
            }
            else {
                instruction = label;
            }

            if (instruction == "ORG") {
                int address;
                iss >> hex >> address;
                locationCounter = address;
            }
            else if (instruction == "END") {
                break;
            }
            else if (mri.find(instruction) != mri.end()) {
                string operand;
                iss >> operand;
                int address = symbols[operand];
                string binaryValue = mri[instruction] + bitset<12>(address).to_string();
                string addressingMode = "x";
                iss >> addressingMode;
                if (addressingMode == "I")binaryValue[0] = '1';
                bin[locationCounter] = binaryValue;
                locationCounter++;
            }
            else if (rri.find(instruction) != rri.end()) {
                bin[locationCounter] = rri[instruction];
                locationCounter++;
            }
            else if (ioi.find(instruction) != ioi.end()) {
                bin[locationCounter] = ioi[instruction];
                locationCounter++;
            }
            else if (instruction == "HEX" || instruction == "DEC") {
                int value;
                if (instruction == "HEX") {
                    iss >>hex >> value;
                    bin[locationCounter] = bitset<16>(value).to_string();
                }
                else if (instruction == "DEC") {
                    iss >> value;
                    bin[locationCounter] = bitset<16>(value).to_string();
                }
                locationCounter++;
            }
        }
    }

    void assemble(vector<string> assemblyCode) {
        code = assemblyCode;
        firstPass();
        secondPass();
    }

    void printBinaryOutput() {
        cout << "Symbol Table:" << endl;
        vector<pair<int, string>>symbolstable;
        for (auto it : symbols)symbolstable.push_back({ it.second,it.first });
        sort(all(symbolstable));
        for (int i = 0; i < symbolstable.size(); i++)
        {
            cout << hex<<symbolstable[i].second << ": " << symbolstable[i].first<<endl;
        }
        cout << endl;
        cout << "Machine Code:" << endl;
        vector<pair<int, string>>lines;
        for (auto it : bin)lines.push_back({ it.first,it.second });
        for (int i = 0; i < lines.size(); ++i) {
            cout << hex << lines[i].first << ": " << lines[i].second << endl;
        }
    }
};

unsigned main() {
    ndpro;
    //freopen("longpath.in", "r", stdin);
    //freopen("longpath.out", "w", stdout);
    int t = 1;
    //cin >> t;
    while (t--)
    {
        /*
        vector<string> code = {
            "ORG 100",
            "LDA SUB",
            "CMA",
            "INC",
            "ADD MIN",
            "STA DIF",
            "HLT",
            "MIN, DEC 83",
            "SUB, DEC -23",
            "DIF, HEX 0",
            "END"
        };*/
        
        vector<string>code;
        string line;
        while (getline(cin, line))
        {
            code.push_back(line);
            if (line == "END")break;
        }

        try {
            Assembler assembler;
            assembler.assemble(code);
            cout << endl;
            assembler.printBinaryOutput();
        }
        catch (const runtime_error& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
    return 0;
}