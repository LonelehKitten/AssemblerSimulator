#include "template.h"

using namespace std;

int main(int argc, char **argv)
{
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
    vector<string> params;//{"Mem1","Mem2"};
    params.push_back("Mem1");
    params.push_back("Mem2");
    Semantic A1 = Semantic("SomaMem MACRO Mem1, Mem2","SomaMem",iMACRO,&params); //SomaMem MACRO Mem1, Mem2
    Semantic A2 = Semantic("",iMOV); // mov AX,Mem1
    Semantic A3 = Semantic("1",iPUSH); // push AX
    Semantic A4 = Semantic("1",iMOV); // mov AX,Mem2
    Semantic A5 = Semantic("1",iMOV); // mov DX,AX
    Semantic A6 = Semantic("1",iPOP); // pop AX
    Semantic A7 = Semantic("1",iADD); // add AX,DX
    Semantic A8 = Semantic("1",iMOV); // mov Mem1,AX
    Semantic A9 = Semantic("1",iENDM); // ENDM
    vector<Semantic *> lines{
        &A1,
        &A2,
        &A3,
        &A4,
        &A5,
        &A6,
        &A7,
        &A8,
        &A9
    };
    //lines.push_back(&Teste);

    Macro currentMacro;
    for (int i = 0; i < lines.size(); i++)
    {
        Semantic * Item = lines[i];
        if(Item->getType() == iMACRO){
            currentMacro = Macro();
         //   MDefinicao();
            cout << "iMACRO" << endl;
        }else if(Item->getType() == iENDM){
            cout << "iENDM" << endl;
         //   MNormal();
        }else if(Item->getType() == iMACROCALL){
            cout << "iMACROCALL" << endl;
           // IterarMacroCall();
        }else{
            cout << Item->getType() << endl;
        }
    }
};

/*
.
AAADDD MACROTESTE MACRO p1, p2, p3...
LABEL NOME_DA_MACRO MACRO PARAMETROS
*/

// mover esse arquivo para uma pasta chamada 'test' dentro do arquivo bin depois