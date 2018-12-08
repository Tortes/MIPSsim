#include<string>
#include<iostream>
#include<fstream>
#include<vector>
using namespace std;


/*------------------------------------------------------------------------------*/
int REGISTERS[32];
int mipslength;
vector<int> DATA;
vector<string> mipsString;

string WaitIns;
string ExeIns;
vector<string> PreIssue(4,"");
vector<string> PreALU(2,"");
string PostALU;
vector<string> PreALUB(2,"");
string PostALUB;
vector<string> PreMEM(2,"");
string PostMEM;

/*----------------------------utils----------------------------------------------*/
int string2int(string s)
{
    return stoi(s,0,2);
}

string string2string(string s)
{
    return to_string(stoi(s, 0, 2));
}

int truevalue(string mipsString)
{
    string tmp(&mipsString[1], &mipsString[32]);
    if (mipsString[0] == '1')
    {
        for (int i = 0; i < 31;i++)
        {
            if(tmp[i]=='0')
                tmp[i] = '1';
            else
                tmp[i] = '0';
        }
        return -1 * (1 + stoi(tmp, 0, 2));
    }
        
    else
        return stoi(tmp, 0, 2);
}

int stringVectorCount(vector<string> v)
{   int count = 0;
    for(int i = 0; i < v.size(); i++)
        if(v[i].empty() == 0)
            ++count;
    return count;
}



/*----------------------INSTRUCTION DEF-----------------------------------------*/
int SLL(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    string sa(&mipsString[21], &mipsString[26]);
    if (flag == 0){
        fp << "[SLL"
           << "\tR" <<  string2string(rd)
           << ", R" << string2string(rt)
           << ", #" << string2string(sa) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = REGISTERS[string2int(rt)] << string2int(sa);
        return nextins + 1;
    }
}

int SRL(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    string sa(&mipsString[21], &mipsString[26]);
    if(flag==0) {
        fp << "[SRL"
           << "\tR" << string2string(rd)
           << ", R" << string2string(rt)
           << ", #" << string2string(sa) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = (unsigned)REGISTERS[string2int(rt)] >> string2int(sa);
        return nextins + 1;
    }
}

int SRA(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    string sa(&mipsString[21], &mipsString[26]);
    if(flag==0) {
        fp << "[SRA"
           << "\tR" << string2string(rd)
           << ", R" << string2string(rt)
           << ", #" << string2string(sa) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = REGISTERS[string2int(rt)] >> string2int(sa);
        return nextins + 1;
    }
}

int NOP(int flag, int nextins, ofstream &fp)
{
    if(flag==0) {
        fp << "NOP" << "\n";
        return 0;
    }
    else
        return nextins + 1;
}

int ADD(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fp << "[ADD"
           << "\tR" << string2string(rd)
           << ", R" << string2string(rs)
           << ", R" << string2string(rt) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = REGISTERS[string2int(rs)] + REGISTERS[string2int(rt)];
        return nextins + 1;
    }
}

int SUB(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fp << "[SUB"
           << "\tR" << string2string(rd)
           << ", R" << string2string(rs)
           << ", R" << string2string(rt) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = REGISTERS[string2int(rs)] - REGISTERS[string2int(rt)];
        return nextins + 1;
    }
}

int MUL(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fp << "[MUL"
           << "\tR" << string2string(rd)
           << ", R" << string2string(rs)
           << ", R" << string2string(rt) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = REGISTERS[string2int(rs)] * REGISTERS[string2int(rt)];
        return nextins + 1;
    }
}

int AND(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fp << "[AND"
           << "\tR" << string2string(rd)
           << ", R" << string2string(rs)
           << ", R" << string2string(rt) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = (REGISTERS[string2int(rs)] == REGISTERS[string2int(rt)]);
        return nextins + 1;
    }
}

int NOR(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fp << "[NOR"
           << "\tR" << string2string(rd)
           << ", R" << string2string(rs)
           << ", R" << string2string(rt) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = (REGISTERS[string2int(rs)] == 0 && REGISTERS[string2int(rt)] == 0) ? 1 : 0;
        return nextins + 1;
    }
}

int SLT(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fp << "[SLT"
           << "\tR" << string2string(rd)
           << ", R" << string2string(rs)
           << ", R" << string2string(rt) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = (REGISTERS[string2int(rs)] < REGISTERS[string2int(rt)]);
        return nextins + 1;
    }
}

int ADDI(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fp << "[ADD"
           << "\tR" << string2string(rt)
           << ", R" << string2string(rs)
           << ", #" << string2string(ins) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] + string2int(ins));
        return nextins + 1;
    }
}

int SUBI(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fp << "[SUB"
           << "\tR" << string2string(rt)
           << ", R" << string2string(rs)
           << ", #" << string2string(ins) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] - string2int(ins));
        return nextins + 1;
    }
}

int MULI(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fp << "[MUL"
           << "\tR" << string2string(rt)
           << ", R" << string2string(rs)
           << ", #" << string2string(ins) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] * string2int(ins));
        return nextins + 1;
    }
}

int ANDI(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fp << "[AND"
           << "\tR" << string2string(rt)
           << ", R" << string2string(rs)
           << ", #" << string2string(ins) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] == string2int(ins));
        return nextins + 1;
    }
}

