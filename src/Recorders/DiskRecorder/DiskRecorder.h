#ifndef DISK_RECORDER_H
#define DISK_RECORDER_H

#include "Recorders/IRecorder.h"

class DiskRecorder : public IRecorder {
private:
    const std::string GRAPH_DIR = "graphs";
    std::string subDirectory;
    bool isRewriteFiles = true;
    long long graphCount = 0;
public:
    void initialize() override;
    void recordGraph(const Graph& g) override;
};

#endif
