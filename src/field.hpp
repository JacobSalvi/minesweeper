#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "app.hpp"
#include "tile.hpp"
#include "xpmHandler.hpp"

class Field {
private:
  std::vector<std::vector<Tile>> field;
  const int width;
  const int height;

  std::vector<XpmHandler> xpm_images = std::vector<XpmHandler>();

  void initialize_images(application app, int screen_num) {
    std::vector<char **> raw_xpms = {
        base_xpm, flag_xpm, empty_xpm, one_xpm, two_xpm, three_xpm,
        four_xpm, five_xpm, six_xpm, seven_xpm, eigth_xpm, bomb_xpm,
    };
    for (int i = 0; i != raw_xpms.size(); ++i) {
      xpm_images.push_back(
          XpmHandler(app.disp, app.win, screen_num, raw_xpms[i]));
    }
  }

  XpmHandler &get_xpm_handler_for_tile(Tile &tile) {
    TileStatus status = tile.get_status();
    if (status == TileStatus::hidden) {
      return xpm_images[0];
    }
    if (status == TileStatus::flagged) {
      return xpm_images[1];
    }
    return xpm_images[tile.get_type() + 2];
  }

  bool is_tile_at_position_bomb(const int row, const int column) {
    if (row < 0 || row >= height) {
      return false;
    }
    if (column < 0 || column >= width) {
      return false;
    }
    return field[row][column].is_tile_bomb();
  }

public:
  Field(const int width, const int height, const int bombs, application app,
        int screen_num)
      : width(width), height(height) {
    std::vector<Tile> tiles = std::vector<Tile>();
    const int total_number_of_tiles = width * height;
    // create random sequence of tiles in which bombs are contained
    for (int i = 0; i != total_number_of_tiles; ++i) {
      Tile new_tile = Tile();
      if (i < bombs) {
        new_tile.set_type(TileType::bomb);
      }
      tiles.push_back(new_tile);
    }

    // shuffle the field
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(tiles.begin(), tiles.end(), std::default_random_engine(seed));

    for (int row = 0; row != height; ++row) {
      std::vector<Tile> tile_row;
      for (int column = 0; column != width; ++column) {
        tile_row.push_back(tiles[row * width + column]);
      }
      field.push_back(tile_row);
    }

    // per each tile set how many bombs it is touching
    for (int row = 0; row != height; ++row) {
      for (int column = 0; column != width; ++column) {
        Tile &tile = field[row][column];
        if (tile.is_tile_bomb()) {
          continue;
        }
        int touching_tile_bombs = 0;
        // count the bombs in the 8 neighbouring tiles
        for (int r = row - 1; r < row + 2; ++r) {
          for (int c = column - 1; c < column + 2; ++c) {
            if (is_tile_at_position_bomb(r, c)) {
              touching_tile_bombs++;
            }
          }
        }
        tile.set_type(int_to_tile_type(touching_tile_bombs));
      }
    }

    initialize_images(app, screen_num);
  }

  void reveal_field() {
    for (int row = 0; row != height; ++row) {
      for (int column = 0; column != width; ++column) {
        field[row][column].set_status(TileStatus::revelead);
      }
    }
  }

  void flag_all_bombs() {
    for (int row = 0; row != height; ++row) {
      for (int column = 0; column != width; ++column) {
        Tile &tile = field[row][column];
        if (tile.get_type() == TileType::bomb) {
          tile.set_status(TileStatus::flagged);
        }
      }
    }
  }

  Tile &get_tile(int row, int column) { return field[row][column]; }

  int reveal_adjacent_tiles(int row, int column) {
    int revealed_tiles = 0;
    for (int r = row - 1; r < row + 2; ++r) {
      if (r < 0 || r >= height) {
        continue;
      }
      for (int c = column - 1; c < column + 2; ++c) {
        if (c < 0 || c >= width) {
          continue;
        }
        Tile &tile = field[r][c];
        if (tile.get_type() == TileType::bomb) {
          continue;
        }
        if (tile.get_status() == TileStatus::revelead) {
          continue;
        }
        if (tile.get_status() == TileStatus::flagged) {
          continue;
        }
        if (tile.get_type() == TileType::zero) {
          tile.set_status(TileStatus::revelead);
          revealed_tiles += reveal_adjacent_tiles(r, c);
        } else {
          tile.set_status(TileStatus::revelead);
        }
        revealed_tiles++;
      }
    }
    return revealed_tiles;
  }

  void draw(application &app) {
    for (int row = 0; row != height; ++row) {
      for (int column = 0; column != width; ++column) {
        Tile &tile = field[row][column];
        XpmHandler& xpm_handler = get_xpm_handler_for_tile(tile);
        // TODO: get rid of the magic number 30
        xpm_handler.draw(app.disp, app.win, 30 * column, 30 * row);
      }
    }
  }

  void debug() {
    for (int i = 0; i != height; ++i) {
      for (int j = 0; j != width; ++j) {
        std::cout << field[i][j].get_type();
      }
      std::cout << std::endl;
    }
  }

  Field &operator=(Field const &t) { return *this; }
};