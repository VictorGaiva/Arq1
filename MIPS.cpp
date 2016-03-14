#include "MIPS.h"
/*
* 	Construtor do simulador. Aloca a memória RAM Inicializa as variaves com valores nulos
*/
MIPS::MIPS(){
	//Ram
	this->Ram = malloc(RAM_SIZE);
	memset(this->Ram, 0, RAM_SIZE);

	//Regs
    memset(this->Reg, 0, sizeof(int)*32);

	this->PC 	= 0;
	this->HILO 	= 0;
	this->HI 	= 0;
	this->LO 	= 0;
}
MIPS::~MIPS(){
    free(this->Ram);
}
/*
*	Recebe como parametro um ponteiro para o arquivo de entrada contendo o código à ser simulado,
*		então lê e decodifica cada linha do arquivo para sua instrução equivalente, salvando a lista
*		na variável "Code"
*/
int MIPS::LoadCode(std::fstream *File){
	std::string Linha = "";
	struct Instruction Ins;

	while (!(File->eof()))
	{
		//Para cada linha
		std::getline(*File, Linha);

		//Verifica se é um comentario
		if(Linha[0] == '#' || Linha[0] == '\0')
			continue;

		//Decodifique a instrucao
		Ins = this->Decode(Linha);

		//E salve na lista
		if(Ins.OK)
			this->Code.push_back(Ins);
		//else
			//Instrução não indentificada
	}
	return 0;
}

