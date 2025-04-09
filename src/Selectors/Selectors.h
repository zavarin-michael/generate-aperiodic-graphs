#ifndef SELECTORS_H
#define SELECTORS_H
#include <Generators/IGenerator.h>
#include <Recorders/IRecorder.h>
#include <Readers/IReader.h>

template <typename GraphType>
IRecorder<GraphType>* getRecorder(const std::function<IRecorder<GraphType>*()>& getDefaultRecorder, const std::string& defaultRecorderName);
template <typename GraphType>
IGenerator<GraphType>* getGenerator(const std::function<IGenerator<GraphType>*()>& getDefaultGenerator, const std::string& defaultGeneratorName);
template <typename GraphType>
IReader<GraphType>* getReader(const std::function<IReader<GraphType>*()>& getDefaultReader, const std::string& defaultReaderName);

#include "GetRecorder.tpp"
#include "GetGenerator.tpp"
#include "GetReader.tpp"

#endif
