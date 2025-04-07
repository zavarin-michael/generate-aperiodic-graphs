#ifndef CONSOLERECORDER_H
#define CONSOLERECORDER_H

#include "Recorders/IRecorder.h"

template<typename GraphType>
class ConsoleRecorder : public IRecorder<GraphType> {
public:
    void recordGraph(GraphType& g) override;
};

#endif //CONSOLERECORDER_H
