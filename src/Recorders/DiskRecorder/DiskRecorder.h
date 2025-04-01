#ifndef DISK_RECORDER_H
#define DISK_RECORDER_H

#include <filesystem>

#include "Recorders/IRecorder.h"

template<typename GraphType>
class DiskRecorder : public IRecorder<GraphType> {
private:
    std::string filename;
    bool isRewriteFiles;
    std::size_t graphCount = 0;
public:
    std::filesystem::path dirPath;
    explicit DiskRecorder(const std::filesystem::path &rootDir, std::filesystem::path subDirs = "", bool isRewriteFiles = true);
    void recordGraph(GraphType& g) override;
    DiskRecorder setFilename(const std::string& filename);
    DiskRecorder setDirPath(std::filesystem::path dirPath);
    void writeGraph(std::ofstream &ofs, GraphType &g);
};

#include "DiskRecorder.tpp"

#endif
