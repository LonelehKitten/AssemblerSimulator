#include "ServiceBus.h"
#include "InterfaceBus.h"

namespace ServiceBus {
    void startExpandMacros() {
        InterfaceBus::getInstance().runExpandMacros();
    }
    void startBuildAndRun() {
        InterfaceBus::getInstance().runBuildAndRun();
    }
    void startAssembleAndRun() {
        InterfaceBus::getInstance().runAssembleAndRun();
    }
    void startAssembleAndRunBySteps() {
        InterfaceBus::getInstance().runAssembleAndRunBySteps();
    }
    void startRun() {
        InterfaceBus::getInstance().runRun();
    }
    void startRunBySteps() {
        InterfaceBus::getInstance().runRunBySteps();
    }
    void startTest() {
        InterfaceBus::getInstance().runTest();
    }
    //void startExpandMacros();
    //void startExpandMacros();
};
