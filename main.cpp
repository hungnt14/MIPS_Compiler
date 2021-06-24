#include <bits/stdc++.h>

#define INIT_OFFSET 4194304

using namespace std;

vector<string> input;
map<string, int> reg;
map<string, pair<int, int> > opcode;

void init() {
    /// opcode: string: command name, pair<int, int>: binary value and command type: 0 = R, 1 = I, 2 = J
	/// R_TYPE
	opcode["sll"]  = make_pair(0, 0);
	opcode["srl"]  = make_pair(2, 0);
	opcode["jr"] = make_pair(8, 0);
	opcode["add"]  = make_pair(32, 0);
	opcode["addu"]  = make_pair(33, 0);
    opcode["sub"]  = make_pair(34, 0);
	opcode["subu"]  = make_pair(35, 0);
	opcode["and"]  = make_pair(36, 0);
    opcode["or"]   = make_pair(37, 0);
	opcode["nor"]  = make_pair(39, 0);
	opcode["slt"]  = make_pair(42, 0);
	opcode["sltu"]  = make_pair(43, 0);
    /// I_TYPE
    opcode["beq"]  = make_pair(4 << 26, 1);
	opcode["bne"]  = make_pair(5 << 26, 1);
	opcode["addi"] = make_pair(8 << 26, 1);
    opcode["lui"] = make_pair(15 << 26, 1);
	opcode["lw"]   = make_pair(35 << 26, 1);
	opcode["lbu"]   = make_pair(36 << 26, 1);
	opcode["lhu"]   = make_pair(37 << 26, 1);
	opcode["sw"]   = make_pair(43 << 26, 1);
    /// J_TYPE
	opcode["j"]    = make_pair(2 << 26, 2);
    opcode["jal"]  = make_pair(3 << 26, 2);
    string reg_list[32] = {"$zero", "$at", "$v0", "$v1",
                        "$a0", "$a1", "$a2", "$a3",
                        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
                        "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
                        "$t8", "$t9",
                        "$k0", "$k1",
                        "$gp", "$sp", "$fp", "$ra"};

    for (int i = 0; i < 32; i++)
        reg[reg_list[i]] = i;
}

unsigned int Complement(unsigned int n) {
   int number_of_bits = floor(log2(n))+1;
   /// XOR the given integer with poe(2,
   /// number_of_bits-1 and print the result
   return ((1 << number_of_bits) - 1) ^ (unsigned int)n;
}

string binaryConverter(unsigned int num) {
	unsigned int bin;
	string s = "";
	unsigned int i = 0;
	while (num > 0)
	{
		i++;
		bin = num % 2;
		if (bin) {
			s += "1";
		}
		else {
			s += "0";
		}
		num /= 2;
	}
	for (unsigned int j = 32 - i; j > 0; j--) {
		s += "0";
	}
	reverse(s.begin(), s.end());
	return s;
}

