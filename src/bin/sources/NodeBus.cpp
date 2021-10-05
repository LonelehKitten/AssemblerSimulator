#include "NodeBus.h"

NodeBus::NodeBus(EventEmitter& eventEmitter, NodeInfo& info)
    : eventEmitter(eventEmitter), info(info)
{}
