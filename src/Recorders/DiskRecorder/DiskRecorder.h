#ifndef DISK_RECORDER_H
#define DISK_RECORDER_H

#include <filesystem>

#include "Recorders/IRecorder.h"

template<typename GraphType>
class DiskRecorder : public IRecorder<GraphType> {
private:
    static const std::filesystem::path GRAPH_DIR;
    std::string subDirectory;
    bool isRewriteFiles = true;
    std::size_t graphCount = 0;
public:
    DiskRecorder();
    void recordGraph(const GraphType& g) override;
};

#endif
