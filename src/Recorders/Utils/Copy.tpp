#include <Recorders/IRecorder.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>
#include <types/types.h>

template<>
std::unique_ptr<DiskRecorder<DirectedGraph>> Copy(const IRecorder<Automata>* from) {
    auto diskRecorder = dynamic_cast<const DiskRecorder<Automata>*>(from);
    if (diskRecorder) {
        return std::make_unique<DiskRecorder<DirectedGraph>>(
            diskRecorder->dirPath,
            "./", // or diskRecorder->getSubDir() if you have it
            false
        );
    }

    throw std::runtime_error("Copy: Unsupported recorder type or missing cast.");
}

template std::unique_ptr<DiskRecorder<DirectedGraph>> Copy(const IRecorder<Automata>* from);
