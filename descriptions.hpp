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
const std::string ROOM_PRISON_HALLWAY_1 =
    "You step through the cell door into a dark hallway. The hallway stretches "
    "into the darkness...\n";
const std::string ROOM_PRISON_HALLWAY_2 =
    "You walk through the dark hallway...\nThe walls, lined with rows of empty "
    "cells.\n";
const std::string ROOM_PRISON_HALLWAY_3 = "You approach a fork in the hallway...\n";
const std::string ROOM_PRISON_HALLWAY_4 = "You walk down the dark hallway...\n";
const std::string SEARCH_PRISON_HALLWAY_4 =
    "You hear the faint scurrying of rats, their claws scraping against the "
    "floor, drawing closer in the dark.\n";
const std::string ROOM_PRISON_HALLWAY_7 = "You stand in front of an opened door...\n";
const std::string ROOM_PRISON_HALLWAY_5 = "You walk down the dark hallway...\n";
const std::string SEARCH_PRISON_HALLWAY_5 =
    "You hear the faint scurrying of rats, their claws scraping against the "
    "floor, drawing closer in the dark.\n";
const std::string ROOM_PRISON_1_MIDDLE = "You stand in the middle of the dark room...\n";
const std::string ROOM_PRISON_1_NORTH = "A masked figure stands in front of a door...\n";
const std::string ROOM_PRISON_1_SOUTH = "You stand at the southern edge of the dark room...\n";
const std::string ROOM_PRISON_1_EAST = "You feel around the wall...\n";
const std::string ROOM_PRISON_1_WEST = "You feel around the wall...\n";
const std::string ROOM_PRISON_1_NORTHEAST = "You walk into the corner of the room...\n";
const std::string ROOM_PRISON_1_NORTHWEST = "You walk into the corner of the room...\n";
const std::string ROOM_PRISON_1_SOUTHEAST = "You see a chest on ground...\n";
const std::string SEARCH_ROOM_PRISON_1_SOUTHEAST = "The chest looks old and greasy...\n";
const std::string ROOM_PRISON_1_SOUTHWEST = "You walk into the corner of the room...\n";
const std::string ROOM_PRISON_HALLWAY_6 =
    "Before you looms a battered door, the wood blackened with age and "
    "something darker. A twisted, rust-stained plaque above it displays a "
    "single phrase: Torture Chamber.\n";
const std::string ROOM_PRISON_2_SOUTHEAST =
    "You stand at the SOUTH EAST corner of the room. You see chains dangling "
    "over unseen stains...\n";
const std::string ROOM_PRISON_2_SOUTH =
    "The chains rattle faintly as you walk down the SOUTH wall.\n";
const std::string ROOM_PRISON_2_SOUTHWEST =
    "A door looms before you, smooth and unremarkable, yet the air around it "
    "feels wrong.\nThe sign says...Storage\n";
const std::string ROOM_PRISON_2_MIDDLE =
    "The dangling chains ring loudly as you walk through them...\n";
const std::string ROOM_PRISON_2_WEST =
    "As you walk forward, it feels as if the walls are curving inward, closing "
    "the space.\n";
const std::string ROOM_PRISON_2_EAST =
    "You see blood-stained tables surrounded by candles as you walk forward.\n";
const std::string ROOM_PRISON_2_NORTH =
    "You stand in the NORTH wall.\nThe wall is covered in writing, each word "
    "stained with blood.\n";
