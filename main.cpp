#include "descriptions.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

// Func Prototypes
void print_centered(const std::string &text, size_t width);
std::string to_lowercase(const std::string &input);
std::string extract_direction(const std::string &input);
void show_menu();
void start_new_game();
void quit_game(bool &game_running);

class Item {
public:
    std::string item_name;
    std::string item_description;

    Item() = default;

    Item(const std::string &name, const std::string &description)
        : item_name(name), item_description(description) {}
};

class NPC {
public:
    std::string name;
    std::string description;
    int health;
    bool hostile;
    std::string required_item;
    std::string dialogue;
    std::string death_item;
    std::string post_receive_item_dialogue;
    Item *drop_item = nullptr;
    Item *give_player_item = nullptr;
    std::vector<Item> inventory;

    NPC(const std::string &npc_name, const std::string &npc_description, int npc_health = 5,
        bool is_hostile = false, const std::string &npc_required_item = "",
        const std::string &npc_dialogue = "", const std::string &npc_death_item = "",
        const std::string &npc_post_receive_item_dialogue = "", Item *npc_drop_item = nullptr,
        Item *npc_give_player_item = nullptr)
        : name(npc_name), description(npc_description), health(npc_health), hostile(is_hostile),
          required_item(npc_required_item), dialogue(npc_dialogue), death_item(npc_death_item),
          post_receive_item_dialogue(npc_post_receive_item_dialogue), drop_item(npc_drop_item),
          give_player_item(npc_give_player_item) {}

    void talk() const { std::cout << name << ": " << dialogue << "\n\n"; }

    void receive_item(const Item &item) { inventory.push_back(item); }

    void take_damage(int damage) {
        health -= damage;
        if (health <= 0) {
            std::cout << name << " was murdered...\n\n";
        } else {
            std::cout << "You attacked " << name << ".\n\n";
        }
    }

    bool can_accept_item(const Item &item) {
        return required_item.empty() || to_lowercase(item.item_name) == to_lowercase(required_item);
    }

    bool can_be_killed_with_item(const std::string &item_name) {
        return to_lowercase(item_name) == to_lowercase(death_item);
    }
};

class Door {
private:
    bool locked;
    std::string required_key;

public:
    Door(const std::string &key = "") : locked(!key.empty()), required_key(to_lowercase(key)) {}

    bool is_locked() const { return locked; }

    bool can_unlock(const std::vector<Item> &inventory) const {
        for (const auto &item : inventory) {
            if (to_lowercase(item.item_name) == required_key) {
                return true;
            }
        }
        return false;
    }

    void unlock() { locked = false; }

    std::string get_required_key() const { return required_key; }
};

class Chest {
private:
    bool locked;
    std::vector<std::string> required_keys;
    Item contained_item;
    bool opened = false;

public:
    Chest(const Item &item, const std::vector<std::string> &keys = {})
        : locked(!keys.empty()), contained_item(item) {
        for (const auto &key : keys) {
            required_keys.push_back(to_lowercase(key));
        }
    }

    bool is_locked() const { return locked; }

    bool is_opened() const { return opened; }

    bool can_unlock(const std::vector<Item> &inventory) const {
        std::vector<std::string> inventory_keys;
        for (const auto &item : inventory) {
            inventory_keys.push_back(to_lowercase(item.item_name));
        }
        for (const auto &required : required_keys) {
            if (std::find(inventory_keys.begin(), inventory_keys.end(), required) ==
                inventory_keys.end()) {
                return false;
            }
        }
        return true;
    }

    void unlock() { locked = false; }

    Item open() {
        opened = true;
        return contained_item;
    }

    std::string get_required_key() const {
        std::string result;
        for (size_t i = 0; i < required_keys.size(); i++) {
            result += required_keys[i];
            if (i != required_keys.size() - 1) result += ", ";
        }
        return result;
    }
};

class Player {
public:
    std::vector<Item> player_inventory;
    bool is_alive = true;

    void add_to_inventory(const Item &item) {
        player_inventory.push_back(item);
        std::cout << item.item_name << " has been added to your inventory.\n\n";
    }

    void print_inventory() const {
        if (player_inventory.empty()) {
            std::cout << "\nYour inventory is empty.\n";
        } else {
            std::cout << "\nInventory:\n";
            for (const auto &item : player_inventory) {
                std::cout << "- " << item.item_name << ": " << item.item_description << "\n";
            }
        }
    }

    bool has_item(const std::string &item_name) const {
        for (const auto &item : player_inventory) {
            if (item.item_name == item_name) {
                return true;
            }
        }
        return false;
    }

    void player_dies() { is_alive = false; }
};

class Room {
public:
    std::string room_description;
    std::string search_description;
    std::map<std::string, Room *> room_exits;
    std::map<std::string, Door> doors;
    std::vector<Item> items;
    bool has_been_searched = false;
    Chest *chest = nullptr;
    std::string revealed_item_name;
    std::vector<NPC> npcs;

    Room(const std::string &desc, const std::string &search = "")
        : room_description(desc), search_description(search) {}

    void add_room_exit(const std::string &direction, Room *room) { room_exits[direction] = room; }

    Room *get_exit(const std::string &direction) {
        auto i = room_exits.find(direction);
        return i != room_exits.end() ? i->second : nullptr;
    }

    void add_door(const std::string &direction, const Door &door) { doors[direction] = door; }

    bool has_door(const std::string &direction) const {
        return doors.find(direction) != doors.end();
    }

    Door *get_door(const std::string &direction) {
        auto it = doors.find(direction);
        return it != doors.end() ? &(it->second) : nullptr;
    }

    void add_chest(Chest *new_chest) { chest = new_chest; }

    void print_description() {
        std::cout << room_description << "\n";
        if (!npcs.empty()) {
            for (const auto &npc : npcs) {
                std::cout << npc.description << "\n";
            }
        }
    }

    void print_search_description() {
        has_been_searched = true;
        if (!revealed_item_name.empty()) {
            if (!search_description.empty()) {
                std::cout << search_description << "\n";
            }
            std::cout << "You found a " << revealed_item_name << ".\n";
            std::cout << "\nType 'take' to pick it up.\n\n";
        } else if (!search_description.empty()) {
            std::cout << search_description << "\n";
        } else {
            std::cout << "You find nothing of interest.\n\n";
        }

        if (!revealed_item_name.empty()) {
        }
    }

    bool has_been_searched_by_player() const { return has_been_searched; }

