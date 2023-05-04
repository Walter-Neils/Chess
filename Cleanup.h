//
// Created by walterineils on 5/3/23.
//

#ifndef CHESS_CLEANUP_H
#define CHESS_CLEANUP_H

template<typename T>
class Cleanup
{
private:
    const T& action;
public:
    Cleanup(const T& action) : action(action) {}

    ~Cleanup()
    {
        action();
    }
};


#endif //CHESS_CLEANUP_H
