//
// Created by User on 29.03.2025.
//

#ifndef NORECORDER_H
#define NORECORDER_H

#include "Recorders/IRecorder.h"

template<typename GraphType>
class NoRecorder : public IRecorder<GraphType>  {
public:
    void recordGraph(const GraphType& g) override;
};



#endif //NORECORDER_H
