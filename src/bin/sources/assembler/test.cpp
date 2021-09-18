#include "template.h"

using namespace std;
<<<<<<< Updated upstream
/*
template <class T>
Vector<T> */
=======
>>>>>>> Stashed changes

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
    Semantic A1 = Macro("SomaMem MACRO Mem1, Mem2","SomaMem",&params); //SomaMem MACRO Mem1, Mem2

    vector<string> MemovParamsm1;
    MemovParamsm1.push_back("AX");
    MemovParamsm1.push_back("Mem1");
    Semantic A2 = Mov("mov AX, Mem1", &MemovParamsm1); // mov AX,Mem1
    
    Semantic A3 = Push("push AX"); // push AX
    
    vector<string> MemovParamsm2;
    MemovParamsm2.push_back("AX");
    MemovParamsm2.push_back("Mem2");
    Semantic A4 = Mov("mov AX, Mem2", &MemovParamsm2); // mov AX,Mem2

    vector<string> MemovParamsDX;
    MemovParamsDX.push_back("DX");
    MemovParamsDX.push_back("AX");
    Semantic A5 = Mov("mov DX, AX", &MemovParamsDX); // mov DX,AX

    Semantic A6 = Pop("pop AX"); // pop AX
    Semantic A7 = Add("add AX,DX", "DX"); // add AX,DX

    vector<string> MemovParamsM1AX;
    MemovParamsM1AX.push_back("AX");
    MemovParamsM1AX.push_back("Mem1");
    Semantic A8 = Mov("mov Mem1, AX", &MemovParamsM1AX); // mov Mem1,AX

    Semantic A9 = EndM("ENDM"); // ENDM
    
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

<<<<<<< Updated upstream
 //   Macro currentMacro;
=======
    

>>>>>>> Stashed changes
    for (int i = 0; i < lines.size(); i++)
    {
        Semantic * Item = lines[i];
        if(Item->getType() == iMACRO){
   //         currentMacro = Macro();
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