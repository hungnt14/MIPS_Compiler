#include <bits/stdc++.h>

using namespace std;

vector<string> input;
map<string, int> reg;
map<string, int> opcode;

void init() {
	opcode["add"] = 32;
	opcode["and"] = 36;
	opcode["sub"] = 34;
	opcode["nor"] = 39;
	opcode["or"] = 37;
	opcode["slt"] = 42;

	opcode["addi"] = (8 << 26);
	opcode["lw"] = (35 << 26);
	opcode["sw"] = (43 << 26);
	opcode["beq"] = (4 << 26);
	opcode["bne"] = (5 << 26);

	opcode["j"] = (2 << 26);

	reg["$zero"] = 0;
	reg["$at"] = 1;
	reg["$v0"] = 2;
	reg["$v1"] = 3;
	reg["$a0"] = 4;
	reg["$a1"] = 5;
	reg["$a2"] = 6;
	reg["$a3"] = 7;
	reg["$t0"] = 8;
	reg["$t1"] = 9;
	reg["$t2"] = 10;
	reg["$t3"] = 11;
	reg["$t4"] = 12;
	reg["$t5"] = 13;
	reg["$t6"] = 14;
	reg["$t7"] = 15;
	reg["$t8"] = 16;
	reg["$t9"] = 17;
	reg["$s0"] = 18;
	reg["$s1"] = 19;
	reg["$s2"] = 20;
	reg["$s3"] = 21;
	reg["$s4"] = 22;
	reg["$s5"] = 23;
	reg["$s6"] = 24;
	reg["$s7"] = 25;
	reg["$k0"] = 26;
	reg["$k1"] = 27;
	reg["$gp"] = 28;
	reg["$sp"] = 29;
	reg["$fp"] = 30;
	reg["$ra"] = 31;
}

void DeleleComment(vector<string> &input) {
    /// Arguments: lines of input
    /// Output: In a clear form of command
}

void CalculateImmediate(vector<string> input, vector<int> &label) {
    /// Arguments: lines of input (clear form)
    /// Output: list of calculated label
}

void BuildLabelTable(vector<string> &input, vector<int> label) {
    /// Arguments: lines of input (clear form); label list
    /// Output: input with no label, also export label table into temp file. ("temp.txt")
}

int GenerateBinary(string cmd) {
    /// Arguments: a string of command
    /// Output: binary code
}

void FirstPass() {
    DeleleComment(input);
    vector<int> calculatedLabel;
    CalculateImmediate(input, calculatedLabel);
    BuildLabelTable(input, calculatedLabel);
}

void SecondPass() {
    for (int i = 0; i < input.size(); i++) {
        int binary = GenerateBinary(input[i]);
        /// do something here
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    string line;
    while (cin >> line) {
        input.push_back(line);
    }
    init();
    FirstPass();
    SecondPass();
}
//cbcbcbc
