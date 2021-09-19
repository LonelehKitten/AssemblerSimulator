#include "Assembler.h"
#include "../analyzer/RecognitionManager.h"

int main() {

    RecognitionManager * rm = new RecognitionManager();

    std::vector<Semantic *> * s_arr = rm->analyze(

        std::string("Dados SEGMENT\n") +
        std::string("Var1 DW 5\n") +
        std::string("Var2 DW 8\n") +
        std::string("Var3 DW 3\n") +
        std::string("Dados ENDS\n") +

        std::string("Codigo SEGMENT\n") +
        std::string("ASSUME CS: Codigo\n") +
        std::string("ASSUME DS: Dados\n") +

        std::string("SomaMem MACRO Mem1, Mem2 ;; Soma duas variáveis\n") +
        std::string("mov  AX,Mem1\n") +
        std::string("push AX\n") +
        std::string("mov  AX,Mem2\n") +
        std::string("mov  DX,AX\n") +
        std::string("pop  AX\n") +
        std::string("add  AX,DX\n") +
        std::string("mov  Mem1,AX\n") +
        std::string("ENDM\n") +

        std::string("Inicio:\n") +
        std::string("mov AX, Dados\n") +
        std::string("mov DS, AX\n") +
        std::string("mov AX,2\n") +
        std::string("mov AX, DX\n") +
        std::string("mul DX\n") + 
        std::string("SomaMem Var1,V\n") +
        std::string("mov AX, Var1\n") +
        std::string("add AX, DX\n") +
        std::string("mov Var2, AX\n") +
        std::string("SomaMem Var1, 3\n") +
        std::string("SomaMem Var1, Var3\n") +
        std::string("CODIGO ENDS\n") +
        std::string("END Inicio")
    );

    std::cout << s_arr->size() << std::endl;

    for(int i = 0; i < (int) s_arr->size(); i++)
    {
        std::cout << "l: " << s_arr->at(i)->getLine() << ", t: " << s_arr->at(i)->getType() << std::endl;
    }

    return 1;
}

/*
.
AAADDD MACROTESTE MACRO p1, p2, p3...
LABEL NOME_DA_MACRO MACRO PARAMETROS
*/

// mover esse arquivo para uma pasta chamada 'test' dentro do arquivo bin depois