    void reset_search_status() {
        has_been_searched = false;
        revealed_item_name = "";
    }

    void add_item(const Item &item) { items.push_back(item); }

    Item *find_item(const std::string &item_name) {
        for (auto &item : items) {
            if (to_lowercase(item.item_name) == item_name) {
                return &item;
            }
        }
        return nullptr;
    }

    bool remove_item(const std::string &item_name) {
        auto it = std::remove_if(items.begin(), items.end(),
                                 [&](Item &i) { return to_lowercase(i.item_name) == item_name; });
        if (it != items.end()) {
            items.erase(it, items.end());
            return true;
        }
        return false;
    }

    void add_npc(const NPC &npc) { npcs.push_back(npc); }

    NPC *find_npc(const std::string &npc_name) {
        for (auto &npc : npcs) {
            if (to_lowercase(npc.name) == to_lowercase(npc_name)) {
                return &npc;
            }
        }
        return nullptr;
    }

    bool remove_npc(const std::string &npc_name) {
        auto i = std::remove_if(npcs.begin(), npcs.end(), [&](NPC &n) {
            return to_lowercase(n.name) == to_lowercase(npc_name);
        });
        if (i != npcs.end()) {
            npcs.erase(i, npcs.end());
            return true;
        }
        return false;
    }
};

void attempt_move(Room *&room_current, const std::string &direction) {
    Room *next_room = room_current->get_exit(direction);
    if (!next_room) {
        std::cout << "You can't go that way.\n\n";
        return;
    }

    if (room_current->has_door(direction)) {
        Door *door = room_current->get_door(direction);
        if (door && door->is_locked()) {
            std::cout << "The door is locked. Maybe there's a key nearby...\n\n";
            return;
        }
    }

    room_current = next_room;
    room_current->print_description();
}

void try_open_door(Room *room_current, Player &player) {
    for (auto &[direction, door] : room_current->doors) {
        if (door.is_locked()) {
            if (door.can_unlock(player.player_inventory)) {
                std::cout << "You use the " << door.get_required_key()
                          << " to unlock the door.\n\n";
                door.unlock();
                return; // unlock just one door at a time
            } else {
                std::cout << "The door is locked.\n\n";
                return;
            }
        }
    }
    std::cout << "There is no locked door here that you can open.\n\n";
}

void talk_to_npc(Room *room_current) {
    if (!room_current->npcs.empty()) {
        NPC *npc = &room_current->npcs[0]; // Always talk to the first NPC in the room
        npc->talk();
    } else {
        std::cout << "There is no one to talk to...\n\n";
    }
}

void give_item_to_npc(Room *room_current, Player &player, const std::string &item_name) {
    // Check for NPC in the room
    if (room_current->npcs.empty()) {
        std::cout << "There is no one to give the item to...\n\n";
        return;
    }

    // Find the first NPC in the room
    NPC *npc = &room_current->npcs[0];
    if (!npc) {
        std::cout << "There is no one to give an item to...\n\n";
        return;
    }

    // Give the required item
    if (npc->required_item.empty()) {
        std::cout << npc->name << " doesn't seem interested in anything you have.\n";
        return;
    }

    // Find item in player's inventory
    auto i = std::find_if(
        player.player_inventory.begin(), player.player_inventory.end(),
        [&](const Item &item) { return to_lowercase(item.item_name) == to_lowercase(item_name); });

    if (i != player.player_inventory.end()) {
        // Check if the item matches what the NPC wants
        if (to_lowercase(i->item_name) == to_lowercase(npc->required_item)) {
            npc->receive_item(*i);
            player.player_inventory.erase(i);
            std::cout << "You gave the " << item_name << " to " << npc->name << ".\n\n";

            if (!npc->post_receive_item_dialogue.empty()) {
                std::cout << npc->post_receive_item_dialogue << "\n";
            }

            if (npc->give_player_item != nullptr) {
                std::cout << npc->name << " gives you a " << npc->give_player_item->item_name
                          << ".\n\n";
                player.add_to_inventory(*npc->give_player_item);
                npc->give_player_item = nullptr;
            }
        } else {
            std::cout << npc->name << " doesn't want that item.\n\n";
        }

        if (npc->can_be_killed_with_item(item_name)) {
            npc->health = 0;
            std::cout << npc->name << " falls to the ground and dies...\n\n";
            room_current->remove_npc(npc->name);

            if (npc->drop_item != nullptr) {
                room_current->add_item(*npc->drop_item);
                room_current->revealed_item_name = npc->drop_item->item_name;
                room_current->has_been_searched = false;
            }
        }
    } else {
        std::cout << "You don't have that item...\n\n";
    }
}

void attack_npc(Room *room_current, Player &player, const std::string &npc_name) {
    NPC *npc = room_current->find_npc(npc_name);
    if (npc) {
        int max_damage = 1; // Default damage

        for (const auto &item : player.player_inventory) {
            if (item.item_name == "rusted knife") {
                max_damage = std::max(max_damage, 2);
            } else if (item.item_name == "obsidian dagger") {
                max_damage = std::max(max_damage, 5);
            }
        }

        npc->take_damage(max_damage);
        int required_damage = 5;

        if (max_damage >= required_damage) {
            if (npc->health <= 0) {
                if (npc->drop_item != nullptr) {
                    room_current->add_item(*npc->drop_item);
                    room_current->revealed_item_name = npc->drop_item->item_name;
                    room_current->has_been_searched = false;
                }
                room_current->remove_npc(npc_name);
            }
        } else {
            std::cout << "The " << npc->name
                      << " stands, and without hesitation...\nslices your throat.\n";
            player.player_dies();
            return;
        }
    } else {
        std::cout << "There is no one to attack..\n\n";
    }
}

bool check_victory(const Player &player) {
    for (const auto &item : player.player_inventory) {
        if (to_lowercase(item.item_name) == "orbis dei") {
            std::cout << "\nYou feel an impossible weight settle in your "
                         "hands...\nYou hear the heavens call upon you...\nThe ORBIS "
                         "DEI hums with unknowable power...\nEverything "
                         "fades...\nThanks for playing!!!\n\n";
            return true;
        }
    }
    return false;
}

