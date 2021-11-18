#include "LoaderTables.h"

Relocation::Relocation(SegmentDef * segment)
{
    //this->segment = segment;
    this->size = segment->getSize();
    this->initPos = segment->getLocation();
}