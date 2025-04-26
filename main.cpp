#include <algorithm>
#include <iostream>
#include <map>

#include <vector>

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

class Player {
public:
    std::vector<Item> player_inventory;

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
};

class Room {
public:
    std::string room_description;
    std::string search_description;
    std::map<std::string, Room *> room_exits;
    std::map<std::string, std::string> locked_exits;
    std::vector<Item> items;
    bool has_been_searched = false;
    std::string revealed_item_name;

    Room(const std::string &desc, const std::string &search = "")
        : room_description(desc), search_description(search) {}

    void add_room_exit(const std::string &direction, Room *room) { room_exits[direction] = room; }

    Room *get_exit(const std::string &direction) {
        auto i = room_exits.find(direction);
        return i != room_exits.end() ? i->second : nullptr;
    }

    void lock_exit(const std::string &direction, const std::string &required_key) {
        locked_exits[direction] = to_lowercase(required_key);
    }

    bool is_exit_locked(const std::string &direction) const {
        return locked_exits.find(direction) != locked_exits.end();
    }

    bool can_unlock(const std::string &direction, const std::vector<Item> &inventory) const {
        auto i = locked_exits.find(direction);
        if (i != locked_exits.end()) {
            for (const auto &item : inventory) {
                if (to_lowercase(item.item_name) == i->second) {
                    return true;
                }
            }
        }
        return false;
    }

    void unlock_exit(const std::string &direction) { locked_exits.erase(direction); }

    void print_description() { std::cout << room_description << "\n"; }

    void print_search_description() {
        has_been_searched = true;
        if (!search_description.empty()) {
            std::cout << search_description << "\n";
        } else {
            std::cout << "You find nothing of interest.\n\n";
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
    };
};

void attempt_move(Room *&room_current, const std::string &direction, Player &player) {
    if (room_current->get_exit(direction)) {
        if (room_current->is_exit_locked(direction)) {
            if (room_current->can_unlock(direction, player.player_inventory)) {
                std::cout << "You use a key and unlock the door...\n\n";
                room_current->unlock_exit(direction);
            } else {
                std::cout << "The door is locked. Maybe there's a key nearby...\n\n";
                return;
            }
        }
        room_current = room_current->get_exit(direction);
        room_current->print_description();
    } else {
        std::cout << "You can't go that way.\n\n";
    }
}

// Global items
std::map<std::string, Item> item_library = {
    {"rusted knife",
     Item("rusted knife", "A slightly dull rusted knife. Could be useful later...")},
    {"cell key", Item("cell key", "A small iron key.")}};

void start_new_game() {
    std::cout << "\nInitializing TENEBRAE...\n\nYou wake up in a room dimly "
                 "lit by a torch...\n";

    Room room_start("You stand in the middle of the cell room...\n",
                    "You look around the room and see a cell door to the NORTH "
                    "wall, a dirty ragged bed on the floor to the EAST wall, "
                    "and a dirty bucket to the SOUTH wall.\n");
    Room room_start_north("You face the cell door...\n");
    Room room_start_south("You look down and see a bucket filled with who knows what...\n",
                          "You hesitate before plunging your hand into the sludge-filled bucket. "
                          "You feel something cold and slimy...\n");
    room_start_south.revealed_item_name = "cell key";
    Room room_start_east("You look down at the dirty ragged bed... it seems just a minute ago "
                         "you were having the worst nightmare...\n",
                         "You feel under the bed...\n");
    room_start_east.revealed_item_name = "rusted knife";
    Room room_start_west("You stare blankly at the wall...\n");
    Room room_start_northeast("You face the NORTH EAST corner of the room.\n");
    Room room_start_northwest("You face the NORTH WEST corner of the room.\n");
    Room room_start_southeast("You face the SOUTH EAST corner of the room.\n");
    Room room_start_southwest("You face the SOUTH WEST corner of the room.\n");
    Room room_prison_hallway("You step through the cell door into a dark hallway. The hallway "
                             "stretches into the darkness...\n");

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

    room_start_south.add_item(item_library["cell key"]);
    room_start_east.add_item(item_library["rusted knife"]);

    room_start_north.lock_exit("north", "cell key");

    room_start_north.add_room_exit("north", &room_prison_hallway);
    room_prison_hallway.add_room_exit("south", &room_start_north);

    Room *room_current = &room_start;
    Player player;

    room_current->print_description();

    std::string player_action;

    while (true) {
        std::cout << "\nACTION: ";
        std::getline(std::cin >> std::ws, player_action);
        player_action = to_lowercase(player_action);
        std::cout << "\n";

        if (player_action.find("search") != std::string::npos ||
            player_action.find("find") != std::string::npos) {
            room_current->print_search_description();
            if (!room_current->revealed_item_name.empty()) {
                std::cout << "You found a " << room_current->revealed_item_name << ".\n";
                std::cout << "\nType 'take' to pick it up.\n\n";
            }

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
                std::cout << "You see nothing to take.\n\n";
            }
        } else if (player_action.find("inventory") != std::string::npos) {
            player.print_inventory();
        } else if (!extract_direction(player_action).empty()) {
            std::string dir = extract_direction(player_action);
            attempt_move(room_current, dir, player);
        } else if (player_action == "quit" || player_action == "exit" ||
                   player_action == "quit game") {
            std::cout << "You decide it's time to stop. Returning to the Main "
                         "Menu.\n";
            break;
        } else {
            std::cout << "You can't do that right now. \nTry search, "
                         "inventory, north, south, east, west, or quit\n";
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
    print_centered("Enter 1 or 2");
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
        std::cin >> menu_choice;
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
