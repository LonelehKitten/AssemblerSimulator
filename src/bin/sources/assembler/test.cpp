<<<<<<< HEAD
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
        std::string("mul AX\n") + // mudar ax para dx
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
=======
#include "template.h"

using namespace std;

std::vector<macroDef *> macroTable;
std::vector<label *> labelTable;

int main(int argc, char **argv)
{
    std::vector<bool> macroFound;

    //std::vector<Semantic *> lines = rm->getLines();
    //Semantic add = Semantic("1", iADD);
    //cout << add.getLine() << endl;

    /*
        SomaMem MACRO Mem1, Mem2
        mov AX,Mem1
        push AX
        mov AX,Mem2
        mov DX,AX
        pop AX
        add AX,DX
        mov Mem1,AX
        ENDM
    */

    /*
    ax = operador destino
    dx = fonte

    */
    vector<string> params; //{"Mem1","Mem2"};
    params.push_back("Mem1");
    params.push_back("Mem2");
    Macro A1 = Macro("SomaMem MACRO Mem1, Mem2", "SomaMem", &params); //SomaMem MACRO Mem1, Mem2
    //cout << "Nome" << A1.getName() << endl;

    vector<string> MemovParamsm1;
    MemovParamsm1.push_back("AX");
    MemovParamsm1.push_back("Mem1");
    Mov A2 = Mov("mov AX, Mem1", &MemovParamsm1); // mov AX,Mem1

    Push A3 = Push("push AX"); // push AX

    vector<string> MemovParamsm2;
    MemovParamsm2.push_back("AX");
    MemovParamsm2.push_back("Mem2");
    Mov A4 = Mov("mov AX, Mem2", &MemovParamsm2); // mov AX,Mem2

    vector<string> MemovParamsDX;
    MemovParamsDX.push_back("DX");
    MemovParamsDX.push_back("AX");
    Mov A5 = Mov("mov DX, AX", &MemovParamsDX); // mov DX,AX

    Pop A6 = Pop("pop AX");          // pop AX
    Add A7 = Add("add AX,DX", "DX"); // add AX,DX

    vector<string> MemovParamsM1AX;
    MemovParamsM1AX.push_back("AX");
    MemovParamsM1AX.push_back("Mem1");
    Mov A8 = Mov("mov Mem1, AX", &MemovParamsM1AX); // mov Mem1,AX

    EndM A9 = EndM("ENDM"); // ENDM

    vector<Semantic *> lines{
        &A1,
        &A2,
        &A3,
        &A4,
        &A5,
        &A6,
        &A7,
        &A8,
        &A9};

    for (int i = 0; i < lines.size(); i++)
    {
        Semantic *Item = lines[i];
        cout << Item->getLine() << endl;
        if (Item->getType() == iMACRO)
        {
            if (macroFound.size() == 0)
            {
                
                Macro * itemMacro = (Macro *)(lines[i]);
                macroTable.push_back(new macroDef(itemMacro->getName(), *(itemMacro->getParams())));
                macroFound.push_back(true);
            }
            else
            {
                macroFound.push_back(true);
            }
        }
        
        else if (Item->getType() == iENDM)
        {
            //   MNormal();
            if (macroFound.size() == 0)
            {
                // Erro
                cout << "Error ";
            }else{
                macroFound.pop_back();
            }

        }
        else if (Item->getType() == iMACROCALL)
        {
            // IterarMacroCall();
        }
        else if (macroFound.size() == 0){
            cout << "Error ";
            // Error
        }else
        {
            macroTable.back()->setText(Item->getLine());
        }
    }

    cout << "Total: " << macroTable.size() << endl << endl;
    for (int i = 0; i < macroTable.size(); i++)
    {
        macroDef * item = macroTable[i];
        cout << item->getText() << endl;
    }

};
>>>>>>> 19d9fba70ef35e0a348157f8da31e081225bb791

/*
.
AAADDD MACROTESTE MACRO p1, p2, p3...
LABEL NOME_DA_MACRO MACRO PARAMETROS
*/

// mover esse arquivo para uma pasta chamada 'test' dentro do arquivo bin depois