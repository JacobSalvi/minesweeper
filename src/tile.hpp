#pragma once

#include "app.hpp"
#include "xpmHandler.hpp"

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
        
        void set_xpm_for_tile(application &app, XpmHandler& xpm_handler){
            if(status == TileStatus::hidden){
                xpm_handler.set_new_xpm(app.disp, app.win, base_xpm);
                return;
            }
            if(status==TileStatus::flagged){
                xpm_handler.set_new_xpm(app.disp, app.win, flag_xpm);
                return;
            }

            switch (type) {
                case TileType::zero:
                    xpm_handler.set_new_xpm(app.disp, app.win, empty_xpm);
                    break;
                case TileType::one:
                    xpm_handler.set_new_xpm(app.disp, app.win, one_xpm);
                    break;
                case TileType::two:
                    xpm_handler.set_new_xpm(app.disp, app.win, two_xpm);
                    break;
                case TileType::three:
                    xpm_handler.set_new_xpm(app.disp, app.win, three_xpm);
                    break;
                case TileType::four:
                    xpm_handler.set_new_xpm(app.disp, app.win, four_xpm);
                    break;
                case TileType::five:
                    xpm_handler.set_new_xpm(app.disp, app.win, five_xpm);
                    break;
                case TileType::six:
                    xpm_handler.set_new_xpm(app.disp, app.win, six_xpm);
                    break;
                case TileType::seven:
                    xpm_handler.set_new_xpm(app.disp, app.win, seven_xpm);
                    break;
                case TileType::eight:
                    xpm_handler.set_new_xpm(app.disp, app.win, eigth_xpm);
                    break;
                case TileType::bomb:
                    xpm_handler.set_new_xpm(app.disp, app.win, bomb_xpm);
                    break;
            }
        }
};