const std::string SEARCH_ROOM_PRISON_2_NORTH =
    "\033[0;31m" // Start red color
    "▄▄▄█████▓ ██░ ██ ▓█████▓██   ██▓    █     █░ ██▓ ██▓     ██▓       "
    "▓█████▄  ██▀███   ██▓ ███▄    █  ██ ▄█▀                      \n"
    "▓  ██▒ ▓▒▓██░ ██▒▓█   ▀ ▒██  ██▒   ▓█░ █ ░█░▓██▒▓██▒    ▓██▒       ▒██▀ "
    "██▌▓██ ▒ ██▒▓██▒ ██ ▀█   █  ██▄█▒                       \n"
    "▒ ▓██░ ▒░▒██▀▀██░▒███    ▒██ ██░   ▒█░ █ ░█ ▒██▒▒██░    ▒██░       ░██   "
    "█▌▓██ ░▄█ ▒▒██▒▓██  ▀█ ██▒▓███▄░                       \n"
    "░ ▓██▓ ░ ░▓█ ░██ ▒▓█  ▄  ░ ▐██▓░   ░█░ █ ░█ ░██░▒██░    ▒██░       ░▓█▄   "
    "▌▒██▀▀█▄  ░██░▓██▒  ▐▌██▒▓██ █▄                       \n"
    "  ▒██▒ ░ ░▓█▒░██▓░▒████▒ ░ ██▒▓░   ░░██▒██▓ ░██░░██████▒░██████▒   "
    "░▒████▓ ░██▓ ▒██▒░██░▒██░   ▓██░▒██▒ █▄                      \n"
    "  ▒ ░░    ▒ ░░▒░▒░░ ▒░ ░  ██▒▒▒    ░ ▓░▒ ▒  ░▓  ░ ▒░▓  ░░ ▒░▓  ░    ▒▒▓  "
    "▒ ░ ▒▓ ░▒▓░░▓  ░ ▒░   ▒ ▒ ▒ ▒▒ ▓▒                      \n"
    "    ░     ▒ ░▒░ ░ ░ ░  ░▓██ ░▒░      ▒ ░ ░   ▒ ░░ ░ ▒  ░░ ░ ▒  ░    ░ ▒  "
    "▒   ░▒ ░ ▒░ ▒ ░░ ░░   ░ ▒░░ ░▒ ▒░                      \n"
    "  ░       ░  ░░ ░   ░   ▒ ▒ ░░       ░   ░   ▒ ░  ░ ░     ░ ░       ░ ░  "
    "░   ░░   ░  ▒ ░   ░   ░ ░ ░ ░░ ░                       \n"
    "          ░  ░  ░   ░  ░░ ░            ░     ░      ░  ░    ░  ░      ░   "
    "    ░      ░           ░ ░  ░                         \n"
    "                        ░ ░                                         ░     "
    "                                                      \n"
    "▄▄▄█████▓ ██░ ██ ▓█████     ▄▄▄▄    ██▓    ▓█████   ██████   ██████ "
    "▓█████ ▓█████▄     ▄▄▄▄    ██▓     ▒█████   ▒█████  ▓█████▄ \n"
    "▓  ██▒ ▓▒▓██░ ██▒▓█   ▀    ▓█████▄ ▓██▒    ▓█   ▀ ▒██    ▒ ▒██    ▒ ▓█   "
    "▀ ▒██▀ ██▌   ▓█████▄ ▓██▒    ▒██▒  ██▒▒██▒  ██▒▒██▀ ██▌\n"
    "▒ ▓██░ ▒░▒██▀▀██░▒███      ▒██▒ ▄██▒██░    ▒███   ░ ▓██▄   ░ ▓██▄   ▒███  "
    " ░██   █▌   ▒██▒ ▄██▒██░    ▒██░  ██▒▒██░  ██▒░██   █▌\n"
    "░ ▓██▓ ░ ░▓█ ░██ ▒▓█  ▄    ▒██░█▀  ▒██░    ▒▓█  ▄   ▒   ██▒  ▒   ██▒▒▓█  "
    "▄ ░▓█▄   ▌   ▒██░█▀  ▒██░    ▒██   ██░▒██   ██░░▓█▄   ▌\n"
    "  ▒██▒ ░ ░▓█▒░██▓░▒████▒   ░▓█  "
    "▀█▓░██████▒░▒████▒▒██████▒▒▒██████▒▒░▒████▒░▒████▓    ░▓█  ▀█▓░██████▒░ "
    "████▓▒░░ ████▓▒░░▒████▓ \n"
    "  ▒ ░░    ▒ ░░▒░▒░░ ▒░ ░   ░▒▓███▀▒░ ▒░▓  ░░░ ▒░ ░▒ ▒▓▒ ▒ ░▒ ▒▓▒ ▒ ░░░ ▒░ "
    "░ ▒▒▓  ▒    ░▒▓███▀▒░ ▒░▓  ░░ ▒░▒░▒░ ░ ▒░▒░▒░  ▒▒▓  ▒ \n"
    "    ░     ▒ ░▒░ ░ ░ ░  ░   ▒░▒   ░ ░ ░ ▒  ░ ░ ░  ░░ ░▒  ░ ░░ ░▒  ░ ░ ░ ░  "
    "░ ░ ▒  ▒    ▒░▒   ░ ░ ░ ▒  ░  ░ ▒ ▒░   ░ ▒ ▒░  ░ ▒  ▒ \n"
    "  ░       ░  ░░ ░   ░       ░    ░   ░ ░      ░   ░  ░  ░  ░  ░  ░     ░  "
    "  ░ ░  ░     ░    ░   ░ ░   ░ ░ ░ ▒  ░ ░ ░ ▒   ░ ░  ░ \n"
    "          ░  ░  ░   ░  ░    ░          ░  ░   ░  ░      ░        ░     ░  "
    "░   ░        ░          ░  ░    ░ ░      ░ ░     ░    \n"
    "                                 ░                                        "
    "  ░               ░                            ░     \n"
    "\033[0m"; // Reset color back to normal
const std::string ROOM_PRISON_2_NORTHWEST =
    "You stand before a table where a small chest rests, its surface slick "
    "with blood.\n";
const std::string SEARCH_ROOM_PRISON_2_NORTHWEST =
    "You look close and see that blood has soaked into the cracks of the "
    "chest.\n";
const std::string ROOM_PRISON_2_NORTHEAST =
    "A table stands before you, its surface lined with tools designed to tear, "
    "cut, and break.\n";
const std::string SEARCH_ROOM_PRISON_2_NORTHEAST =
    "You run your fingers over the cold, jagged tools, the metallic surface of "
    "each one sending a chill up your spine as you feel the weight of their "
    "purpose.\n";
const std::string ROOM_STORAGE_1 =
    "The room is dimly lit, the air thick with the metallic scent of blood. "
    "Shelves line the walls, each filled with countless bottles, their glass "
    "dark and stained\n";
const std::string SEARCH_ROOM_STORAGE_1 = "You look through the blood-filled bottles...\n";

// Item descriptions
const std::string ITEM_RUSTED_KNIFE = "A slightly dull rusted knife. Could be useful later...";
const std::string ITEM_CELL_KEY =
    "A cold, rusted key. It feels strangely heavy in your hand, as if it "
    "remembers every door it has ever locked... and every one it has trapped "
    "inside.\n";
const std::string ITEM_ROOM_KEY = "A small iron key.\n";
const std::string ITEM_BLOODSTAINED_KEY =
    "The key is small, its surface smeared with fresh blood, the red stains "
    "still wet and dark against the metal.\n";
const std::string ITEM_AEGIS_THORN =
    "A thin, obsidian dagger, its blade slick with dried blood. The hilt is "
    "worn, and a sense of dread clings to it, as if it thirsts for more.\n";
const std::string ITEM_BLOOD_BOTTLE =
    "The bottle is filled with dark blood, its glass marked with strange "
    "symbols. The air smells heavy with iron, and it almost feels like the "
    "blood is calling out, waiting to be consumed.\n";
} // namespace descriptions
