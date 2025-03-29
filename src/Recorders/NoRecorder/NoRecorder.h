//
// Created by User on 29.03.2025.
//

#ifndef NORECORDER_H
#define NORECORDER_H

#include "Recorders/IRecorder.h"

class NoRecorder : public IRecorder  {
public:
    void recordGraph(const Graph& g) override;
};



#endif //NORECORDER_H
