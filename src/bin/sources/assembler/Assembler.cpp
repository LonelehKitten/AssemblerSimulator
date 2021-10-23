#include "Assembler.h"
#include "../InterfaceBus.h"
#define File std::vector<Semantic *>

Assembler::Assembler(std::vector<Semantic *> * lines) :
    lines(lines)
{}

/** Coisas de Semantic que precisamos pegar:
 * getOperands - não entendi como funciona, talvez não seja o que precisamos
 * getLabel - Só tem o getName de uma subclasse Semantic Label
 * a subclasse pra pseudoinstrução ORG - não está implementada
 * (talvez tenha mais, mas por enquanto foi o que notamos)
 ** Coisas de Semantic prontas:
 * getType - retorna um enum, com base nele podemos pegar o código de máquina referente
 */

std::vector<unsigned char> Assembler::assembleByteCode(Semantic * line)
{
    std::vector<unsigned char> bytecode;
    //bytecode.push_back(bytecode(line->getType()));
    //bytecode.push_back(bytecode(line->getOperands()));
    return bytecode;
}

void Assembler::assembleStep2()
{
    int locationCounter = 0;
    int startAddress;
    for(line = this->lines->begin(); line != this->lines->end(); ++line)
    {
        opcode = line->getType();
        //operand = line->getOperand();     //pegar depois

        switch(opcode):
            case 'EQU':
                // NADA
            break;

            case 'ORG':
                //LC = GETOPERANDVALUE (operand)
            break;

            case 'END':
                //startAddress = GETOPERANDVALUE(operand)
                //ASSEMBLECLOSINGCODE(startAdress);
            break;

            case default:
                if(opcode == 'DC' || opcode == 'DS'){
                    //entry = findtable(POT, opcode);
                }else{
                    //entry = findtable(MOT, opcode);
                }

                //length = getinstructionsize(entry, operand);
                code = assembleByteCode(line);
                //code = gerenatemachinecode(entry, operand);

                //Concatena os vetores code e this->assembleCode
                //assemblemachinecode(locationCounter, code);    //Usa this->assembleCode, std::vector<unsigned char>
                locationCounter = locationCounter + length;

            break;
    }

    return;
}

void Assembler::assemble()
{
    int locationCounter = 0;
    int label;                      //#### template ####
    int opcode;                     //#### template ####
    int operand;                    //#### template ####
    int* entry = NULL;              //#### template ####
    int value, length;
    
    //file == *this->lines
    for(File::iterator line = this->lines->begin(); line != this->lines->end(); ++line)
    {
        //label = line->getLabel();         //falta pegar da linha
        opcode = line->getType();
        //operand = line->getOperand();     //pegar depois

        // ** entry <- FINDTABLE(POT, opcode) **
        //entry = tabela_de_pseudoinstrucoes;

        if(entry != NULL)   //Se eh pseudoinstrucao
        {
            switch (opcode) 
            {
                case 'ORG':
                    // ** LC <-GETOPERANDVALUE (operand) ** 
                    //locationCounter = line->getValue();
                break;

                case 'END':
                    assembleStep2();
                break;

                case default:
                    if(opcode == 'EQU')
                    {
                        // ** value <- GETOPERANDVALUE (operand) ** 
                        //value = (operand);
                        length = 0;
                    }else
                    {
                        value = locationCounter;
                        //lenght(entry, operand);
                    }

                    if(label != NULL) 
                    {
                        // ** INSERTTABLE(ST, label, value) ** 
                        //(ST, label, value); //usar this->symbolTable (unordered_map) para ST
                        locationCounter = locationCounter + lenght;
                    }
                break;

            }
        }
        else
        {
            // ** entry <- FINDTABLE(MOT, opcode) **
            //entry = tabela_de_instrucoes_maquina;

            if(entry != NULL){
                // ** length <- GETINSTRUCTIONSZE(entry, operand) **
                length = (entry, operand);
                
                if(label != NULL)
                    //INSERTTABLE (ST, label, LC)
                    //(ST, label, LC);        //usar this->symbolTable (unordered_map) para ST
                    
                locationCounter = locationCounter + lenght;
            }
            else 
            {
                //error message, invalid opcode     
            }
        }
        
    }
    
    return; //Retornar talvez ja aqui o código de máquina
}