// Global items
std::map<std::string, Item> item_library = {
    {"rusted knife", Item("rusted knife", descriptions::ITEM_RUSTED_KNIFE)},
    {"cell key", Item("cell key", descriptions::ITEM_CELL_KEY)},
    {"room key", Item("room key", descriptions::ITEM_ROOM_KEY)},
    {"blood-stained key", Item("blood-stained key", descriptions::ITEM_BLOODSTAINED_KEY)},
    {"obsidian dagger", Item("obsidian dagger", descriptions::ITEM_OBSIDIAN_DAGGER)},
    {"blood bottle", Item("blood bottle", descriptions::ITEM_BLOOD_BOTTLE)},
    {"gold key", Item("gold key", descriptions::ITEM_GOLD_KEY)},
    {"pater orbis", Item("pater orbis", descriptions::ITEM_PATER_ORBIS)},
    {"mater orbis", Item("mater orbis", descriptions::ITEM_MATER_ORBIS)},
    {"filius orbis", Item("filius orbis", descriptions::ITEM_FILIUS_ORBIS)},
    {"ORBIS DEI", Item("ORBIS DEI", descriptions::ITEM_ORBIS_DEI)},
    {"notes", Item("notes", descriptions::ITEM_NOTES)},
    {"torn note", Item("torn note", descriptions::ITEM_TORN_NOTE)},
    {"blood necklace", Item("blood necklace", descriptions::ITEM_BLOOD_NECKLACE)},
    {"mother's heart", Item("mother's heart", descriptions::ITEM_MOTHERS_HEART)},
    {"wooden sword", Item("wooden sword", descriptions::ITEM_WOODEN_SWORD)}};

// Functions
void print_centered(const std::string &text, size_t width = 80) {
    size_t pad = (width - text.length()) / 2;
    if (pad > 0) std::cout << std::string(pad, ' ');
    std::cout << text << '\n';
}

