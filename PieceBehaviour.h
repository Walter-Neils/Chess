//
// Created by walterineils on 3/9/23.
//

#ifndef CHESS_PIECEBEHAVIOUR_H
#define CHESS_PIECEBEHAVIOUR_H

#include <nlohmann/json.hpp>
#include <iostream>

struct Move
{
    std::string type;
    std::string offsetMode;
    bool        isFirstMove;
    bool        isCapture;
    int         x;
    int         y;

    bool operator==(const Move& rhs) const
    {
        return type == rhs.type && offsetMode == rhs.offsetMode && isFirstMove == rhs.isFirstMove &&
               isCapture == rhs.isCapture && x == rhs.x && y == rhs.y;
    }
};

class PieceBehaviour
{
private:
    std::string       pieceName;
    std::vector<Move> moves;
    int               value = -1;
    std::string       iconName;
private:
    PieceBehaviour() = default;

public:
    PieceBehaviour(const std::string& pieceName, const std::vector<Move>& moves, int value,
                   const std::string& iconName);

    [[nodiscard]] const std::string& getPieceName() const;

    [[nodiscard]] const std::vector<Move>& getMoves() const;

    [[maybe_unused]][[nodiscard]] int getValue() const;

    [[maybe_unused]][[nodiscard]] const std::string& getIconNameRaw() const;

    [[nodiscard]] std::string getIconName(bool isWhite, bool isOutlined) const;

    static PieceBehaviour from_json(const nlohmann::json& j);
};

/*
 * {
  "pieceName": "knight",
  "moves": [
    {
      "type": "jump",
      "offset": {
        "x": 2,
        "y": 1
      },
      "isCapture": false,
      "isFirstMove": false,
      "offsetMode": "absolute"
    },
    {
      "type": "jump",
      "offset": {
        "x": 2,
        "y": -1
      },
      "isCapture": false,
      "isFirstMove": false,
      "offsetMode": "absolute"
    },
    {
      "type": "jump",
      "offset": {
        "x": -2,
        "y": 1
      },
      "isCapture": false,
      "isFirstMove": false,
      "offsetMode": "absolute"
    },
    {
      "type": "jump",
      "offset": {
        "x": -2,
        "y": -1
      },
      "isCapture": false,
      "isFirstMove": false,
      "offsetMode": "absolute"
    },
    {
      "type": "jump",
      "offset": {
        "x": 1,
        "y": 2
      },
      "isCapture": false,
      "isFirstMove": false,
      "offsetMode": "absolute"
    },
    {
      "type": "jump",
      "offset": {
        "x": 1,
        "y": -2
      },
      "isCapture": false,
      "isFirstMove": false,
      "offsetMode": "absolute"
    },
    {
      "type": "jump",
      "offset": {
        "x": -1,
        "y": 2
      },
      "isCapture": false,
      "isFirstMove": false,
      "offsetMode": "absolute"
    },
    {
      "type": "jump",
      "offset": {
        "x": -1,
        "y": -2
      },
      "isCapture": false,
      "isFirstMove": false,
      "offsetMode": "absolute"
    }
  ],
  "pieceValue": 3,
  "pieceIcon": "knight-{color}-{variant}"
}
 * */


#endif //CHESS_PIECEBEHAVIOUR_H
