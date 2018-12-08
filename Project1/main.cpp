#include<cstdio>
#include<cstdlib>
#include<string>
#include<iostream>
#include<fstream>
using namespace std;

const int M = 10000;
int *REGISTERS = new int[32]();
int *DATA = new int[M]();
int tailofinstruction;
// const string Instruction[18] = ["J","JR","BEQ","BLTZ","BGTZ","BREAK","SW","LW","SLL","SRL","SRA","NOP","ADD","SUB","MUL","AND", "NOR","SLT"];

int string2int(string s)
{
    return stoi(s,0,2);
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

/************************************************
 * INSTRUCTION DEF
 ***********************************************/

int SLL(string mipsString, int flag, int nextins, FILE *fp)
{
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    string sa(&mipsString[21], &mipsString[26]);
    if (flag == 0){
        fprintf(fp,"SLL R%d, R%d, #%d\n", string2int(rd), string2int(rt), string2int(sa));
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = REGISTERS[string2int(rt)] << string2int(sa);
        return nextins + 1;
    }
}

int SRL(string mipsString, int flag, int nextins, FILE *fp)
{
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    string sa(&mipsString[21], &mipsString[26]);
    if(flag==0) {
        fprintf(fp,"SRL R%d, R%d, #%d\n", string2int(rd), string2int(rt), string2int(sa));
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = (unsigned)REGISTERS[string2int(rt)] >> string2int(sa);
        return nextins + 1;
    }
}

int SRA(string mipsString, int flag, int nextins, FILE *fp)
{
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    string sa(&mipsString[21], &mipsString[26]);
    if(flag==0) {
        fprintf(fp,"SRA R%d, R%d, #%d\n", string2int(rd), string2int(rt), string2int(sa));
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = REGISTERS[string2int(rt)] >> string2int(sa);
        return nextins + 1;
    }
}

int NOP(int flag, int nextins, FILE *fp)
{
    if(flag==0) {
        fprintf(fp,"NOP\n");
        return 0;
    }
    else
        return nextins + 1;
}

int ADD(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fprintf(fp,"ADD R%d, R%d, R%d\n", string2int(rd), string2int(rs), string2int(rt));
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = REGISTERS[string2int(rs)] + REGISTERS[string2int(rt)];
        return nextins + 1;
    }
}

int SUB(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fprintf(fp,"SUB R%d, R%d, R%d\n", string2int(rd), string2int(rs), string2int(rt));
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = REGISTERS[string2int(rs)] - REGISTERS[string2int(rt)];
        return nextins + 1;
    }
}

int MUL(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fprintf(fp,"MUL R%d, R%d, R%d\n", string2int(rd), string2int(rs), string2int(rt));
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = REGISTERS[string2int(rs)] * REGISTERS[string2int(rt)];
        return nextins + 1;
    }
}

int AND(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fprintf(fp,"AND R%d, R%d, R%d\n", string2int(rd), string2int(rs), string2int(rt));
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = (REGISTERS[string2int(rs)] == REGISTERS[string2int(rt)]);
        return nextins + 1;
    }
}

int NOR(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fprintf(fp,"NOR R%d, R%d, R%d\n", string2int(rd), string2int(rs), string2int(rt));
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = (REGISTERS[string2int(rs)] == 0 && REGISTERS[string2int(rt)] == 0) ? 1 : 0;
        return nextins + 1;
    }
}

int SLT(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string rd(&mipsString[16], &mipsString[21]);
    if(flag==0) {
        fprintf(fp,"SLT R%d, R%d, R%d\n", string2int(rd), string2int(rs), string2int(rt));
        return 0;
    }
    else{
        REGISTERS[string2int(rd)] = (REGISTERS[string2int(rs)] < REGISTERS[string2int(rt)]);
        return nextins + 1;
    }
}

int ADDI(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"ADD R%d, R%d, #%d\n", string2int(rt), string2int(rs), string2int(ins));
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] + string2int(ins));
        return nextins + 1;
    }
}

int SUBI(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"SUB R%d, R%d, #%d\n", string2int(rt), string2int(rs), string2int(ins));
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] - string2int(ins));
        return nextins + 1;
    }
}

int MULI(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"MUL R%d, R%d, #%d\n", string2int(rt), string2int(rs), string2int(ins));
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] * string2int(ins));
        return nextins + 1;
    }
}

int ANDI(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"AND R%d, R%d, #%d\n", string2int(rt), string2int(rs), string2int(ins));
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] == string2int(ins));
        return nextins + 1;
    }
}

int NORI(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"NOR R%d, R%d, #%d\n", string2int(rt), string2int(rs), string2int(ins));
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] == 0 && string2int(ins) == 0) ? 1 : 0;
        return nextins + 1;
    }
}

int SLTI(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string ins(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"SLT R%d, R%d, #%d\n", string2int(rt), string2int(rs), string2int(ins));
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = (REGISTERS[string2int(rs)] < string2int(ins));
        return nextins + 1;
    }
}

int J(string mipsString, int flag, int nextins, FILE *fp)
{
    string ind(&mipsString[6], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"J #%d\n", 4*string2int(ind));
        return 0;
    }
    else{
        return (string2int(ind)-16);
    }
}

int JR(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    if(flag==0) {
        fprintf(fp,"JR R%d\n", string2int(rs));
        return 0;
    }
    else{
        return REGISTERS[string2int(rs)];
    }
}

int BEQ(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string offset(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"BEQ R%d, R%d, #%d\n", string2int(rs), string2int(rt), string2int(offset)*4);
        return 0;
    }
    else{
        return (REGISTERS[string2int(rs)] == REGISTERS[string2int(rt)]) ? nextins + string2int(offset) + 1: nextins + 1;
    }
}