int NORI(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fp << "[NOR"
           << "\tR" << string2string(rt)
           << ", R" << string2string(rs)
           << ", #" << string2string(ins) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] == 0 && string2int(ins) == 0) ? 1 : 0;
        return nextins + 1;
    }
}

int SLTI(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fp << "[SLT"
           << "\tR" << string2string(rt)
           << ", R" << string2string(rs)
           << ", #" << string2string(ins) << "]" <<"\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] < string2int(ins));
        return nextins + 1;
    }
}

int J(string mipsString, int flag, int nextins, ofstream &fp)
{
    string ind(&mipsString[6], &mipsString[32]);
    if(flag==0) {
        fp << "J"
           << "\t#" << to_string(4 * string2int(ind)) << "\n";
        return 0;
    }
    else{
        return (string2int(ind)-16);
    }
}

int JR(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    if(flag==0) {
        fp << "JR"
           << "\tR" << string2string(rs) << "\n";
        return 0;
    }
    else{
        return REGISTERS[string2int(rs)];
    }
}

int BEQ(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string offset(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fp << "BEQ"
           << "\tR" << string2string(rs)
           << ", R" << string2string(rt)
           << ", #" << to_string(string2int(offset)*4) << "\n";
        return 0;
    }
    else{
        return (REGISTERS[string2int(rs)] == REGISTERS[string2int(rt)]) ? nextins + string2int(offset) + 1: nextins + 1;
    }
}

int BLTZ(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string offset(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fp << "BLTZ"
           << "\tR" << string2string(rs)
           << ", #" << to_string(string2int(offset)*4) << "\n";
        return 0;
    }
    else{
        nextins = (REGISTERS[string2int(rs)] < 0) ? nextins + string2int(offset) + 1: nextins + 1;
        return (REGISTERS[string2int(rs)] < 0) ? nextins + string2int(offset) + 1: nextins + 1;
    }
}

int BGTZ(string mipsString, int flag, int nextins, ofstream &fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string offset(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fp << "BGTZ"
           << "\tR" << string2string(rs)
           << ", #" << to_string(string2int(offset)*4) << "\n";
        return 0;
    }
    else{
        return (REGISTERS[string2int(rs)] > 0) ? nextins + string2int(offset) + 1: nextins + 1;
    }
}

int BREAK(int flag, ofstream &fp)
{
    if(flag==0) {
        fp << "BREAK"
           << "\n";
        return -1;
    }
    else
        return -1;
}

