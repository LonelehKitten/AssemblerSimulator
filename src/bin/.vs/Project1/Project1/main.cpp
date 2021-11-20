#include <iostream>

#include "../../../sources/analyzer/RecognitionManager.h"
#include "../../../sources/assembler/Assembler.h"
#include "../../../sources/GlobalSettings.h"

int main() {

    std::string code = "";
    
        code += "program SEGMENT\n";
        code += "ASSUME CS: program\n";
        code += "ASSUME DS: program\n";
        code += "main:\n";
        code += "add ax, 100\n";
        code += "loop1:\n";
        code += "sub ax, 10\n";
        code += "jnz loop1\n";
        code += "program ENDS\n";
        code += "END main";
    /*
    code += "data SEGMENT\n";
    code += "max EQU 10\n";
    code += "unit2 DW 1\n";
    code += "unit DW 1\n";
    code += "data ENDS\n";
    code += "program SEGMENT\n";
    code += "ASSUME CS: program\n";
    code += "ASSUME DS: data\n";
    code += "main:\n";
    code += "mov ax, data\n";
    code += "mov ds, ax\n";
    code += "add ax, max\n";
    code += "loop1:\n";
    code += "sub ax, unit\n";
    code += "jnz loop1\n";
    code += "loop2:\n";
    code += "program ENDS\n";
    code += "END main";
    */
    RecognitionManager* recognitionManager = new RecognitionManager();

    std::vector<Semantic*>* semantics = recognitionManager->analyzeText(code);
    LOG("assemble and run: analyzer")
        //return;
        Assembler* assembler = new Assembler(semantics);
    assembler->init(true);
    LOG("assemble and run: preprocess")
        assembler->assemble(0);


    return 1;
}