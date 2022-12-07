#pragma once

#include "app.hpp"
#include "field.hpp"
#include "tile.hpp"
#include "xpmHandler.hpp"
#include <iostream>

enum GameStatus { setting_up, ready, playing, lost, won };

class Game {
private:
  GameStatus status = GameStatus::setting_up;
  Field game_field;
  int bombs = 0;
  int number_of_tiles = 0;
  int revealed_tiles = 0;
  int flagged_tiles = 0;

  void handle_right_click(int row, int column) {
    Tile &tile = game_field.get_tile(row, column);
    if (tile.get_status() == TileStatus::flagged) {
      tile.set_status(TileStatus::hidden);
      flagged_tiles--;
    }
    else if (tile.get_status() == TileStatus::hidden) {
      tile.set_status(TileStatus::flagged);
      flagged_tiles++;
    }
  }

  void handle_left_click(int row, int column) {
    Tile &tile = game_field.get_tile(row, column);
    if(tile.get_status() == TileStatus::flagged){
        return;
    }
    if (tile.get_type() == TileType::bomb) {
      status = GameStatus::lost;
      return;
    }
    tile.set_status(TileStatus::revelead);
    revealed_tiles++;
    revealed_tiles += game_field.reveal_adjacent_tiles(row, column);
  }

public:
  Game(const int width, const int height, const int bombs, application app, int screen_num)
      : bombs(bombs), game_field(Field(width, height, bombs, app, screen_num)),
        number_of_tiles(width * height) {}

  void handle_click(int x, int y, int button) {
    if (status == GameStatus::setting_up) {
      status = GameStatus::playing;
    }
    int column = x / 30;
    int row = y / 30;
    if (button == 1) {
      handle_left_click(row, column);
    } else if (button == 3) {
      handle_right_click(row, column);
    }
    if (status == GameStatus::lost || status == GameStatus::won) {
      return;
    }
    if (number_of_tiles - revealed_tiles == bombs) {
        std::cout<<number_of_tiles<<", "<<revealed_tiles<<", "<<flagged_tiles<<std::endl;
      status = GameStatus::won;
      std::cout << "VICTORY" << std::endl;
    }
  }

  void draw(application &app, XpmHandler &xpm_handler) {
    switch (status) {
    case GameStatus::lost:
      game_field.reveal_field();
      break;
    case GameStatus::won:
      game_field.flag_all_bombs();
    default:
      break;
    }
    game_field.draw(app);
  }

  void debug() { game_field.debug(); }
};