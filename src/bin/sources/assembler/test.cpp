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

/*
.
AAADDD MACROTESTE MACRO p1, p2, p3...
LABEL NOME_DA_MACRO MACRO PARAMETROS
*/

// mover esse arquivo para uma pasta chamada 'test' dentro do arquivo bin depois