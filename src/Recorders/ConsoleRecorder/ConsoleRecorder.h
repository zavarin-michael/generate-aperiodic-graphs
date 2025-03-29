#ifndef CONSOLERECORDER_H
#define CONSOLERECORDER_H

#include "Recorders/IRecorder.h"

class ConsoleRecorder : public IRecorder {
public:
    void recordGraph(const Graph& g) override;
};

#endif //CONSOLERECORDER_H