std::string to_lowercase(const std::string &input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string extract_direction(const std::string &input) {
    std::vector<std::string> direction_words = {"north", "south", "east", "west"};
    for (const auto &dir : direction_words) {
        if (input.find(dir) != std::string::npos) {
            return dir;
        }
    }
    return "";
}

void start_new_game() {
    std::cout << "\nInitializing TENEBRAE...\n\nYou wake up in dimly lit room...\n";

    Room room_start(descriptions::ROOM_START, descriptions::SEARCH_START);
    Room room_start_north(descriptions::ROOM_START_NORTH);
    Room room_start_south(descriptions::ROOM_START_SOUTH, descriptions::SEARCH_SOUTH);
    Room room_start_east(descriptions::ROOM_START_EAST, descriptions::SEARCH_EAST);
    Room room_start_west(descriptions::ROOM_START_WEST);
    Room room_start_northeast(descriptions::ROOM_START_NORTHEAST);
    Room room_start_northwest(descriptions::ROOM_START_NORTHWEST);
    Room room_start_southeast(descriptions::ROOM_START_SOUTHEAST);
    Room room_start_southwest(descriptions::ROOM_START_SOUTHWEST);
    Room room_prison_hallway_1(descriptions::ROOM_PRISON_HALLWAY_1);
    Room room_prison_hallway_2(descriptions::ROOM_PRISON_HALLWAY_2);
    Room room_prison_hallway_3(descriptions::ROOM_PRISON_HALLWAY_3);
    Room room_prison_hallway_4(descriptions::ROOM_PRISON_HALLWAY_4,
                               descriptions::SEARCH_PRISON_HALLWAY_4);
    Room room_prison_hallway_5(descriptions::ROOM_PRISON_HALLWAY_5,
                               descriptions::SEARCH_PRISON_HALLWAY_5);
    Room room_prison_hallway_7(descriptions::ROOM_PRISON_HALLWAY_7);
    Room room_prison_1_middle(descriptions::ROOM_PRISON_1_MIDDLE,
                              descriptions::SEARCH_ROOM_PRISON_1_MIDDLE);
    Room room_prison_1_north(descriptions::ROOM_PRISON_1_NORTH);
    Room room_prison_1_south(descriptions::ROOM_PRISON_1_SOUTH);
    Room room_prison_1_east(descriptions::ROOM_PRISON_1_EAST);
    Room room_prison_1_west(descriptions::ROOM_PRISON_1_WEST,
                            descriptions::SEARCH_ROOM_PRISON_1_WEST);
    Room room_prison_1_northeast(descriptions::ROOM_PRISON_1_NORTHEAST);
    Room room_prison_1_northwest(descriptions::ROOM_PRISON_1_NORTHWEST);
    Room room_prison_1_southeast(descriptions::ROOM_PRISON_1_SOUTHEAST,
                                 descriptions::SEARCH_ROOM_PRISON_1_SOUTHEAST);
    Room room_prison_1_southwest(descriptions::ROOM_PRISON_1_SOUTHWEST);
    Room room_prison_hallway_8(descriptions::ROOM_PRISON_HALLWAY_8);
    Room room_prison_hallway_6(descriptions::ROOM_PRISON_HALLWAY_6);
    Room room_prison_2_southeast(descriptions::ROOM_PRISON_2_SOUTHEAST);
    Room room_prison_2_south(descriptions::ROOM_PRISON_2_SOUTH);
    Room room_prison_2_southwest(descriptions::ROOM_PRISON_2_SOUTHWEST);
    Room room_prison_2_middle(descriptions::ROOM_PRISON_2_MIDDLE);
    Room room_prison_2_west(descriptions::ROOM_PRISON_2_WEST);
    Room room_prison_2_east(descriptions::ROOM_PRISON_2_EAST);
    Room room_prison_2_north(descriptions::ROOM_PRISON_2_NORTH,
                             descriptions::SEARCH_ROOM_PRISON_2_NORTH);
    Room room_prison_2_northwest(descriptions::ROOM_PRISON_2_NORTHWEST,
                                 descriptions::SEARCH_ROOM_PRISON_2_NORTHWEST);
    Room room_prison_2_northeast(descriptions::ROOM_PRISON_2_NORTHEAST,
                                 descriptions::SEARCH_ROOM_PRISON_2_NORTHEAST);
    Room room_storage_1(descriptions::ROOM_STORAGE_1, descriptions::SEARCH_ROOM_STORAGE_1);
    Room room_prison_hallway_9(descriptions::ROOM_PRISON_HALLWAY_9);
    Room room_prison_hallway_10(descriptions::ROOM_PRISON_HALLWAY_10);
    Room room_prison_hallway_11(descriptions::ROOM_PRISON_HALLWAY_11);
    Room room_prison_hallway_12(descriptions::ROOM_PRISON_HALLWAY_12);
    Room room_cathedral_g1(descriptions::ROOM_CATHEDRAL_G1);
    Room room_cathedral_g2(descriptions::ROOM_CATHEDRAL_G2);
    Room room_cathedral_g3(descriptions::ROOM_CATHEDRAL_G3);
    Room room_cathedral_g4(descriptions::ROOM_CATHEDRAL_G4);
    Room room_cathedral_g5(descriptions::ROOM_CATHEDRAL_G5);
    Room room_cathedral_g6(descriptions::ROOM_CATHEDRAL_G6);
    Room room_cathedral_g7(descriptions::ROOM_CATHEDRAL_G7);
    Room room_cathedral_g8(descriptions::ROOM_CATHEDRAL_G8);
    Room room_cathedral_g9(descriptions::ROOM_CATHEDRAL_G9);
    Room room_cathedral_g10(descriptions::ROOM_CATHEDRAL_G10,
                            descriptions::SEARCH_ROOM_CATHEDRAL_G10);
    Room room_cathedral_g11(descriptions::ROOM_CATHEDRAL_G11);
    Room room_cathedral_g12(descriptions::ROOM_CATHEDRAL_G12);
    Room room_cathedral_g13(descriptions::ROOM_CATHEDRAL_G13);
    Room room_cathedral_g14(descriptions::ROOM_CATHEDRAL_G14);
    Room room_cathedral_g15(descriptions::ROOM_CATHEDRAL_G15,
                            descriptions::SEARCH_ROOM_CATHEDRAL_G15);
    Room room_cathedral_g16(descriptions::ROOM_CATHEDRAL_G16);
    Room room_cathedral_g17(descriptions::ROOM_CATHEDRAL_G17);
    Room room_cathedral_g18(descriptions::ROOM_CATHEDRAL_G18);
    Room room_cathedral_g19(descriptions::ROOM_CATHEDRAL_G19,
                            descriptions::SEARCH_ROOM_CATHEDRAL_G19);
    Room room_cathedral_g20(descriptions::ROOM_CATHEDRAL_G20);
    Room room_cathedral_g21(descriptions::ROOM_CATHEDRAL_G21,
                            descriptions::SEARCH_ROOM_CATHEDRAL_G21);
    Room room_cathedral_g22(descriptions::ROOM_CATHEDRAL_G22);
    Room room_brother_1_middle(descriptions::ROOM_BROTHER_1_MIDDLE);
    Room room_brother_1_south(descriptions::ROOM_BROTHER_1_SOUTH);
    Room room_brother_1_west(descriptions::ROOM_BROTHER_1_WEST);
    Room room_brother_1_east(descriptions::ROOM_BROTHER_1_EAST);
    Room room_brother_1_southeast(descriptions::ROOM_BROTHER_1_SOUTHEAST);
    Room room_brother_1_southwest(descriptions::ROOM_BROTHER_1_SOUTHWEST);
    Room room_brother_2_middle(descriptions::ROOM_BROTHER_2_MIDDLE);
    Room room_brother_2_north(descriptions::ROOM_BROTHER_2_NORTH);
    Room room_brother_2_south(descriptions::ROOM_BROTHER_2_SOUTH);
    Room room_brother_2_east(descriptions::ROOM_BROTHER_2_EAST);
    Room room_brother_2_northeast(descriptions::ROOM_BROTHER_2_NORTHEAST);
    Room room_brother_2_southeast(descriptions::ROOM_BROTHER_2_SOUTHEAST);
    Room room_brother_3_middle(descriptions::ROOM_BROTHER_3_MIDDLE);
    Room room_brother_3_north(descriptions::ROOM_BROTHER_3_NORTH);
    Room room_brother_3_south(descriptions::ROOM_BROTHER_3_SOUTH);
    Room room_brother_3_west(descriptions::ROOM_BROTHER_3_WEST);
    Room room_brother_3_northwest(descriptions::ROOM_BROTHER_3_NORTHWEST);
    Room room_brother_3_southwest(descriptions::ROOM_BROTHER_3_SOUTHWEST);

    // Starting Room Area Items, Doors, Chests
    room_start_north.add_door("north", Door("cell key"));
    room_start_south.revealed_item_name = "cell key";
    room_start_east.revealed_item_name = "rusted knife";
    room_start_south.add_item(item_library["cell key"]);
    room_start_east.add_item(item_library["rusted knife"]);

    // Starting Room Area
    room_start.add_room_exit("north", &room_start_north);
    room_start.add_room_exit("south", &room_start_south);
    room_start.add_room_exit("east", &room_start_east);
    room_start.add_room_exit("west", &room_start_west);
    room_start_north.add_room_exit("south", &room_start);
    room_start_north.add_room_exit("east", &room_start_northeast);
    room_start_north.add_room_exit("west", &room_start_northwest);
    room_start_northwest.add_room_exit("east", &room_start_north);
    room_start_northwest.add_room_exit("south", &room_start_west);
    room_start_northeast.add_room_exit("west", &room_start_north);
    room_start_northeast.add_room_exit("south", &room_start_east);
    room_start_north.add_room_exit("west", &room_start_northwest);
    room_start_south.add_room_exit("north", &room_start);
    room_start_south.add_room_exit("east", &room_start_southeast);
    room_start_south.add_room_exit("west", &room_start_southwest);
    room_start_southwest.add_room_exit("north", &room_start_west);
    room_start_southwest.add_room_exit("east", &room_start_south);
    room_start_southeast.add_room_exit("west", &room_start_south);
    room_start_southeast.add_room_exit("north", &room_start_east);
    room_start_west.add_room_exit("north", &room_start_northwest);
    room_start_west.add_room_exit("east", &room_start);
    room_start_west.add_room_exit("south", &room_start_southwest);
    room_start_east.add_room_exit("west", &room_start);
    room_start_east.add_room_exit("north", &room_start_northeast);
    room_start_east.add_room_exit("south", &room_start_southeast);

    // Prison Hallway Area
    room_start_north.add_room_exit("north", &room_prison_hallway_1);
    room_prison_hallway_1.add_room_exit("south", &room_start_north);
    room_prison_hallway_1.add_room_exit("north", &room_prison_hallway_2);
    room_prison_hallway_2.add_room_exit("south", &room_prison_hallway_1);
    room_prison_hallway_2.add_room_exit("north", &room_prison_hallway_3);
    room_prison_hallway_3.add_room_exit("south", &room_prison_hallway_2);
    room_prison_hallway_3.add_room_exit("north", &room_prison_hallway_4);
    room_prison_hallway_4.add_room_exit("south", &room_prison_hallway_3);
    room_prison_hallway_4.add_room_exit("north", &room_prison_hallway_7);
    room_prison_hallway_3.add_room_exit("west", &room_prison_hallway_5);
    room_prison_hallway_5.add_room_exit("east", &room_prison_hallway_3);
    room_prison_hallway_5.add_room_exit("west", &room_prison_hallway_6);
    room_prison_hallway_6.add_room_exit("east", &room_prison_hallway_5);
    room_prison_hallway_6.add_room_exit("west", &room_prison_2_southeast);
    room_prison_hallway_7.add_room_exit("north", &room_prison_1_south);
    room_prison_hallway_7.add_room_exit("south", &room_prison_hallway_4);
    room_prison_hallway_8.add_room_exit("east", &room_prison_1_west);
    room_prison_hallway_8.add_room_exit("west", &room_prison_hallway_9);
    room_prison_hallway_9.add_room_exit("east", &room_prison_hallway_8);
    room_prison_hallway_9.add_room_exit("west", &room_prison_hallway_10);
    room_prison_hallway_10.add_room_exit("east", &room_prison_hallway_9);
    room_prison_hallway_10.add_room_exit("north", &room_prison_hallway_11);
    room_prison_hallway_11.add_room_exit("south", &room_prison_hallway_10);
    room_prison_hallway_11.add_room_exit("north", &room_prison_hallway_12);
    room_prison_hallway_12.add_room_exit("south", &room_prison_hallway_11);
    room_prison_hallway_12.add_room_exit("north", &room_cathedral_g21);

    // Items and Chests in Prison Room 1
    Chest chest_pr_1(item_library["room key"]);
    room_prison_1_southeast.add_chest(&chest_pr_1);

    // NPC in Prison Room 1 (NORTH)
    NPC masked_figure_1("Masked Figure",
                        "A masked figure stands motionless. It watches you, and you can’t shake "
                        "the sense it wants something...from you.\n",
                        5, false, "blood bottle", "...", "blood bottle",
                        "The masked figure lifts the blood bottle overhead and lets out a "
                        "bone-chilling screech that echoes through the chamber.\nThe masked "
                        "figure drinks the "
                        "whole bottle...\n",
                        &item_library["gold key"]);
    room_prison_1_north.add_npc(masked_figure_1);

    // Prison Room 1
    room_prison_1_west.add_door("west", Door("gold key"));

    room_prison_1_south.add_room_exit("south", &room_prison_hallway_7);
    room_prison_1_south.add_room_exit("north", &room_prison_1_middle);
    room_prison_1_south.add_room_exit("east", &room_prison_1_southeast);
    room_prison_1_south.add_room_exit("west", &room_prison_1_southwest);
    room_prison_1_middle.add_room_exit("south", &room_prison_1_south);
    room_prison_1_middle.add_room_exit("east", &room_prison_1_east);
    room_prison_1_middle.add_room_exit("west", &room_prison_1_west);
    room_prison_1_middle.add_room_exit("north", &room_prison_1_north);
    room_prison_1_north.add_room_exit("south", &room_prison_1_middle);
    room_prison_1_north.add_room_exit("east", &room_prison_1_northeast);
    room_prison_1_north.add_room_exit("west", &room_prison_1_northwest);
    room_prison_1_west.add_room_exit("north", &room_prison_1_northwest);
    room_prison_1_west.add_room_exit("east", &room_prison_1_middle);
    room_prison_1_west.add_room_exit("south", &room_prison_1_southwest);
    room_prison_1_east.add_room_exit("north", &room_prison_1_northeast);
    room_prison_1_east.add_room_exit("west", &room_prison_1_middle);
    room_prison_1_east.add_room_exit("south", &room_prison_1_southeast);
    room_prison_1_northeast.add_room_exit("west", &room_prison_1_north);
    room_prison_1_northeast.add_room_exit("south", &room_prison_1_east);
    room_prison_1_northwest.add_room_exit("east", &room_prison_1_north);
    room_prison_1_northwest.add_room_exit("south", &room_prison_1_west);
    room_prison_1_southeast.add_room_exit("north", &room_prison_1_east);
    room_prison_1_southeast.add_room_exit("west", &room_prison_1_south);
    room_prison_1_southwest.add_room_exit("north", &room_prison_1_west);
    room_prison_1_southwest.add_room_exit("east", &room_prison_1_south);
    room_prison_1_west.add_room_exit("west", &room_prison_hallway_8);

    // Items and Chests in Prison Room
    room_prison_2_northeast.add_item(item_library["blood-stained key"]);
    room_prison_2_northeast.revealed_item_name = "blood-stained key";
    Chest chest_pr_2(item_library["obsidian dagger"], {"blood-stained key"});
    room_prison_2_northwest.add_chest(&chest_pr_2);

    // Prison Room 2, Torture Chamber
    room_prison_hallway_6.add_door("west", Door("room key"));

    room_prison_2_southeast.add_room_exit("east", &room_prison_hallway_6);
    room_prison_2_southeast.add_room_exit("west", &room_prison_2_south);
    room_prison_2_southeast.add_room_exit("north", &room_prison_2_east);
    room_prison_2_southwest.add_room_exit("east", &room_prison_2_south);
    room_prison_2_southwest.add_room_exit("north", &room_prison_2_west);
    room_prison_2_south.add_room_exit("east", &room_prison_2_southeast);
    room_prison_2_south.add_room_exit("north", &room_prison_2_middle);
    room_prison_2_south.add_room_exit("west", &room_prison_2_southwest);
    room_prison_2_middle.add_room_exit("south", &room_prison_2_south);
    room_prison_2_middle.add_room_exit("east", &room_prison_2_east);
    room_prison_2_middle.add_room_exit("west", &room_prison_2_west);
    room_prison_2_middle.add_room_exit("north", &room_prison_2_north);
    room_prison_2_north.add_room_exit("south", &room_prison_2_middle);
    room_prison_2_north.add_room_exit("west", &room_prison_2_northwest);
    room_prison_2_north.add_room_exit("east", &room_prison_2_northeast);
    room_prison_2_northwest.add_room_exit("east", &room_prison_2_north);
    room_prison_2_northwest.add_room_exit("south", &room_prison_2_west);
    room_prison_2_northeast.add_room_exit("west", &room_prison_2_north);
    room_prison_2_northeast.add_room_exit("south", &room_prison_2_east);
    room_prison_2_west.add_room_exit("east", &room_prison_2_middle);
    room_prison_2_west.add_room_exit("north", &room_prison_2_northwest);
    room_prison_2_west.add_room_exit("south", &room_prison_2_southwest);
    room_prison_2_east.add_room_exit("west", &room_prison_2_middle);
    room_prison_2_east.add_room_exit("north", &room_prison_2_northeast);
    room_prison_2_east.add_room_exit("south", &room_prison_2_southeast);

    // Doors and Items in Room Storage 1
    room_prison_2_southwest.add_door("west", Door("blood-stained key"));
    room_storage_1.add_item(item_library["blood bottle"]);
    room_storage_1.revealed_item_name = "blood bottle";

    // Room Storage 1
    room_prison_2_southwest.add_room_exit("west", &room_storage_1);
    room_storage_1.add_room_exit("east", &room_prison_2_southwest);

    // Doors and Items in Cathedral Room
    room_prison_hallway_12.add_door("north", Door("gold key"));
    room_cathedral_g6.add_door("west", Door("gold key"));
    room_cathedral_g14.add_door("east", Door("gold key"));
    room_cathedral_g1.add_door("north", Door("gold key"));
    Chest chest_c1(item_library["ORBIS DEI"], {"pater orbis", "mater orbis", "filius orbis"});
    room_cathedral_g10.add_chest(&chest_c1);
    Chest chest_c2(item_library["mother's heart"]);
    room_cathedral_g15.add_chest(&chest_c2);
    Chest chest_c3(item_library["wooden sword"]);
    room_cathedral_g19.add_chest(&chest_c3);

    // NPCS
    NPC masked_figure_2("Masked Figure", "A masked figure stands there motionless...", 5, false,
                        "blood bottle", "The room named Filius contains the son...", "blood bottle",
                        "The masked figure lifts the blood bottle overhead and lets out a "
                        "bone-chilling screech that echoes through the chamber.\nThe masked "
                        "figure drinks the whole bottle...\n",
                        &item_library["notes"], nullptr);
    room_cathedral_g6.add_npc(masked_figure_2);
    NPC masked_figure_4("Masked Figure", "A masked figure stands there motionless...", 5, false,
                        "blood bottle", "The room named Mater houses the mother...", "blood bottle",
                        "The masked figure lifts the blood bottle overhead and lets out a "
                        "bone-chilling screech that echoes through the chamber.\nThe masked "
                        "figure drinks the whole bottle...\n",
                        &item_library["torn note"], nullptr);
    room_cathedral_g14.add_npc(masked_figure_4);
    NPC masked_figure_3("Masked Figure", "A masked figure stands there motionless...", 5, false,
                        "blood bottle", "WORSHIP THY PATER!", "blood bottle",
                        "The masked figure lifts the blood bottle overhead and lets out a "
                        "bone-chilling screech that echoes through the chamber.\nThe masked "
                        "figure drinks the whole bottle...\n",
                        nullptr, nullptr);
    room_cathedral_g1.add_npc(masked_figure_3);
    NPC masked_priest("Masked Priest",
                      "The priest stands in silence, his white robes soaked through with "
                      "blood. A gold mask hides his face. You see nothing in his eyes as they "
                      "stare at you...\nYou also notice a necklace, with a blood vial dangling "
                      "from his neck...\n",
                      5, false, "blood bottle",
                      "Pater Orbis - the eye that judges!\nMater Orbis - the heart that "
                      "grieves!\nFilius Orbis - the hand that strikes!\nEach must be "
                      "fed.\nOnly through blood does their silence speak.\nOnly through "
                      "sacrifice, the cycle will be complete.",
                      "blood bottle", "Yes. The sacred blood! Drink this with me my brothers!",
                      &item_library["blood necklace"], nullptr);
    room_cathedral_g10.add_npc(masked_priest);

    // Cathedral Room
    room_cathedral_g21.add_room_exit("south", &room_prison_hallway_12);
    room_cathedral_g21.add_room_exit("west", &room_cathedral_g20);
    room_cathedral_g21.add_room_exit("east", &room_cathedral_g22);
    room_cathedral_g21.add_room_exit("north", &room_cathedral_g17);
    room_cathedral_g20.add_room_exit("east", &room_cathedral_g21);
    room_cathedral_g20.add_room_exit("north", &room_cathedral_g16);
    room_cathedral_g22.add_room_exit("west", &room_cathedral_g21);
    room_cathedral_g22.add_room_exit("north", &room_cathedral_g18);
    room_cathedral_g17.add_room_exit("south", &room_cathedral_g21);
    room_cathedral_g17.add_room_exit("west", &room_cathedral_g16);
    room_cathedral_g17.add_room_exit("east", &room_cathedral_g18);
    room_cathedral_g17.add_room_exit("north", &room_cathedral_g10);
    room_cathedral_g16.add_room_exit("east", &room_cathedral_g17);
    room_cathedral_g16.add_room_exit("west", &room_cathedral_g15);
    room_cathedral_g16.add_room_exit("south", &room_cathedral_g20);
    room_cathedral_g16.add_room_exit("north", &room_cathedral_g9);
    room_cathedral_g15.add_room_exit("east", &room_cathedral_g16);
    room_cathedral_g15.add_room_exit("north", &room_cathedral_g8);
    room_cathedral_g18.add_room_exit("west", &room_cathedral_g17);
    room_cathedral_g18.add_room_exit("east", &room_cathedral_g19);
    room_cathedral_g18.add_room_exit("north", &room_cathedral_g11);
    room_cathedral_g18.add_room_exit("south", &room_cathedral_g22);
    room_cathedral_g19.add_room_exit("west", &room_cathedral_g18);
    room_cathedral_g19.add_room_exit("north", &room_cathedral_g12);
    room_cathedral_g10.add_room_exit("south", &room_cathedral_g17);
    room_cathedral_g10.add_room_exit("west", &room_cathedral_g9);
    room_cathedral_g10.add_room_exit("east", &room_cathedral_g11);
    room_cathedral_g10.add_room_exit("north", &room_cathedral_g4);
    room_cathedral_g9.add_room_exit("south", &room_cathedral_g16);
    room_cathedral_g9.add_room_exit("east", &room_cathedral_g10);
    room_cathedral_g9.add_room_exit("west", &room_cathedral_g8);
    room_cathedral_g9.add_room_exit("north", &room_cathedral_g3);
    room_cathedral_g8.add_room_exit("east", &room_cathedral_g9);
    room_cathedral_g8.add_room_exit("west", &room_cathedral_g7);
    room_cathedral_g8.add_room_exit("south", &room_cathedral_g15);
    room_cathedral_g7.add_room_exit("east", &room_cathedral_g8);
    room_cathedral_g7.add_room_exit("west", &room_cathedral_g6);
    room_cathedral_g6.add_room_exit("east", &room_cathedral_g7);
    room_cathedral_g11.add_room_exit("west", &room_cathedral_g10);
    room_cathedral_g11.add_room_exit("east", &room_cathedral_g12);
    room_cathedral_g11.add_room_exit("south", &room_cathedral_g18);
    room_cathedral_g11.add_room_exit("north", &room_cathedral_g5);
    room_cathedral_g12.add_room_exit("west", &room_cathedral_g11);
    room_cathedral_g12.add_room_exit("east", &room_cathedral_g13);
    room_cathedral_g12.add_room_exit("south", &room_cathedral_g19);
    room_cathedral_g13.add_room_exit("west", &room_cathedral_g12);
    room_cathedral_g13.add_room_exit("east", &room_cathedral_g14);
    room_cathedral_g14.add_room_exit("west", &room_cathedral_g13);
    room_cathedral_g4.add_room_exit("south", &room_cathedral_g10);
    room_cathedral_g4.add_room_exit("east", &room_cathedral_g5);
    room_cathedral_g4.add_room_exit("west", &room_cathedral_g3);
    room_cathedral_g4.add_room_exit("north", &room_cathedral_g2);
    room_cathedral_g3.add_room_exit("east", &room_cathedral_g4);
    room_cathedral_g3.add_room_exit("south", &room_cathedral_g9);
    room_cathedral_g5.add_room_exit("west", &room_cathedral_g4);
    room_cathedral_g5.add_room_exit("south", &room_cathedral_g11);
    room_cathedral_g2.add_room_exit("south", &room_cathedral_g4);
    room_cathedral_g2.add_room_exit("north", &room_cathedral_g1);
    room_cathedral_g1.add_room_exit("south", &room_cathedral_g2);
    room_cathedral_g6.add_room_exit("west", &room_brother_2_east);
    room_cathedral_g1.add_room_exit("north", &room_brother_1_south);
    room_cathedral_g14.add_room_exit("east", &room_brother_3_west);

    // Room 2

    // NPC

    NPC filius("The son", "The son sits in the middle of his room, looking for something...\n", 5,
               false, "wooden sword",
               "Please help me, I've lost my wooden sword! If you find it, I can give "
               "you something in return.",
               "", "That's it! Here you can have this.", &item_library["filius orbis"],
               &item_library["filius orbis"]);
    room_brother_2_middle.add_npc(filius);

    room_brother_2_east.add_room_exit("east", &room_cathedral_g6);
    room_brother_2_east.add_room_exit("north", &room_brother_2_northeast);
    room_brother_2_east.add_room_exit("south", &room_brother_2_southeast);
    room_brother_2_east.add_room_exit("west", &room_brother_2_middle);
    room_brother_2_northeast.add_room_exit("south", &room_brother_2_east);
    room_brother_2_northeast.add_room_exit("west", &room_brother_2_north);
    room_brother_2_southeast.add_room_exit("north", &room_brother_2_east);
    room_brother_2_southeast.add_room_exit("west", &room_brother_2_south);
    room_brother_2_middle.add_room_exit("east", &room_brother_2_east);
    room_brother_2_middle.add_room_exit("north", &room_brother_2_north);
    room_brother_2_middle.add_room_exit("south", &room_brother_2_south);
    room_brother_2_south.add_room_exit("north", &room_brother_2_middle);
    room_brother_2_south.add_room_exit("east", &room_brother_2_southeast);
    room_brother_2_north.add_room_exit("east", &room_brother_2_northeast);
    room_brother_2_north.add_room_exit("south", &room_brother_2_middle);

    // Room 3

    // NPC
    NPC mater("The mother", "The mother sits in the middle of the room, painting something...\n", 5,
              false, "mother's heart",
              "Don't speak to me while I'm painting...\nComeback once you've got "
              "something worthwhile...",
              "",
              "Oh...that's my old project.\nI've taken my heart and sacrificed "
              "it to our savior!\nYou should try it sometime...",
              &item_library["mater orbis"], &item_library["mater orbis"]);
    room_brother_3_middle.add_npc(mater);

    room_brother_3_west.add_room_exit("west", &room_cathedral_g14);
    room_brother_3_west.add_room_exit("north", &room_brother_3_northwest);
    room_brother_3_west.add_room_exit("south", &room_brother_3_southwest);
    room_brother_3_west.add_room_exit("east", &room_brother_3_middle);
    room_brother_3_southwest.add_room_exit("north", &room_brother_3_west);
    room_brother_3_southwest.add_room_exit("east", &room_brother_3_south);
    room_brother_3_south.add_room_exit("west", &room_brother_3_southwest);
    room_brother_3_south.add_room_exit("north", &room_brother_3_middle);
    room_brother_3_middle.add_room_exit("west", &room_brother_3_west);
    room_brother_3_middle.add_room_exit("north", &room_brother_3_north);
    room_brother_3_middle.add_room_exit("south", &room_brother_3_south);
    room_brother_3_northwest.add_room_exit("south", &room_brother_3_west);
    room_brother_3_northwest.add_room_exit("east", &room_brother_3_north);
    room_brother_3_north.add_room_exit("west", &room_brother_3_northwest);
    room_brother_3_north.add_room_exit("south", &room_brother_3_middle);

    // Room 1

    // NPC
    NPC pater("The father", "The father sits upon a throne of blood...\n", 5, false,
              "blood necklace",
              "Someone stole my blood necklace...\nWhen I find out who did it, "
              "I'm going to tear their head out of their fleshed body!",
              "",
              "You've found it...who had it???\nWas it the priest?\nNo matter, "
              "here take this and start your ascent...",
              &item_library["pater orbis"], &item_library["pater orbis"]);
    room_brother_1_middle.add_npc(pater);

    room_brother_1_south.add_room_exit("south", &room_cathedral_g1);
    room_brother_1_south.add_room_exit("west", &room_brother_1_southwest);
    room_brother_1_south.add_room_exit("east", &room_brother_1_southeast);
    room_brother_1_south.add_room_exit("north", &room_brother_1_middle);
    room_brother_1_southwest.add_room_exit("east", &room_brother_1_south);
    room_brother_1_southwest.add_room_exit("north", &room_brother_1_west);
    room_brother_1_southeast.add_room_exit("west", &room_brother_1_south);
    room_brother_1_southeast.add_room_exit("north", &room_brother_1_east);
    room_brother_1_middle.add_room_exit("south", &room_brother_1_south);
    room_brother_1_middle.add_room_exit("west", &room_brother_1_west);
    room_brother_1_middle.add_room_exit("east", &room_brother_1_east);
    room_brother_1_west.add_room_exit("east", &room_brother_1_middle);
    room_brother_1_west.add_room_exit("south", &room_brother_1_southwest);
    room_brother_1_east.add_room_exit("west", &room_brother_1_middle);
    room_brother_1_east.add_room_exit("south", &room_brother_1_southeast);

    // Player's current Room
    Room *room_current = &room_start;
    Player player;

    room_current->print_description();

    std::string player_action;

    while (true) {
        if (!player.is_alive) {
            std::cout << "\nYou died...\n";
            break;
        }
        if (check_victory(player)) {
            break;
        }

        std::cout << "\nACTION: ";
        std::getline(std::cin >> std::ws, player_action);
        player_action = to_lowercase(player_action);
        std::cout << "\n";

        if (player_action.find("search") != std::string::npos ||
            player_action.find("find") != std::string::npos ||
            player_action.find("look") != std::string::npos) {
            room_current->print_search_description();

        } else if (player_action.find("take") != std::string::npos) {
            if (room_current->has_been_searched_by_player() &&
                !room_current->revealed_item_name.empty()) {
                Item *item = room_current->find_item(room_current->revealed_item_name);
                if (item) {
                    player.add_to_inventory(*item);
                    room_current->remove_item(item->item_name);
                    room_current->revealed_item_name.clear(); // prevent double-take
                } else {
                    std::cout << "The item is no longer here.\n";
                }
            } else {
                std::cout << "You see nothing to take.\nTry searching first...\n\n";
            }
        } else if (player_action.find("inventory") != std::string::npos) {
            player.print_inventory();

        } else if (player_action.find("open") != std::string::npos ||
                   player_action.find("use key") != std::string::npos) {
            if (room_current->chest && !room_current->chest->is_opened()) {
                if (room_current->chest->is_locked()) {
                    if (room_current->chest->can_unlock(player.player_inventory)) {
                        std::cout << "You unlock the chest using the "
                                  << room_current->chest->get_required_key() << ".\n\n";
                        room_current->chest->unlock();
                    } else {
                        std::cout << "The chest is locked.\n\n";
                        continue;
                    }
                }
                Item found_item = room_current->chest->open();
                std::cout << "You open the chest and found... " << found_item.item_name << "!\n\n";
                player.add_to_inventory(found_item);
                if (check_victory(player)) {
                    break;
                };
            } else {
                try_open_door(room_current, player);
            }

        } else if (!extract_direction(player_action).empty()) {
            std::string dir = extract_direction(player_action);
            attempt_move(room_current, dir);

        } else if (player_action.find("talk") != std::string::npos ||
                   player_action.find("ask") != std::string::npos) {
            talk_to_npc(room_current);

        } else if (player_action.find("give") != std::string::npos) {
            size_t item_position = player_action.find(" ");

            if (item_position != std::string::npos) {
                std::string item_name =
                    player_action.substr(item_position + 1); // Extract item position
                give_item_to_npc(room_current, player, item_name);
            } else {
                std::cout << "Give what?\n";
            }

        } else if (player_action.find("kill self") != std::string::npos ||
                   player_action.find("kill myself") != std::string::npos ||
                   player_action.find("suicide") != std::string::npos) {
            if (player.has_item("rusted knife")) {
                std::cout << "You can't handle the darkness...\nYou take the rusted "
                             "knife and plunge it deep into stomach...\n";
                player.player_dies();
            } else if (player.has_item("obsidian dagger")) {
                std::cout << "The dagger speaks to you...\nIt wants you...\nYou hear "
                             "the voices that come before...\nYou look to the ceiling "
                             "and plunge the dagger into your stomach...\n";
                player.player_dies();
            } else {
                std::cout << "You have nothing to kill yourself with...\n";
            }

        } else if (player_action.find("attack") != std::string::npos ||
                   player_action.find("kill") != std::string::npos) {
            if (!room_current->npcs.empty()) {
                std::string npc_name =
                    room_current->npcs[0].name; // Get the name of the first NPC in the room
                attack_npc(room_current, player, npc_name);
            } else {
                std::cout << "There is no one to attack...\n";
            }

        } else if (player_action.find("drink blood bottle") != std::string::npos) {
            if (player.has_item("blood bottle")) {
                std::cout << "You begin to drink the blood bottle...\nYou feel the thick "
                             "coagulated blood slide down your throat...\nAt first your body "
                             "wanted to reject it, but after you drink...\nand drink...\nand "
                             "drink...\nYou begin to feel something else...\nBliss...\n";
                player.player_dies();

            } else {
                std::cout << "You don't have a blood bottle in your inventory.\n\n";
            }
        } else if (player_action == "quit" || player_action == "exit" ||
                   player_action == "quit game") {
            std::cout << "You decide it's time to stop. Returning to the Main "
                         "Menu.\n";
            break;

        } else {
            std::cout << "You can't do that right now. \nTry search, "
                         "inventory, north, south, east, west, or quit\n\n";
        }
    }
}

void show_menu() {
    std::cout << "\n\n";
    print_centered("<============================>");
    print_centered("TENEBRAE");
    print_centered("Main Menu");
    std::cout << "\n";
    print_centered("    [1] New Game      ");
    print_centered("    [2] Quit          ");
    std::cout << "\n";
    print_centered("Enter [1] or [2]");
    print_centered(" <============================>\n");
}

void quit_game(bool &game_running) {
    std::cout << "\nQuitting...\n";
    game_running = false;
}

int main() {
    int menu_choice{};
    bool game_running{true};

    while (game_running) {
        show_menu();
        std::cout << "ACTION: ";
        if (!(std::cin >> menu_choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter 1 OR 2.\n";
            continue;
        }
        switch (menu_choice) {
        case 1:
            start_new_game();
            break;
        case 2:
            quit_game(game_running);
            break;
        default:
            std::cout << "Invalid choice\n";
        }
    }
}
