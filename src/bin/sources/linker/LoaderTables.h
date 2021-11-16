#ifndef LOADER_TABLES_H
#define LOADER_TABLES_H

#include "../assembler/SegmentDef.h"

struct Relocation
{
    //SegmentDef * segment;
    int size;
    int initPos;

    Relocation(SegmentDef *);
};

#endif // LOADER_TABLES_H