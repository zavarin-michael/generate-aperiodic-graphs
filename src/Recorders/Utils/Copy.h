#include <Recorders/IRecorder.h>
#include <memory>

#ifndef COPY_H
#define COPY_H

template <typename To, typename From>
IRecorder<To>* Copy(IRecorder<From>* from);

#include "Copy.tpp"

#endif //COPY_H
