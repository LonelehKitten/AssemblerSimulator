#ifndef LOADER_H
#define LOADER_H

#include <unordered_map>
#include <vector>
#include <string>

#include "LoaderTables.h"

#include "../Utils.h"

class Loader
{
private:
    std::unordered_map<std::string, Relocation *> RelocationMap;

    std::vector<byte> memory;

public:
    Loader();
};



/*
Mapa de Relocação
-Possui um campo após o texto original, onde contém a informação de quais posições devem ser realocadas, não deve ser levado esse campo 
para o produto final.
-Possui informações como tamanho, posição inicial do segmento.

Vetor de Transferência
-Uso de uma área de dados global (para todos os segmentos) – referências externas fica restrita a subrotinas
-Uso de vetor de transferência de nomes simbólicos de subrotinas (realizado pelo montador)
*/

#endif // LOADER_H