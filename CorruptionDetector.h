//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_CORRUPTIONDETECTOR_H
#define CHESS_CORRUPTIONDETECTOR_H

#include <iostream>
#include <thread>
#include <csignal>

template<size_t N>
class CorruptionDetector
{
private:
    void* guardBytes[N]{};
public:
    inline CorruptionDetector()
    {
        for (size_t i = 0; i < N; i++) {
            guardBytes[i] = this;
        }
    }

    inline size_t isCorrupted()
    {
        for (size_t i = 0; i < N; i++) {
            if (guardBytes[i] != this) {
                return i + 1;
            }
        }

        return 0;
    }

    inline std::string drawCorruptionDiagram()
    {
        std::stringstream ss;
        // Example: [====**========]
        // Where * is the corruption
        ss << "[";
        for (size_t i = 0; i < N; i++) {
            if (guardBytes[i] == this) {
                ss << "=";
            }
            else {
                ss << "*";
            }
        }
        ss << "]";

        return ss.str();
    }

    inline void validOrThrow()
    {
        auto corruptionIndex = isCorrupted();
        if (corruptionIndex) {
            std::cout << "Memory corruption detected! (" << corruptionIndex << ")" << std::flush
                      << drawCorruptionDiagram() << std::endl;
            throw std::runtime_error("Memory corruption detected!");
        }
    }
};

typedef CorruptionDetector<16384> HailMaryMemoryCorruptionDetector;


#endif //CHESS_CORRUPTIONDETECTOR_H