/*
*	Recebe uma string como entrada contendo a instrução, recupera um indentificador e faz um parse nos
*		operandos, retornando os valores em uma estrutura.
*/
struct Instruction MIPS::Decode(std::string Linha){
	struct Instruction Ret;
	Ret.OK = true;
	switch(Linha[0]){
		case 'a':
			if(Linha[1] == 'd'){
				if(Linha[3] != 'i'){		//add V
					Ret.ID 				= 'a';

					sscanf(Linha.c_str(), "add $%d , $%d , $%d", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
				}
				else if(Linha[4] != 'u'){	//addi V
					Ret.ID 				= 'b';

					sscanf(Linha.c_str(), "addi $%d , $%d , %x", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
				}
				else{						//addiu V
					Ret.ID 				= 'c';

					sscanf(Linha.c_str(), "addiu $%d, $%d, %x", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
				}
			}
			else if(Linha[3] == 'i'){		//andi V
				Ret.ID 				= 'd';

				sscanf(Linha.c_str(), "andi $%d, $%d, %x", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
			else{							//and V
				Ret.ID 				= 'e';

				sscanf(Linha.c_str(), "and $%d, $%d, $%d", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
		break;

		case 'b':
			if(Linha[1] == 'e'){			//beq V
				Ret.ID 				= 'f';

				sscanf(Linha.c_str(), "beq $%d, $%d, %x", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
			else{							//bne V
				Ret.ID 				= 'g';

				sscanf(Linha.c_str(), "bne $%d, $%d, %x", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
		break;

		case 'd':							//div V
			Ret.ID 					= 'h';

			sscanf(Linha.c_str(), "div $%d, $%d", &(Ret.RegDestino), &(Ret.Operando1));
		break;

		case 'j':
			if(Linha[1] == 'a'){			//jal V
				Ret.ID 				= 'i';

				sscanf(Linha.c_str(), "jal %d", &(Ret.Operando1));
			}
			else if(Linha[1] == 'r'){		//jr V
				Ret.ID 			    = 'j';

				sscanf(Linha.c_str(), "jr $%d", &(Ret.Operando1));
			}
			else{							//j V
				Ret.ID 				= 'k';

				sscanf(Linha.c_str(), "j %d", &(Ret.Operando1));
			}
		break;

		case 'l':
		if(Linha[1] == 'b'){				//lbu V
			Ret.ID 				= 'l';

            sscanf(Linha.c_str(), "lbu $%d, %d($%d)", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
		}
		else if(Linha[1] == 'h'){			//lh V
			Ret.ID 				= 'm';

            sscanf(Linha.c_str(), "lh $%d, %d($%d)", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
		}
		else if(Linha[1] == 'u'){			//lui V
			Ret.ID 				= 'n';

            sscanf(Linha.c_str(), "lui $%d, %x", &(Ret.RegDestino), &(Ret.Operando1));
		}
		else{								//lw V
			Ret.ID 				= 'o';

            sscanf(Linha.c_str(), "lw $%d, %d($%d)", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
		}
		break;

		case 'm':
		if(Linha[1] == 'u'){				//mult V
			Ret.ID 				= 'p';

            sscanf(Linha.c_str(), "mult $%d, $%d", &(Ret.Operando1), &(Ret.Operando2));
		}
		else if(Linha[2] == 'h'){			//mfhi V
			Ret.ID 				= 'q';

            sscanf(Linha.c_str(), "mfhi $%d", &(Ret.RegDestino));
		}
		else{								//mflo V
			Ret.ID 				= 'r';

            sscanf(Linha.c_str(), "mflo $%d", &(Ret.RegDestino));
		}
		break;

		case 'o':
		if(Linha[2] == 'i'){				//ori V
			Ret.ID 				= 's';

            sscanf(Linha.c_str(), "ori $%d, $%d, %x", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
		}
		else{								//or V
			Ret.ID 				= 't';

            sscanf(Linha.c_str(), "or $%d, $%d, $%d", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
		}
		break;

		case 's':
			if(Linha[1] == 'b'){			//sb V
                Ret.ID 				= 'u';

                sscanf(Linha.c_str(), "sb $%d, %x($%d)", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
			else if(Linha[1] == 'h'){		//sh V
                Ret.ID 				= 'v';

                sscanf(Linha.c_str(), "sh $%d, %x($%d)", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
			else if(Linha[1] == 'r'){		//srl V
				Ret.ID 				= 'w';
				sscanf(Linha.c_str(), "srl $%d, $%d, %x", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
			else if(Linha[1] == 'u'){		//sub V
                Ret.ID 				= 'x';

                sscanf(Linha.c_str(), "sub $%d , $%d , $%d", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
			else if(Linha[1] == 'w'){		//sw V
                Ret.ID 				= 'y';

                sscanf(Linha.c_str(), "sw $%d, %x($%d)", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
			else if(Linha[2] == 'l'){		//sll V
				Ret.ID 				= 'z';
				sscanf(Linha.c_str(), "sll $%d, $%d, %x", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
			else if(Linha[3] == 'i'){		//slti V
                Ret.ID 				= 'A';

                sscanf(Linha.c_str(), "slti $%d, $%d, %x", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
			else{							//slt V
                Ret.ID 				= 'B';

                sscanf(Linha.c_str(), "slt $%d , $%d , $%d", &(Ret.RegDestino), &(Ret.Operando1), &(Ret.Operando2));
			}
			break;
		default:
		std::cout << "Comando nao reconhecido:\n\"" << Linha << "\"\n";
		Ret.OK = false;
	}
	return Ret;
}

/*
*	Executa as instruções contidas na lista de instruções Code.
*	Interpreta o indentificador da instrução e realiza as operações de acordo junto com os operandos passados.
* 	Caso a instrução não seja indentificada, gera um mensagem de aviso e finaliza a execução do código por inconsistencia de dados.
*/
int MIPS::Exec(){
	int OP1, OP2;						//Operadores
	unsigned int U_OP1, U_OP2;			//Operadores sem sinal

	struct Instruction Ins_Atual;
	int Tam_Code = this->Code.size();

	while(this->PC/4 < Tam_Code){
		//Passo1: Carregue a instrução com o valor do  PC
		Ins_Atual = this->Code[this->PC/4];

		//Passo2: Execute a instrução e atualize os valores dos registradores e da memória
		switch(Ins_Atual.ID){
			case 'a'://add V
				OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Reg[(int)Ins_Atual.Operando2];
				this->Reg[(int)Ins_Atual.RegDestino] = OP1 + OP2;

				this->PC += 4;			//PC++
			break;

			case 'b'://addi V
				OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Ins_Atual.Operando2;
				this->Reg[(int)Ins_Atual.RegDestino] = OP1 + OP2;

				this->PC += 4;			//PC++
			break;

			case 'c'://addiu V
				U_OP1 = (unsigned int)Reg[(int)Ins_Atual.Operando1];
				U_OP2 = (unsigned int)Ins_Atual.Operando2;
				this->Reg[(int)Ins_Atual.RegDestino] = U_OP1 + U_OP2;

				this->PC += 4;			//PC++
			break;

			case 'd'://andi V
				OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Ins_Atual.Operando2;
				this->Reg[(int)Ins_Atual.RegDestino] = OP1 & OP2;

				this->PC += 4;			//PC++
			break;

			case 'e'://and V
				OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Reg[(int)Ins_Atual.Operando2];
				this->Reg[(int)Ins_Atual.RegDestino] = OP1 & OP2;

				this->PC += 4;			//PC++
			break;

			case 'f'://beq V
				OP1 = Reg[(int)Ins_Atual.RegDestino];
				OP2 = Reg[(int)Ins_Atual.Operando1];
				if(OP1 == OP2)
					this->PC += (Ins_Atual.Operando2 << 2);
				else
					this->PC += 4;		//PC++
			break;

			case 'g'://bne V
				OP1 = Reg[(int)Ins_Atual.RegDestino];
				OP2 = Reg[(int)Ins_Atual.Operando1];
				if(OP1 != OP2)
					this->PC += (Ins_Atual.Operando2 << 2);
				else
					this->PC += 4;		//PC++
			break;

			case 'h'://div V
				OP1 = Reg[(int)Ins_Atual.RegDestino];
				OP2 = Reg[(int)Ins_Atual.Operando1];

				this->LO = OP1/OP2;
				this->HI = OP1%OP2;

				this->HILO = (long long int)(((long long int) this->HI << 32) | this->LO);


				this->PC += 4;		//PC++
			break;

			case 'i'://jal V
                OP1 = Ins_Atual.Operando1;

                this->Reg[31] = this->PC+4;//+8?
                this->PC = (this->PC & 0xF0000000) | (OP1 << 2);
			break;

			case 'j'://jr V
                OP1 = this->Reg[Ins_Atual.Operando1];

                this->PC = OP1;
			break;

			case 'k'://j V
                OP1 = Ins_Atual.Operando1;

                this->PC = (this->PC & 0xF0000000) | (OP1 << 2);
			break;

			case 'l'://lbu V
				OP1 = Ins_Atual.Operando1;
				OP2 = this->Reg[(int)Ins_Atual.Operando2];

				U_OP1 = *((char*) this->Ram + (OP1 + OP2));

				this->Reg[(int)Ins_Atual.RegDestino] =(unsigned int) U_OP1;

				this->PC += 4;			//PC++
			break;

			case 'm'://lh V
				OP1 = Ins_Atual.Operando1;
				OP2 = this->Reg[(int)Ins_Atual.Operando2];

				//Ponteiro para o começo da RAM somado aos offsets. Cast para ponteiro de short e entao o conteudo desse ponteiro.
				OP1 = *((short*) this->Ram + (OP1 + OP2));

				this->Reg[(int)Ins_Atual.RegDestino] = OP1;

				this->PC += 4;			//PC++
			break;

			case 'n'://lui V
				OP1 = Ins_Atual.Operando1;

				this->Reg[(int)Ins_Atual.RegDestino] = OP1 << 16;

				this->PC += 4;			//PC++
			break;

			case 'o'://lw V
				OP1 = Ins_Atual.Operando1;
				OP2 = this->Reg[(int)Ins_Atual.Operando2];

				OP1 = *((int*) this->Ram + (OP1 + OP2));

				this->Reg[(int)Ins_Atual.RegDestino] = OP1;

				this->PC += 4;			//PC++
			break;

			case 'p'://mult V
                OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Reg[(int)Ins_Atual.Operando2];
				this->HILO = OP1 * OP2;
				this->HI = (this->HILO & 0xFFFFFFFF00000000);
				this->LO = (this->HILO & 0x00000000FFFFFFFF);

				this->PC += 4;			//PC++
			break;

			case 'q'://mfhi V
                this->Reg[(int)Ins_Atual.RegDestino] = this->HI;

                this->PC += 4;			//PC++
			break;

			case 'r'://mflo V
                this->Reg[(int)Ins_Atual.RegDestino] = this->LO;

                this->PC += 4;			//PC++
			break;

			case 's'://ori V
                OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Ins_Atual.Operando2;
				this->Reg[(int)Ins_Atual.RegDestino] = OP1|OP2;

				this->PC += 4;			//PC++
			break;

			case 't'://or V
                OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Reg[(int)Ins_Atual.Operando2];
				this->Reg[(int)Ins_Atual.RegDestino] = OP1|OP2;

				this->PC += 4;			//PC++
			break;

			case 'u'://sb V
				OP1 = (this->Reg[(int)Ins_Atual.RegDestino] & 0x000000FF);
				OP2 = Ins_Atual.Operando1 + this->Reg[(int)Ins_Atual.Operando2];

				*((char*) this->Ram + OP2) = (char) OP1;
				this->PC += 4;			//PC++
			break;

			case 'v'://sh V
				OP1 = (this->Reg[(int)Ins_Atual.RegDestino] & 0x0000FFFF);
				OP2 = Ins_Atual.Operando1 + this->Reg[(int)Ins_Atual.Operando2];

				*((short*) this->Ram + OP2) = (short) OP1;
				this->PC += 4;			//PC++
			break;

			case 'w'://srl V
				OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Ins_Atual.Operando2;

				this->Reg[(int)Ins_Atual.RegDestino] = OP1 >> OP2;
				this->PC += 4;			//PC++
			break;

			case 'x'://sub V
                OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Reg[(int)Ins_Atual.Operando2];
				this->Reg[(int)Ins_Atual.RegDestino] = OP1 - OP2;

				this->PC += 4;			//PC++
			break;

			case 'y'://sw V
				OP1 = this->Reg[(int)Ins_Atual.RegDestino];
				OP2 = Ins_Atual.Operando1 + this->Reg[(int)Ins_Atual.Operando2];

				*((int*) this->Ram + OP2) = OP1;
				this->PC += 4;			//PC++
			break;

			case 'z'://sll V
				OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Ins_Atual.Operando2;

				this->Reg[(int)Ins_Atual.RegDestino] = OP1 << OP2;
				this->PC += 4;			//PC++
			break;

			case 'A'://slti V
                OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Ins_Atual.Operando2;
				this->Reg[(int)Ins_Atual.RegDestino] = (OP1 < OP2)? 1 : 0;

				this->PC += 4;			//PC++
			break;

			case 'B'://slt V
                OP1 = Reg[(int)Ins_Atual.Operando1];
				OP2 = Reg[(int)Ins_Atual.Operando2];
				this->Reg[(int)Ins_Atual.RegDestino] = (OP1 < OP2)? 1 : 0;

				this->PC += 4;			//PC++
			break;

			//Inconsistencia de dados
			default:
				std::cout << "Dados invalidos.\n";
				return -1;
		}
	}

	//Solução de contorno para manter o valor do registrador $0 sempre como 0
	this->Reg[0] = 0;

	return 0;
}

void MIPS::PrintState(){
	std::cout << "PC: " << this->PC << "\n";
	std::cout << "HILO: " << this->HILO << "\n";

	for(int i = 0; i < 32; i++){
		std::cout << "Reg" << i << ": " << this->Reg[i] << "\n";
	}
}

/*
*	Retorna o ultimo valor de PC
*/
int MIPS::GetPC(){
	return this->PC;
}

/*
*	Faz um dump da RAM em dump.bin
*/
void MIPS::DumpRAM(){
	std::fstream outp;

	//Abre arqivo
	outp.open("dump.bin", std::fstream::out | std::fstream::trunc | std::fstream::binary);

	//Escreve RAM no arquivo
	if(outp.is_open())
		outp.write((const char*)(this->Ram), RAM_SIZE);
	else
		std::cout << "Arquivo de saida nao pode ser aberto.";

	//Fecha arquivo
	outp.close();
}