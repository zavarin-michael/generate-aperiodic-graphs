#ifndef DISK_RECORDER_H
#define DISK_RECORDER_H

#include <filesystem>

#include "Recorders/IRecorder.h"

class DiskRecorder : public IRecorder {
private:
    static const std::filesystem::path GRAPH_DIR;
    std::string subDirectory;
    bool isRewriteFiles = true;
    std::size_t graphCount = 0;
public:
    DiskRecorder();
    void recordGraph(const Graph& g) override;
};

#endif
