//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_MEMORYWATCHER_H
#define CHESS_MEMORYWATCHER_H

#include <iostream>
#include <vector>
#include <map>

#define WRAPPED_PTR(ptr) MemoryWatcherPtrWrapper(ptr)

template<typename T>
class ScopedConstMemoryContract
{
private:
    bool armed = true;
    T* backupData;
    T* ptr;
    size_t size;
public:
    inline ScopedConstMemoryContract(T* ptr, size_t elementCount = 1) : ptr(ptr), size(elementCount)
    {
        backupData = reinterpret_cast<T*>(malloc(elementCount * sizeof(T)));
        memcpy(backupData, ptr, elementCount);
    }

    inline void disarm()
    {
        armed = false;
    }

    inline ~ScopedConstMemoryContract()
    {
        bool corrupted = false;
        if (armed) {
            for (size_t i = 0; i < size; i++) {
                if (backupData[i] != ptr[i]) {
                    std::cout << "Memory corruption detected! (" << i << ")" << std::flush;
                    corrupted = true;
                }
            }
        }
        free(backupData);
        if (corrupted) {
            throw std::runtime_error("Memory corruption detected!");
        }
    }
};

#define SCOPED_CONST_MEMORY_CONTRACT(ptr) ScopedConstMemoryContract WRAPPED_PTR(ptr)


#endif //CHESS_MEMORYWATCHER_H