int SW(string mipsString, int flag, int nextins, ofstream &fp)
{
    string base(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string offset(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fp << "[SW"
           << "\tR" << string2string(rt)
           << ", " << string2string(offset) 
           << "(R" << string2string(base) << ")" <<"]"
           << "\n";
        return 0;
    }
    else{
        DATA[(string2int(offset) - 60) / 4 + REGISTERS[string2int(base)] / 4 - mipslength - 1] = REGISTERS[string2int(rt)];
        return nextins + 1;
    }
}

int LW(string mipsString, int flag, int nextins, ofstream &fp)
{
    string base(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string offset(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fp << "[LW"
           << "\tR" << string2string(rt)
           << ", " << string2string(offset) 
           << "(R" << string2string(base) << ")" << "]" 
           << "\n";
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = DATA[(string2int(offset) - 60) / 4 + REGISTERS[string2int(base)] / 4 - mipslength - 1];
        return nextins + 1;
    }
}


/*----------------------printDisassembly-----------------------------------------*/
int printDisassembly(string mipsString, int flag, int nextins, ofstream &fp)
{
    if(mipsString.size()==0){
        fp << "\n";
        return 0;
    }
    string spcCheck(&mipsString[0], &mipsString[6]);
    string funCheck(&mipsString[26], &mipsString[32]); // trans to string
    if(spcCheck == "000000")
    {
        if(funCheck == "001000")
            return JR(mipsString, flag, nextins, fp);
        else if(funCheck == "001101")
            return BREAK(flag, fp);
        else if(funCheck == "000010")
            return SRL(mipsString, flag, nextins, fp);
        else if(funCheck == "000011")
            return SRA(mipsString, flag, nextins, fp);
        else if(funCheck == "100000")
            return ADD(mipsString, flag, nextins, fp);
        else if(funCheck == "100010")
            return SUB(mipsString, flag, nextins, fp);
        else if(funCheck == "100100")
            return AND(mipsString, flag, nextins, fp);
        else if(funCheck == "100111")
            return NOR(mipsString, flag, nextins, fp);
        else if(funCheck == "101010")
            return SLT(mipsString, flag, nextins, fp);
        else if(funCheck == "000000")
        {
            for (int i = 0; i < mipsString.length() ;i++)
                if(mipsString[i]!='0') 
                    return SLL(mipsString, flag, nextins, fp);  
            return NOP(flag, nextins, fp);
        }
        else
            return -1;
    }
    else if(spcCheck == "000010")
        return J(mipsString, flag, nextins, fp);
    else if(spcCheck == "000100")
        return BEQ(mipsString, flag, nextins, fp);
    else if(spcCheck == "000001")
        return BLTZ(mipsString, flag, nextins, fp);
    else if(spcCheck == "000111")
        return BGTZ(mipsString, flag, nextins, fp);
    else if(spcCheck == "011100")
        return MUL(mipsString, flag, nextins, fp);
    else if(spcCheck == "101011")
        return SW(mipsString, flag, nextins, fp);
    else if(spcCheck == "100011")
        return LW(mipsString, flag, nextins, fp);
    else if(spcCheck == "110000")
        return ADDI(mipsString, flag, nextins, fp);
    else if(spcCheck == "110001")
        return SUBI(mipsString, flag, nextins, fp);
    else if(spcCheck == "100001")
        return MULI(mipsString, flag, nextins, fp);
    else if(spcCheck == "110010")
        return ANDI(mipsString, flag, nextins, fp);
    else if(spcCheck == "110011")
        return NORI(mipsString, flag, nextins, fp);
    else if(spcCheck == "110101")
        return SLTI(mipsString, flag, nextins, fp);
    else
        return -1;
}

/*----------------------getInstruction-----------------------------------------*/
string getInstruction(string mipsString)
{
    string spcCheck(&mipsString[0], &mipsString[6]);
    string funCheck(&mipsString[26], &mipsString[32]); // trans to string
    if(spcCheck == "000000")
    {
        if(funCheck == "001000")
            return "JR";
        else if(funCheck == "001101")
            return "BREAK";
        else if(funCheck == "000010")
            return "SRL";
        else if(funCheck == "000011")
            return "SRA";
        else if(funCheck == "100000")
            return "ADD";
        else if(funCheck == "100010")
            return "SUB";
        else if(funCheck == "100100")
            return "AND";
        else if(funCheck == "100111")
            return "NOR";
        else if(funCheck == "101010")
            return "SLT";
        else if(funCheck == "000000")
        {
            for (int i = 0; i < mipsString.length() ;i++)
                if(mipsString[i]!='0') 
                    return "SLL";  
            return "NOP";
        }
        else
            return "ERROR";
    }
    else if(spcCheck == "000010")
        return "J";
    else if(spcCheck == "000100")
        return "BEQ";
    else if(spcCheck == "000001")
        return "BLTZ";
    else if(spcCheck == "000111")
        return "BGTZ";
    else if(spcCheck == "011100")
        return "MUL";
    else if(spcCheck == "101011")
        return "SW";
    else if(spcCheck == "100011")
        return "LW";
    else if(spcCheck == "110000")
        return "ADDI";
    else if(spcCheck == "110001")
        return "SUBI";
    else if(spcCheck == "100001")
        return "MULI";
    else if(spcCheck == "110010")
        return "ANDI";
    else if(spcCheck == "110011")
        return "NORI";
    else if(spcCheck == "110101")
        return "SLTI";
    else
        return "ERROR";
}

/*----------------------isBranch-----------------------------------------*/
int isBranch(string mipsString)
{
    string Ins(getInstruction(mipsString));
    if(Ins == "J" || Ins == "JR" || Ins == "BEQ" || Ins == "BLTZ" || Ins == "BGTZ")
        return 1;
    return 0;
}

/*----------------------getRegisters-----------------------------------------*/
void getRegisters(string mipsString, int *result)
{
    string spcCheck(&mipsString[0], &mipsString[6]);
    string funCheck(&mipsString[26], &mipsString[32]);
    string part1(&mipsString[6], &mipsString[11]);
    string part2(&mipsString[11], &mipsString[16]);
    string part3(&mipsString[16], &mipsString[21]);
    int p1 = string2int(part1);
    int p2 = string2int(part2);
    int p3 = string2int(part3);
    int targetReg = -1;
    int sourceReg[2] = {-1, -1};
    if(spcCheck == "000000")
    {
        if(funCheck == "000010")
            {targetReg = p3; sourceReg[0] = p2;}
        else if(funCheck == "001000")
            targetReg = p1;
        else if(funCheck == "000011")
            {targetReg = p3; sourceReg[0] = p2;}
        else if(funCheck == "100000")
            {targetReg = p3; sourceReg[0] = p1; sourceReg[1] = p2;}
        else if(funCheck == "100010")
            {targetReg = p3; sourceReg[0] = p1; sourceReg[1] = p2;}
        else if(funCheck == "100100")
            {targetReg = p3; sourceReg[0] = p1; sourceReg[1] = p2;}
        else if(funCheck == "100111")
            {targetReg = p3; sourceReg[0] = p1; sourceReg[1] = p2;}
        else if(funCheck == "101010")
            {targetReg = p3; sourceReg[0] = p1; sourceReg[1] = p2;}
        else if(funCheck == "000000")
        {
            targetReg = p3; sourceReg[0] = p2;
        }
    }
    else if(spcCheck == "000100")
        {targetReg = p1; sourceReg[0] = p2;}
    else if(spcCheck == "000001")
        {targetReg = p1;}
    else if(spcCheck == "000111")
        {targetReg = p1;}
    else if(spcCheck == "011100")
        {targetReg = p3; sourceReg[0] = p1; sourceReg[1] = p2;}
    else if(spcCheck == "101011")
        {targetReg = p2; sourceReg[0] = p1;}
    else if(spcCheck == "100011")
        {targetReg = p2; sourceReg[0] = p1;}
    else if(spcCheck == "110000")
        {targetReg = p2; sourceReg[0] = p1;}
    else if(spcCheck == "110001")
        {targetReg = p2; sourceReg[0] = p1;}
    else if(spcCheck == "100001")
        {targetReg = p2; sourceReg[0] = p1;}
    else if(spcCheck == "110010")
        {targetReg = p2; sourceReg[0] = p1;}
    else if(spcCheck == "110011")
        {targetReg = p2; sourceReg[0] = p1;}
    else if(spcCheck == "110101")
        {targetReg = p2; sourceReg[0] = p1;}
    
    result[0] = targetReg;
    result[1] = sourceReg[0];
    result[2] = sourceReg[1];
    return;
}

/*----------------------isConflict-----------------------------------------*/
int isConflict(string mipsSting, int *reg, int flag)
//flag to choose the target Reg or source Reg
//flag == 0 --> target
//flag == 1 --> source 
{
    int tmp[3];
    getRegisters(mipsSting, tmp);
    if(flag == 0){
        if(reg[tmp[0]] == 1)
            return 1;
    }
    else{
        for(int i = 1; i < 3; i++){
            if(tmp[i] != -1){
                if(reg[tmp[i]] == 1)
                    return 1;
            }
        }
    }
    return 0;
}

/*----------------------printRegisters-----------------------------------------*/
int printRegisters(ofstream &fp, int cycle)
{
    fp << "--------------------\n" << "Cycle:" << to_string(cycle) << "\n";
    //--Output IF Unit--
    fp  << "\n" 
        << "IF Unit:" << "\n";
    fp  << "\t" << "Waiting Instruction: "; printDisassembly(WaitIns, 0, 0, fp);
    fp  << "\t" << "Executed Instruction: "; printDisassembly(ExeIns, 0, 0, fp);
    //--Output Pre-Issue Buffer--
    fp  << "Pre-Issue Buffer:" << "\n";
    fp  << "\t" << "Entry 0:" ; printDisassembly(PreIssue[0], 0, 0, fp);
    fp  << "\t" << "Entry 1:" ; printDisassembly(PreIssue[1], 0, 0, fp);
    fp  << "\t" << "Entry 2:" ; printDisassembly(PreIssue[2], 0, 0, fp);
    fp  << "\t" << "Entry 3:" ; printDisassembly(PreIssue[3], 0, 0, fp);
    //--Output Pre-ALU Queue--
    fp  << "Pre-ALU Queue:" << "\n" ;
    fp  << "\t" << "Entry 0:"; printDisassembly(PreALU[0], 0, 0, fp);
    fp  << "\t" << "Entry 1:"; printDisassembly(PreALU[1], 0, 0, fp);
    //--Output Post-ALU Buffer--
    fp  << "Post-ALU Buffer:"; printDisassembly(PostALU, 0, 0, fp);
    //--Output Pre-ALUB Queue--x7fffffffdff8
    fp  << "Pre-ALUB Queue:" << "\n" ;
    fp  << "\t" << "Entry 0:" ; printDisassembly(PreALUB[0], 0, 0, fp);
    fp  << "\t" << "Entry 1:" ; printDisassembly(PreALUB[1], 0, 0, fp);
    //--Output Post-ALUB Buffer--
    fp  << "Post-ALUB Buffer:"; printDisassembly(PostALUB, 0, 0, fp);
    //--Output Pre-MEM Queue--
    fp  << "Pre-MEM Queue:" << "\n" ;
    fp  << "\t" << "Entry 0:"; printDisassembly(PreMEM[0], 0, 0, fp);
    fp  << "\t" << "Entry 1:"; printDisassembly(PreMEM[1], 0, 0, fp);
    //--Output Post-MEM Buffer--
    fp  << "Post-MEM Buffer:"; printDisassembly(PostMEM, 0, 0, fp);
    
    //--Output Registers--
    fp << "\n"
       << "Registers"
       << "\n"
       << "R00:";
    for (int i = 0; i < 32; i++)
    {
        if(i == 8)
            fp << "\nR08:";
        if (i == 16 || i == 24)
            fp << "\nR" << to_string(i) << ":";
        fp << "\t" << to_string(REGISTERS[i]);
    }

    //--Output Data--
    fp << "\n\nData";
    for (int i = 0; i < DATA.size(); i++)
    {
        if (i % 8 == 0 && i != DATA.size())
            fp << "\n"
               << to_string((i + mipsString.size()) * 4 + 64) << ":";
        fp << "\t" << to_string(DATA[i]);
    }
    fp << "\n";
    return 0;
}

/*----------------------isAllIssuedEmpty-----------------------------------------*/
int isAllIssuedEmpty()
{
    int flag = 1;
    if(stringVectorCount(PreALU) > 0 || stringVectorCount(PreALUB) > 0 || stringVectorCount(PreMEM) > 0 ||
        PostALU.empty() == 0 || PostALUB.empty() == 0 || PostMEM.empty() == 0)
        flag = 0;
    return flag;
}
/*----------------------sortIssue-----------------------------------------*/
void sortIssue(vector<string> &v, int flag)
{
    if(stringVectorCount(v) > 0 && flag < stringVectorCount(v))   //if string is not empty
        while(v[flag].empty()){
            for(int i = flag + 1; i < v.size(); i++){
                v[i-1] = v[i];
                v[i] = "";
            }
        }
}

void supersort(vector<string> &v)
{
    for (int i = 0; i < v.size() - 1; i++){
        while(v[i].empty() && i < stringVectorCount(v)){
            for (int j = i; j < v.size() - 1; j++){
                v[j] = v[j + 1];
                v[j + 1] = "";
            }
        }
    }
}

/*----------------------getBranchResult-----------------------------------------*/
int getBranchResult(string mipsString, int inscount)
{  
    if(getInstruction(mipsString) == "JR"){                       //JR
        string rs(&mipsString[6], &mipsString[11]);
        return REGISTERS[string2int(rs)];
    }
    
    else if(getInstruction(mipsString) == "J"){                      //J
        string ind(&mipsString[6], &mipsString[32]);
        return string2int(ind) - 16;
    }
    else if(getInstruction(mipsString) == "BEQ"){                      //BEQ
        string rs(&mipsString[6], &mipsString[11]);
        string rt(&mipsString[11], &mipsString[16]);
        string offset(&mipsString[16], &mipsString[32]);
        return (REGISTERS[string2int(rs)] == REGISTERS[string2int(rt)]) ? inscount + string2int(offset) + 1 : inscount + 1;
    }
    else if(getInstruction(mipsString) == "BLTZ"){                      //BLTZ
        string rs(&mipsString[6], &mipsString[11]);
        string offset(&mipsString[16], &mipsString[32]);
        return (REGISTERS[string2int(rs)] < 0) ? inscount + string2int(offset) + 1: inscount + 1;
    }
    else if(getInstruction(mipsString) == "BGTZ"){                      //BGTZ
        string rs(&mipsString[6], &mipsString[11]);
        string offset(&mipsString[16], &mipsString[32]);
        return (REGISTERS[string2int(rs)] > 0) ? inscount + string2int(offset) + 1: inscount + 1;
    }
    return -1;
}


/*-------------------------------------------------main------------------------------------------------------*/
int main(int argc, char const *argv[])
{
    remove("./disassembly.txt");
    remove("./simulation.txt");

    ifstream sample;        //init to read from sample
    ofstream disassembly;   //init to write
    ofstream simulation;

    sample.open(argv[1]);   // read from argv
    disassembly.open("disassembly.txt");

    
    int turn = 0;
    //-------------------------------------------------------------
    //Begin to print the disassembly and store in the string array
    do{
        string current_input_line;
        string output;                              // save the string output to disassembly
        
        getline(sample, current_input_line);        // read one line from sample.txt
        mipsString.push_back(current_input_line);   //save to mipsString

        for (int i = 0; i < 32; i++){
            output = output + mipsString[turn][i];
            if (i == 0 || i == 5 || i == 10 || i == 15 || i == 20 || i == 25)
                output = output + " ";
        }
        disassembly << output << "\t" << turn * 4 + 64 << "\t";
    } while (!printDisassembly(mipsString[turn++], 0, 0, disassembly));
    


    //-------------------------------------------------------------
    //Begin to print and store the number
    while (sample.peek() != EOF) // when not reach the EOF of file
    {
        string current_input_line;
        string output;                                      //save the number output to disassembly
        getline(sample, current_input_line);                //read a line of number
        DATA.push_back(truevalue(current_input_line));      //save to DATA

        disassembly << current_input_line << "\t" 
                    << to_string(turn++ * 4 + 64) << "\t" 
                    << to_string(truevalue(current_input_line)) << "\n";
    }
    disassembly.close();
    sample.close();
    mipslength = mipsString.size();

    //------------------------------print simulation----------------------------------------------
    int pc = 0;                 //pc
    int endflag = 0;            //if break
    int nextins = 0;            //store the next pc
    int cycle = 0;              //count the cycle
    int inscount = 0;           //conut the ins
    int tregStatus[32];         //检查已发射指令target register占用情况
    int sregStatus[32];         //检查已发射指令source register占用情况
    int targetSWStatus[32];     //检查已发射SWtarget占用情况
    int sourceSWStatus[32];     //
    int *targetLWStatus = new int[32];
    int *sourceLWStatus = new int[32];
    int pretregStatus[32];      //检查pre-issue队列target register的占用情况
    int presregStatus[32];      //检查pre-issue队列source register的占用情况
    int preALUBcount = 0;
    simulation.open("simulation.txt");

    //init register status 
    //Outside the cycle
    for(int i = 0; i < 32; i++){
        tregStatus[i] = 0;
        sregStatus[i] = 0;
        targetSWStatus[i] = 0;
        sourceSWStatus[i] = 0;
        targetLWStatus[i] = 0;
        sourceLWStatus[i] = 0;
    }

    //---------------cycle-------------------------
    // for (int m = 0; m < 60;m++){
    while (endflag == 0){
        int issueCount = 0;                                 //at most issue 2 ins one cycle
        int preIssueLength = stringVectorCount(PreIssue);   //记录pre-issue队列长度
        int preALULength = stringVectorCount(PreALU);   //记录pre-ALU队列长度
        int preALUBLength = stringVectorCount(PreALUB);   //记录pre-ALUB队列长度
        int preMEMLength = stringVectorCount(PreMEM);   //记录pre-MEM队列长度
        int isSWnotIssue = 0;       //检查pre-issue队列是否有not-issued SW
        int isSW = 0;               //检查是否执行了SW操作
        int ifLW = 1;               //检查是否可以执行LW
        // int ifRunBranch = 0;        //检查是否执行了跳转
        ++cycle;

        //init pre-issue status
        //init each cycle
        for(int i = 0; i < 32; i++){
            pretregStatus[i] = 0;
            presregStatus[i] = 0;
        }

        /*-----------------pre-Issue ins branch-----------------
        *WaitIns
        *ExeIns
        ------------------------------------------------------*/
        int WaitInsReg[3];
        if (ExeIns.empty() == 0)
        {
            //Run the branch, the next is written to inscount
            inscount = getBranchResult(ExeIns, inscount);
            ExeIns = "";
            // ifRunBranch = 1;
        }
        if(WaitIns.empty() == 0){
            getRegisters(WaitIns, WaitInsReg);
            if (isConflict(WaitIns, tregStatus, 0) == 0 &&               //target is conflict with target reg
                isConflict(WaitIns, sregStatus, 0) == 0 &&               //target is conflict with source reg
                isConflict(WaitIns, tregStatus, 1) == 0 &&
                isConflict(WaitIns, targetLWStatus, 0) == 0 &&
                isConflict(WaitIns, sourceLWStatus, 0) == 0 &&
                isConflict(WaitIns, targetLWStatus, 1) == 0 &&
                stringVectorCount(PreIssue) <= 1)
            {
                ExeIns = WaitIns;
                WaitIns = "";
            }
        }

        /*-----------------post buffer-----------------------
        *将操作写回
        *清空操作对寄存器的占用 --在Issue结束后执行--
        *清空post buffer
        ------------------------------------------------------*/
        int ifpostALU = 0;
        int ifpostALUB = 0;
        int ifpostMEM = 0;
        int postALURegArray[3];
        int postALUBRegArray[3];
        int postMEMRegArray[3];
        //post-ALU
        if(PostALU.empty() == 0){
            printDisassembly(PostALU, 1, nextins, simulation);
            getRegisters(PostALU, postALURegArray);
            PostALU = "";
            ifpostALU = 1;
        }
        //post-ALUB
        if(PostALUB.empty() == 0){
            printDisassembly(PostALUB, 1, nextins, simulation);
            getRegisters(PostALUB, postALUBRegArray);
            PostALUB = "";
            ifpostALUB = 1;
        }
        //post-MEM
        if(PostMEM.empty() == 0){
            printDisassembly(PostMEM, 1, nextins, simulation);
            getRegisters(PostMEM, postMEMRegArray);
            PostMEM = "";
            ifpostMEM = 1;
        }
        
        /*-----------------pre queue----------------------------
        *执行，转移到post
        *整理队列
        ------------------------------------------------------*/
        //Pre-ALU
        if(stringVectorCount(PreALU) > 0 && PostALU.empty()){
            PostALU = PreALU[0];
            PreALU[0] = "";
            // sortIssue(PreALU, 0);
            supersort(PreALU);
        }

        //Pre-ALUB
        if(stringVectorCount(PreALUB) > 0 && PostALUB.empty()){
            if(preALUBcount == 1){
                PostALUB = PreALUB[0];
                PreALUB[0] = "";
                // sortIssue(PreALUB, 0);
                supersort(PreALUB);
                preALUBcount = 0;
            }
            else{
                preALUBcount++;
            }
        }

        //Pre-MEM
        if(stringVectorCount(PreMEM) > 0 && PostMEM.empty()){
            if(getInstruction(PreMEM[0]) == "SW"){
                printDisassembly(PreMEM[0], 1, nextins, simulation);
                getRegisters(PreMEM[0], postMEMRegArray);
                PreMEM[0] = "";
                // sortIssue(PreMEM, 0);
                supersort(PreMEM);
                ifpostMEM = 1;
                isSW = 1;
            }
            else{
                PostMEM = PreMEM[0];
                PreMEM[0] = "";
                // sortIssue(PreMEM, 0);
                supersort(PreMEM);
            }
        }

        
        /*-----------------Issue ins--------------------------
        对一条Pre-Issue中的指令:
        *目标pre queue是否已满
        *指令->目标寄存器 检查 是否与目标寄存器List冲突(WAW)
        *指令->目标寄存器 检查 是否与源寄存器List冲突(RAW)
        *指令->源寄存器 检查 是否与目标寄存器List冲突(WAR)
        ------------------------------------------------------*/
        
        int preIssuePTR = 0;
        while(PreIssue[preIssuePTR].empty() == 0 && issueCount < 2 && preIssuePTR < 4){
            string toIssueIns(getInstruction(PreIssue[preIssuePTR]));                 //get the ins name
            int regArray[3];
            getRegisters(PreIssue[preIssuePTR], regArray); //get ins regs


            //to pre-MEM
            if(toIssueIns == "SW"){
                if(isConflict(PreIssue[preIssuePTR], pretregStatus, 0) == 0 &&             //target is conflict with pre-issue target
                    isConflict(PreIssue[preIssuePTR], presregStatus, 0) == 0 &&            //target is conflict with pre-issue source
                    isConflict(PreIssue[preIssuePTR], pretregStatus, 1) == 0 &&            //source is conflict with pre-issue target
                    isConflict(PreIssue[preIssuePTR], tregStatus, 0) == 0 &&               //target is conflict with target reg
                    isConflict(PreIssue[preIssuePTR], sregStatus, 0) == 0 &&               //target is conflict with source reg
                    isConflict(PreIssue[preIssuePTR], tregStatus, 1) == 0 &&               //source is conflict with target reg
                    preMEMLength < 2){                           //is pre full

                        PreMEM[stringVectorCount(PreMEM)] = PreIssue[preIssuePTR];      
                        PreIssue[preIssuePTR] = "";    
                        // sortIssue(PreIssue, preIssuePTR);
                        supersort(PreIssue);                                  //sort the pre-issue
                        tregStatus[regArray[0]] = 1;                          //mark as used
                        targetSWStatus[regArray[0]] = 1;
                        for (int j = 1; j < 3; j++)
                        {
                            if(regArray[j] != -1){
                                sregStatus[regArray[j]] = 1;                  //mark as used
                                sourceSWStatus[regArray[j]] = 1;
                            }
                        }
                        issueCount++;
                        preMEMLength++;
                }
                else{
                    pretregStatus[regArray[0]] = 1;                            //mark as conflict
                    for(int j = 1; j < 3; j++){
                            if(regArray[j] != -1){
                                presregStatus[regArray[j]] = 1;                  //mark as used
                            }
                        }
                    isSWnotIssue = 1;
                    preIssuePTR++;
                }
            }

            //to pre-MEM
            //LW only conflict with not-issued SW
            //no need to mark as conflict
            else if (toIssueIns == "LW")
            {
                if ((isConflict(PreIssue[preIssuePTR], tregStatus, 0) == 1 && isConflict(PreIssue[preIssuePTR], targetSWStatus, 0) == 0) &&
                    (isConflict(PreIssue[preIssuePTR], tregStatus, 1) == 1 && isConflict(PreIssue[preIssuePTR], targetSWStatus, 1) == 0))
                    ifLW = 0;
                if(isSWnotIssue == 0 && preMEMLength < 2 && ifLW && 
                    isConflict(PreIssue[preIssuePTR], tregStatus, 1) == 0){
                        PreMEM[stringVectorCount(PreMEM)] = PreIssue[preIssuePTR];
                        PreIssue[preIssuePTR] = "";    
                        // sortIssue(PreIssue, preIssuePTR);
                        supersort(PreIssue);                              //sort the pre-issue
                        issueCount++;
                        preMEMLength++;
                        targetLWStatus[regArray[0]] = 1;
                        for (int j = 1; j < 3; j++)
                        {
                            if(regArray[j] != -1){
                                sourceLWStatus[regArray[j]] = 1;
                            }
                        }
                }
                else
                    preIssuePTR++;
            }

            //to pre-ALUB
            else if(toIssueIns == "SLL" || toIssueIns == "SRL" || toIssueIns == "SRA" || toIssueIns == "MUL"){
                if(isConflict(PreIssue[preIssuePTR], pretregStatus, 0) == 0 &&             //target is conflict with pre-issue target
                    isConflict(PreIssue[preIssuePTR], presregStatus, 0) == 0 &&            //target is conflict with pre-issue source
                    isConflict(PreIssue[preIssuePTR], pretregStatus, 1) == 0 &&            //source is conflict with pre-issue target
                    isConflict(PreIssue[preIssuePTR], tregStatus, 0) == 0 &&               //target is conflict with target reg
                    isConflict(PreIssue[preIssuePTR], sregStatus, 0) == 0 &&               //target is conflict with source reg
                    isConflict(PreIssue[preIssuePTR], tregStatus, 1) == 0 &&
                    isConflict(PreIssue[preIssuePTR], targetLWStatus, 0) == 0 &&               //target is conflict with target reg
                    isConflict(PreIssue[preIssuePTR], sourceLWStatus, 0) == 0 &&               //target is conflict with source reg
                    isConflict(PreIssue[preIssuePTR], targetLWStatus, 1) == 0 &&                //source is conflict with target reg
                    preALUBLength < 2){                         //is pre full
                        PreALUB[stringVectorCount(PreALUB)] = PreIssue[preIssuePTR];
                        PreIssue[preIssuePTR] = "";    
                        // sortIssue(PreIssue, preIssuePTR);
                        supersort(PreIssue);
                        tregStatus[regArray[0]] = 1;
                        for(int j = 1; j < 3; j++){
                            if(regArray[j] != -1){
                                sregStatus[regArray[j]] = 1;                  //mark as used
                            }
                        }
                        issueCount++;
                        preALUBLength++;
                }
                else{
                    pretregStatus[regArray[0]] = 1;
                    for(int j = 1; j < 3; j++){
                            if(regArray[j] != -1){
                                presregStatus[regArray[j]] = 1;                  //mark as used
                            }
                    }
                    preIssuePTR++;
                }
            }
            //to pre-ALU
            else{
                if(isConflict(PreIssue[preIssuePTR], pretregStatus, 0) == 0 &&             //target is conflict with pre-issue target
                    isConflict(PreIssue[preIssuePTR], presregStatus, 0) == 0 &&            //target is conflict with pre-issue source
                    isConflict(PreIssue[preIssuePTR], pretregStatus, 1) == 0 &&            //source is conflict with pre-issue target
                    isConflict(PreIssue[preIssuePTR], tregStatus, 0) == 0 &&               //target is conflict with target reg
                    isConflict(PreIssue[preIssuePTR], sregStatus, 0) == 0 &&               //target is conflict with source reg
                    isConflict(PreIssue[preIssuePTR], tregStatus, 1) == 0 &&
                    isConflict(PreIssue[preIssuePTR], targetLWStatus, 0) == 0 &&               //target is conflict with target reg
                    isConflict(PreIssue[preIssuePTR], sourceLWStatus, 0) == 0 &&               //target is conflict with source reg
                    isConflict(PreIssue[preIssuePTR], targetLWStatus, 1) == 0 &&                //source is conflict with target reg
                    preALULength < 2){                         //is pre full
                        PreALU[stringVectorCount(PreALU)] = PreIssue[preIssuePTR];
                        PreIssue[preIssuePTR] = "";
                        // sortIssue(PreIssue, preIssuePTR);
                        supersort(PreIssue);

                        tregStatus[regArray[0]] = 1;
                        for(int j = 1; j < 3; j++){
                            if(regArray[j] != -1){
                                sregStatus[regArray[j]] = 1;                  //mark as used
                            }
                        }
                        issueCount++;
                        preALULength++;
                }
                else{
                    pretregStatus[regArray[0]] = 1;
                    for(int j = 1; j < 3; j++){
                            if(regArray[j] != -1){
                                presregStatus[regArray[j]] = 1;                  //mark as used
                            }
                    }
                    preIssuePTR++;
                }
            }
        }

        /*-----------------post buffer-----------------------
        *清空操作对寄存器的占用 --在Issue结束后执行-
        ------------------------------------------------------*/
        if(ifpostALU){
            tregStatus[postALURegArray[0]] = 0;        
            for(int i = 1; i < 3; i++)
                if(postALURegArray[i] >= 0)
                    sregStatus[postALURegArray[i]] = 0;
        }
        if(ifpostALUB){
            tregStatus[postALUBRegArray[0]] = 0;        
            for(int i = 1; i < 3; i++)
                if(postALUBRegArray[i] >= 0)
                    sregStatus[postALUBRegArray[i]] = 0;
        }
        if(ifpostMEM){ 
            if(isSW){
                targetSWStatus[postMEMRegArray[0]] = 0;
                tregStatus[postMEMRegArray[0]] = 0;
                for(int i = 1; i < 3; i++)
                    if(postMEMRegArray[i] >= 0){
                        sourceSWStatus[postMEMRegArray[i]] = 0;
                        sregStatus[postMEMRegArray[i]] = 0;
                    }
            }
            else{
                targetLWStatus[postMEMRegArray[0]] = 0;
                for(int i = 1; i < 3; i++)
                    if(postMEMRegArray[i] >= 0)
                        sourceLWStatus[postMEMRegArray[i]] = 0;
            }
        }


        /*-----------------Ins Fetch-----------------------------
        *有指令stall时不fetch
        *fetch到分支指令时忽略后一个
        *fetch到分支指令inscount不继续自增，等待分支返回inscount
        ------------------------------------------------------*/
        // if(WaitIns.empty() && ExeIns.empty() && preIssueLength < 4 && ifRunBranch == 0){
        if(WaitIns.empty() && ExeIns.empty() && preIssueLength < 4){
            //Read one ins case
            if(preIssueLength == 3){
                //Branch case
                //inscount stop count, the next will be given by the branch
                if(isBranch(mipsString[inscount])){
                    if(getInstruction(mipsString[inscount]) == "J" || getInstruction(mipsString[inscount]) == "JR")
                        ExeIns = mipsString[inscount];
                    else
                        WaitIns = mipsString[inscount];
                    
                }
                //Break case
                else if(getInstruction(mipsString[inscount]) == "BREAK"){
                    ExeIns = mipsString[inscount];
                    endflag = 1;
                }
                //Other case
                else{
                    PreIssue[3] = mipsString[inscount++];
                }
            }

            //Read two ins case
            else{
                for(int i = 0; i < 2; i++){
                    //Branch case
                    if(isBranch(mipsString[inscount])){
                        if(getInstruction(mipsString[inscount]) == "J" || getInstruction(mipsString[inscount]) == "JR")
                            ExeIns = mipsString[inscount];
                        else{
                            WaitIns = mipsString[inscount];
                        }
                        if(i==0)
                            break;
                    }
                    //Break case
                    else if(getInstruction(mipsString[inscount]) == "BREAK"){
                        ExeIns = mipsString[inscount];
                        endflag = 1;
                        if(i==0)
                            break;
                    }
                    //Other case
                    else
                        PreIssue[stringVectorCount(PreIssue)] = mipsString[inscount++];
                    
                }
            }

            supersort(PreIssue);
        }

        

        printRegisters(simulation, cycle);

    }
    simulation.close();
    return 0;
}
