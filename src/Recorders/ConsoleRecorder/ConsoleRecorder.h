#ifndef CONSOLERECORDER_H
#define CONSOLERECORDER_H

#include <string>

#include "Recorders/IRecorder.h"

template<typename GraphType>
class ConsoleRecorder : public IRecorder<GraphType> {
public:
    void recordGraph(GraphType& g, std::string name) override;
};

#endif //CONSOLERECORDER_H