int BLTZ(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string offset(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"BLTZ R%d, #%d\n", string2int(rs), string2int(offset)*4);
        return 0;
    }
    else{
        return (REGISTERS[string2int(rs)] < 0) ? nextins + string2int(offset) + 1: nextins + 1;
    }
}

int BGTZ(string mipsString, int flag, int nextins, FILE *fp)
{
    string rs(&mipsString[6], &mipsString[11]);
    string offset(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"BGTZ R%d, #%d\n", string2int(rs), string2int(offset)*4);
        return 0;
    }
    else{
        return (REGISTERS[string2int(rs)] > 0) ? nextins + string2int(offset) + 1: nextins + 1;
    }
}

int BREAK(int flag, FILE *fp)
{
    if(flag==0) {
        fprintf(fp,"BREAK\n");
        return -1;
    }
    else
        return -1;
}

int SW(string mipsString, int flag, int nextins, FILE *fp)
{
    string base(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string offset(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"SW R%d, %d(R%d)\n", string2int(rt), string2int(offset), string2int(base));
        return 0;
    }
    else{
        DATA[(string2int(offset) - 60) / 4 + REGISTERS[string2int(base)] / 4 - tailofinstruction - 2] = REGISTERS[string2int(rt)];
        return nextins + 1;
    }
}

int LW(string mipsString, int flag, int nextins, FILE *fp)
{
    string base(&mipsString[6], &mipsString[11]);
    string rt(&mipsString[11], &mipsString[16]);
    string offset(&mipsString[16], &mipsString[32]);
    if(flag==0) {
        fprintf(fp,"LW R%d, %d(R%d)\n", string2int(rt), string2int(offset), string2int(base));
        return 0;
    }
    else{
        REGISTERS[string2int(rt)] = DATA[(string2int(offset) - 60) / 4 + REGISTERS[string2int(base)] / 4 - tailofinstruction - 2];
        return nextins + 1;
    }
}

/************************************************
 * printDisassembly
 ***********************************************/



int printDisassembly(string mipsString, int flag, int nextins, FILE *fp)
{
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
        else if(funCheck == "000010")
            return MUL(mipsString, flag, nextins, fp);
        else if(funCheck == "100100")
            return AND(mipsString, flag, nextins, fp);
        else if(funCheck == "100111")
            return NOR(mipsString, flag, nextins, fp);
        else if(funCheck == "101010")
            return SLT(mipsString, flag, nextins, fp);
        else if(funCheck == "000000")
        {
            for (int i = flag; i < mipsString.length() ;i++)
                if(mipsString[i]!='0') return SLL(mipsString, flag, nextins, fp);  
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

/**
 * len: the length of Data
 * tail: the end line of register
 **/
int printRegisters(int len, int tail, FILE *fp)
{
    fprintf(fp, "\nRegisters");
    fprintf(fp, "\nR00:");
    for (int i = 0; i < 32; i++)
    {
        if(i == 16){
            fprintf(fp, "\nR%d:", i);
        }
        fprintf(fp, "\t%d" , REGISTERS[i]);
    }

    fprintf(fp, "\n\nData");
    for (int i = 0; i < len; i++)
    {
        if (i % 8 == 0 && i != len)
            fprintf(fp, "\n%d:", (i + tail + 1) * 4 + 64);
        fprintf(fp, "\t%d", DATA[i]);
        
    }
    fprintf(fp, "\n\n");
    return 0;
}

int main(int argc, char const *argv[])
{
    //print disassembly
    int turn = 0;
    fstream sample("sample.txt");
    FILE *fp = fopen("disassembly.txt", "a+");
    string mipsString[M];
    do{
        getline(sample, mipsString[turn]);
        for (int i = 0; i < 32; i++)
        {
            fprintf(fp, "%c", mipsString[turn][i]); 
            if (i == 5 || i == 10 || i == 15 || i == 20 || i == 25)
                fprintf(fp, " ");
        }
        //print memloc
        fprintf(fp, "\t%d\t", turn * 4 + 64);
    } while (!printDisassembly(mipsString[turn++], 0, 0, fp));
    
    //save the tail of instruction
    tailofinstruction = turn-1;

    //create a temp string to save number
    string mipstmp;
    while (sample.peek() != EOF) // when not reach the EOF of file
    {
        //read a line of number
        getline(sample, mipstmp);
        DATA[turn - tailofinstruction - 1] = truevalue(mipstmp);
        for (int i = 0; i < 32;i++)
            fprintf(fp, "%c", mipstmp[i]);
        fprintf(fp, "\t%d", turn++ * 4 + 64);
        fprintf(fp, "\t%d\n", truevalue(mipstmp));
    }
    int DATAlen = turn - tailofinstruction - 1;
    fclose(fp);


    //print simulation
    int pc = 0;
    int endflag = 0;
    int nextins = 0;
    int cycle = 0;
    FILE *fp1 = fopen("simulation.txt", "a+");
    while (endflag == 0)
    {
        int tmp = nextins;
        ++cycle;
        //do instruction
        nextins = printDisassembly(mipsString[tmp], 1, tmp, fp1);

        //Start print
        fprintf(fp1,"--------------------\n");
        fprintf(fp1,"Cycle:%d\t%d\t", cycle, tmp*4+64);
        //print instruction and set endflag
        if(printDisassembly(mipsString[tmp], 0, 0, fp1) == -1){
            endflag = -1;
        }
        printRegisters(DATAlen, tailofinstruction, fp1);

    }
    fclose(fp1);
    return 0;
}
