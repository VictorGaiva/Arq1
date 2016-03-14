#ifndef _MIPS_
#define _MIPS_
//Alguns includes
#include <iostream>
#include <fstream>

#include <string.h>

#include <vector>
#include <stdlib.h>

#define RAM_SIZE 8192

struct Instruction{
	char ID;                //Indentificador da instrução

	//Data
	char RegDestino;        //Registrador de destino
	int Operando1;        	//Operando1
	int Operando2;        	//Operando2

	//Flag
	bool OK;
};

class MIPS{
private:
	//Ram
	void* Ram;			//Vetor RAM

	//Regs
	int Reg[32];		//Vetor com os 32 registradores de uso 
	int PC;				//Registrador PC
	long long int HILO; //Registrador HILO
	int HI;				//Registrador HI
	int LO;				//Registrador LO
	
	std::vector<struct Instruction> Code;

public:
	int LoadCode(std::fstream *);
	int Exec();
	void PrintState();
	struct Instruction Decode(std::string);
    void DumpRAM();

	int GetPC();
	
    MIPS();
    ~MIPS();

};



#endif