struct Commands {
    /// Example call:
    /// Commands command;
    /// int value = command.R_Type(0, 1, 2, 3, 0, 0);
    unsigned int R_Type(string str) {
        /// add rd rs rt
        regex type1("([a-z]+) ([$][a-z]*[0-9]|[$][a-z]*), ([$][a-z]*[0-9]|[$][a-z]*), ([$][a-z]+[0-9]|[$][a-z]+)");
        /// sll rd rt shamt
        regex type2("([a-z]+) ([$][a-z]*[0-9]|[$][a-z]*), ([$][a-z]*[0-9]|[$][a-z]*), ([0-9]+)");
        /// jr rs
        regex type3("([a-z]+) ([$][a-z]*[0-9]|[$][a-z]*)");
        smatch matches[3];
        regex_search(str, matches[0], type1);
        regex_search(str, matches[1], type2);
        regex_search(str, matches[2], type3);
        unsigned int binNum = 0;
        if (!matches[0].empty()) {
            binNum = opcode[matches[0].str(1)].first; /// opcode
            binNum += (reg[matches[0].str(2)] << 11); /// rd
            binNum += (reg[matches[0].str(3)] << 21); /// rs
            binNum += (reg[matches[0].str(4)] << 16); /// rt
        }
        else if (!matches[1].empty()){
            binNum = opcode[matches[1].str(1)].first; /// opcode
            binNum += (reg[matches[1].str(2)] << 11); /// rd
            binNum += (reg[matches[1].str(3)] << 16); /// rt
            stringstream val(matches[1].str(4)); /// shamt
            unsigned int num = 0;
            val >> num;
            binNum += (num << 6);
        }
        else if (!matches[2].empty()){
            binNum = opcode[matches[2].str(1)].first; /// opcode
            binNum += (reg[matches[2].str(2)] << 21); /// rs
        }
        return binNum;
    }
    unsigned int I_Type(string str, map<string, int> labelAddress) {
        /// addi rt, rs, immediate; beq rs, rt, label
        regex type1("([a-z]+) {1,10}([$][a-z]+[0-9]*|[$][a-z]+), {1,10}([$][a-z]+[0-9]*|[$][a-z]+), {1,10}(.+)?");
        /// lw $t1, 4($t2)
        regex type2("([a-z]+) {1,10}([$][a-z]+[0-9]*|[$][a-z]+), {1,10}([-]?[0-9]+) {0,10}[(]([$][a-z]+[0-9]*|[$][a-z]+)[)]");
        /// lui rt immediate
        regex type3("([a-z]+) {1,10}([$][a-z]+[0-9]*|[$][a-z]+), {1,10}([0-9]+)");
        vector<smatch> matches(3);
        regex_search(str, matches[0], type1);
        regex_search(str, matches[1], type2);
        regex_search(str, matches[2], type3);

        unsigned int binNum = 0;
        if (!matches[0].empty()) {
            binNum = opcode[matches[0].str(1)].first; /// opcode
            if (labelAddress.find(matches[0].str(4)) != labelAddress.end()) {
                /// check if label
                binNum += (reg[matches[0].str(2)] << 21); /// rs
                binNum += (reg[matches[0].str(3)] << 16); /// rt
                int imm = (labelAddress[matches[0].str(4)] - labelAddress[str] - 4) >> 2; /// label (offset = (addr - currentPC - 4) / 4
                imm %= 2 << 15;
                binNum += imm;
            }
            else {
                /// if immediate
                binNum += (reg[matches[0].str(2)] << 16); /// rt
                binNum += (reg[matches[0].str(3)] << 21); /// rs
                stringstream val(matches[0].str(4)); /// immediate
                unsigned int num = 0;
                val >> num;
                if (num < 0) num = Complement(num);
                num %= 2 << 15;
                binNum += num;
            }
        }
        else if (!matches[1].empty()) {
            binNum = opcode[matches[1].str(1)].first; /// opcode
            binNum += (reg[matches[1].str(4)] << 21); /// rs
            binNum += (reg[matches[1].str(2)] << 16);
            stringstream val(matches[1].str(3));
            unsigned int num = 0;
            val >> num;
            if (num < 0) num = Complement(num);
            num %= 2 << 15;
            binNum += num;
        }
        else if (!matches[2].empty()){
            binNum = opcode[matches[2].str(1)].first; /// opcode
            binNum += (reg[matches[2].str(2)] << 16); /// rt
            stringstream val(matches[2].str(3)); /// immediate
            unsigned int num = 0;
            val >> num;
            if (num < 0) num = Complement(num);
            num %= 2<<15;
            binNum += num;
        }
        return binNum;
    }
    int J_Type(string str, map<string, int> labelAddress) {
        regex type1("([j]{1}|[jal]{3}) {1,10}([a-zA-Z]{1}[a-zA-Z0-9$._]*)");
        smatch matches;
        unsigned int binNum = 0;
        regex_search(str, matches, type1);
        if (!matches.empty()) {
            binNum = opcode[matches.str(1)].first; /// opcode
            int address = labelAddress[matches.str(2)] % (2<<28); /// label address
            address >>= 2;
            binNum += address;
        }
        return binNum;
    }
} c;

