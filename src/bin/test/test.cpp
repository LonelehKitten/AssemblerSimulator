#include <iostream>

#include "../sources/analyzer/RecognitionManager.h"
#include "../sources/assembler/Assembler.h"
#include "../sources/GlobalSettings.h"
#include "../sources/linker/Linker.h"

int main()
{

    std::vector<std::string> files; //é o filas
    std::string code = "";
    /*
        code += "program SEGMENT\n";
    */
    // main.asm
    code += "NAME nsei\n";
    code += "EXTRN altura:ABS\n";
    code += "program SEGMENT\n";
    code += "ASSUME CS: program\n";
    code += "ASSUME DS: program\n";
    code += "success DW 33\n";
    code += "main:\n";
    code += "add ax, altura\n";
    code += "program ENDS\n";
    code += "END main";

    files.emplace_back(code);
    code = "";

    // teste.asm
    code += "NAME teste\n";
    code += "PUBLIC altura\n";
    code += "temp SEGMENT\n";
    code += "ASSUME CS : temp\n";
    code += "ASSUME DS : temp\n";
    code += "altura EQU 2 *(0101b - 0DA7h)\n";
    code += "sum PROC\n";
    code += "add ax, dx\n";
    code += "ret;\n";
    code += "sum ENDP\n";
    code += "temp ENDS\n";
    code += "END";

    files.emplace_back(code);

                //

    RecognitionManager * recognitionManager = new RecognitionManager();
    std::vector<Assembler *> * listAssemblers = new std::vector<Assembler *>();
    Assembler * assembler;
    SymbolTable * globalSymbols = new SymbolTable();
    bool error = false;
    int programCounter = 0;

    std::cout << "build and run" << std::endl;

    for (unsigned int i = 0; i < files.size(); i++) {
        std::cout << "File: " << files.at(i) << std::endl;
    }
    
    
    for(unsigned int i = 0;i < files.size(); i++){
        std::vector<Semantic *> *semantics = recognitionManager->analyzeFile(files.at(i));
        assembler = new Assembler(semantics, programCounter,globalSymbols);
        assembler->init(true);
        if(assembler->assemble(false) == 0) {
            programCounter = assembler->getProgramCounter();
            globalSymbols = assembler->getGlobalSymbols();
            listAssemblers->emplace_back(assembler);
        }else{
            error = true;
            break;
        }
        
    }
    
    std::cout << "Total Processado: " << listAssemblers->size() << std::endl;
    if(error){
        std::cout << "Error no assemble " << std::endl;
    }else{
        Linker linker(listAssemblers, programCounter, globalSymbols);

        if(!linker.link()) {
            std::cout << "Erro no linker" << std::endl;
        }
        
        // Linker
    }
    return 1;
}
