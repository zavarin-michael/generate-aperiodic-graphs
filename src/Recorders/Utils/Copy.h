#include <Recorders/IRecorder.h>
#include <memory>

#ifndef COPY_H
#define COPY_H

template <typename RecorderTo, typename From>
std::unique_ptr<RecorderTo> Copy(const IRecorder<From>* from);

#include "Copy.tpp"

#endif //COPY_H