void DeleteComment(vector<string> &input) {
    /// Arguments: lines of input
    /// Output: In a clear form of command
    vector<string> clearInput;

    for (string line: input) {
        if (line == "") continue;
        for (int i = 0; i < line.size() - 1; i++) {
            if (line[i] == '#') {
                line.erase(i, line.size() - i);
                break;
            }
            else if (!isspace(line[i]) && isspace(line[i + 1])) {
                /// have a form: "abcxyz "
                line[i + 1] = '*';
            }
        }
        remove_if(line.begin(), line.end(), [](char c){return isspace(static_cast<unsigned char>(c));});
        replace(line.begin(), line.end(), '*', ' ');
        while (line[0] == ' ' && line.size() != 0)
            line.erase(0);
        while (line[line.size() - 1] == ' ' && line.size() != 0)
            line.erase(line.size() - 1);
        if (line != "") {
            clearInput.push_back(line);
        }
    }
    input = clearInput;
}

void CalculateImmediate(vector<string> input, map<string, int> &label) {
    /// Arguments: lines of input (clear form)
    /// Output: list of calculated label
	int offset = INIT_OFFSET - 4; /// 0x00400000 - 4
	queue<string> labelQueue; /// contain continuosly labels (there aren't and instruction between them)
	for (string line: input) {
		size_t findLabel = line.find(":"); /// if there is a ':' in line, there is a label before it
        if (findLabel != string::npos) { /// if found label
			labelQueue.push(line.substr(0, findLabel)); /// findLabel is also size of label
		}
		if (findLabel == string::npos || findLabel + 1 < line.size()) { /// if label is not found or label is found but there is an instruction after that
 			offset += 4;                                                /// then all labels in queue have the same address at current offset
			while (!labelQueue.empty()) {
				string currentLabel = labelQueue.front();
				labelQueue.pop();
				label[currentLabel] = offset;
            }
		}
	}
	offset += 4;
	while (!labelQueue.empty()) {
        string currentLabel = labelQueue.front();
        labelQueue.pop();
        label[currentLabel] = offset;
    }
}

void BuildLabelTable(vector<string> &input, map<string, int> label) {
    /// Arguments: lines of input (clear form); label list
    /// Output: input with no label, also export label table into temp file. ("labelAddress.txt")

    /// clear label in input
    vector<string> clearInput;
    for (string line: input) {
        if (line == "") continue;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ':') {
                line.erase(line.begin(), line.begin() + i + 1);
                while (isspace(line[0])) {
                    line.erase(line.begin());
                }
                break;
            }
        }
        if (line != "")
            clearInput.push_back(line);
    }
    input = clearInput;
    /// export label table to file
    /// line format: <label name> <address value>
    fstream ofs;
	ofs.open("labelAddress.txt", ios::out | ios::trunc);
    for (auto lb: label) {
        ofs << lb.first << " " << lb.second << endl;
    }
    ofs.close();
}

int GenerateBinary(string cmd, map<string, int> labelAddress) {
    /// Arguments: a string of command
    /// Output: binary code
    string classifier = cmd.substr(0, cmd.find(' '));
    unsigned int binNum = 0;
    if (opcode[classifier].second == 0) {
        binNum = c.R_Type(cmd);
    }
    else if (opcode[classifier].second == 1) {
        binNum = c.I_Type(cmd, labelAddress);
    }
    else {
        binNum = c.J_Type(cmd, labelAddress);
    }
    return (binNum);
}

void FirstPass() {
    DeleteComment(input);
    map<string, int> labelAddress;
    CalculateImmediate(input, labelAddress);
    BuildLabelTable(input, labelAddress);
    fstream ofs;
	ofs.open("labelAddress.txt");
    for (auto lb: labelAddress) {
        ofs << lb.first << " " << lb.second << endl;
    }
    ofs.close();
}

void SecondPass() {
    map<string, int> labelAddress;
    fstream ifs;
    ifs.open("labelAddress.txt");
    string label_name; int label_add;
    while (ifs >> label_name >> label_add) {
        labelAddress[label_name] = label_add;
    }
    ifs.close();

    fstream ofs;
    ofs.open("output.txt");
    for (string line: input) {
    	cout << line << endl;
        cout << "> " << "0x" << setfill('0') << setw(8) << right << hex << GenerateBinary(line, labelAddress) << endl;
        ofs << binaryConverter(GenerateBinary(line, labelAddress)) << endl;
    }
    ofs.close();
}

int main() {
    freopen("input.txt", "r", stdin);
    string line;
    while (getline(cin, line)) {
        input.push_back(line);
    }
    init();
    FirstPass();
    SecondPass();
}
