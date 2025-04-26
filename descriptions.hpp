#pragma once

#include <string>

namespace descriptions {

// Room descriptions
const std::string ROOM_START = "You stand in the middle of the cell room...\n";
const std::string SEARCH_START =
    "You look around the room and see a cell door to the NORTH wall, a dirty "
    "ragged bed on the floor to the EAST wall, and a dirty bucket to the SOUTH "
    "wall.\n";
const std::string ROOM_START_NORTH = "You face the cell door...\n";
const std::string ROOM_START_SOUTH =
    "You look down and see a bucket filled with who knows what...\n";
const std::string SEARCH_SOUTH =
    "You hesitate before plunging your hand into the sludge-filled bucket. You "
    "feel something cold and slimy...\n";
const std::string ROOM_START_EAST =
    "You look down at the dirty ragged bed... it seems just a minute ago you "
    "were having the worst nightmare...\n";
const std::string SEARCH_EAST = "You feel under the bed...\n";
const std::string ROOM_START_WEST = "You stare blankly at the wall...\n";
const std::string ROOM_START_NORTHEAST = "You face the NORTH EAST corner of the room.\n";
const std::string ROOM_START_NORTHWEST = "You face the NORTH WEST corner of the room.\n";
const std::string ROOM_START_SOUTHEAST = "You face the SOUTH EAST corner of the room.\n";
const std::string ROOM_START_SOUTHWEST = "You face the SOUTH WEST corner of the room.\n";
const std::string ROOM_PRISON_HALLWAY =
    "You step through the cell door into a dark hallway. The hallway stretches "
    "into the darkness...\n";

// Item descriptions
const std::string ITEM_RUSTED_KNIFE = "A slightly dull rusted knife. Could be useful later...";
const std::string ITEM_CELL_KEY = "A small iron key.";
} // namespace descriptions
