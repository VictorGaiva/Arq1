#include "MIPS.h"

int main(int argc, char *argv []){
    class MIPS  Processador;
    std::fstream    input;

    //Verifica entrada
    if(argc != 2){
        std::cout << "Uso:\n$ " << argv[0] << " <input.asm>\n" << "ATENCAO:Numeros no arquivo de entrada devem estar escrito em hexadecimal! (I.E. 0x0011)";
        return 0;
    }
    
    //Tenta abrir arquivo
    input.open(argv[1], std::fstream::in);
    if(!(input.is_open())){
        std::cout << "Arquivo " << argv[1] << " nao encontrado!";
        return 0;
    }
    
    //Passo1: Inicialize o processador
    Processador.LoadCode(&input);

    //Passo2: Execute programa
    if(Processador.Exec() == -1){
        std::cout << "Erro na execucao do codigo na instrucao " << Processador.GetPC() << ".\n";
        return 0;
    }
        
    //Passo3: Imprima o ultimo estado dos registradores e da RAM
    Processador.PrintState();
    Processador.DumpRAM();
    
    //Fecha arquivo de entrada
    input.close();

    return 0;
}