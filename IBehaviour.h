//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_IBEHAVIOUR_H
#define CHESS_IBEHAVIOUR_H


class IBehaviour
{
public:
    virtual void update() = 0;

    virtual ~IBehaviour() = default;
};


#endif //CHESS_IBEHAVIOUR_H
