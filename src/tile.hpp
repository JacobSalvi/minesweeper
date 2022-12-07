#pragma once

enum TileStatus{
    flagged, revelead, hidden
};

enum TileType {
    zero, 
    one,
    two,
    three, 
    four, 
    five,
    six, 
    seven, 
    eight,
    bomb
};

TileType int_to_tile_type(int value){
    switch (value) {
        case 0:
            return TileType::zero;
        case 1:
            return TileType::one;
        case 2:
            return TileType::two;
        case 3:
            return TileType::three;
        case 4:
            return TileType::four;
        case 5:
            return TileType::five;
        case 6:
            return TileType::six;
        case 7:
            return TileType::seven;
        case 8:
            return TileType::eight;
        case 9:
            return TileType::bomb;
        default:
            return TileType::zero;
    }
}

struct Tile{
    private:
        TileType type = TileType::zero;
        TileStatus status = TileStatus::hidden;
    public:
        Tile(){}

        TileType get_type(){
            return type;
        }

        TileStatus get_status(){
            return status;
        }

        void set_status(TileStatus new_status){
            status=new_status;
        }

        void set_type(TileType new_type){
            type = new_type;
        }

        bool is_tile_bomb(){
            return type == TileType::bomb;
        }
};