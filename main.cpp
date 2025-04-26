#include "descriptions.hpp"
#include <algorithm>
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
    std::map<std::string, Door> doors;
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

    void add_door(const std::string &direction, const Door &door) { doors[direction] = door; }

    bool has_door(const std::string &direction) const {
        return doors.find(direction) != doors.end();
    }

    Door *get_door(const std::string &direction) {
        auto it = doors.find(direction);
        return it != doors.end() ? &(it->second) : nullptr;
    }

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

// Global items
std::map<std::string, Item> item_library = {
    {"rusted knife", Item("rusted knife", descriptions::ITEM_RUSTED_KNIFE)},
    {"cell key", Item("cell key", descriptions::ITEM_CELL_KEY)},
    {"room key", Item("room key", descriptions::ITEM_ROOM_KEY)}};

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
    room_start_south.revealed_item_name = "cell key";
    Room room_start_east(descriptions::ROOM_START_EAST, descriptions::SEARCH_EAST);
    room_start_east.revealed_item_name = "rusted knife";
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
    Room room_prison_hallway_5(descriptions::ROOM_PRISON_HALLWAY_5);
    Room room_prison_1_middle(descriptions::ROOM_PRISON_1_MIDDLE);
    Room room_prison_1_north(descriptions::ROOM_PRISON_1_NORTH);
    Room room_prison_1_south(descriptions::ROOM_PRISON_1_SOUTH);
    Room room_prison_1_east(descriptions::ROOM_PRISON_1_EAST);
    Room room_prison_1_west(descriptions::ROOM_PRISON_1_WEST);
    Room room_prison_1_northeast(descriptions::ROOM_PRISON_1_NORTHEAST);
    Room room_prison_1_northwest(descriptions::ROOM_PRISON_1_NORTHWEST);
    Room room_prison_1_southeast(descriptions::ROOM_PRISON_1_SOUTHEAST,
                                 descriptions::SEARCH_ROOM_PRISON_1_SOUTHEAST);
    Room room_prison_1_southwest(descriptions::ROOM_PRISON_1_SOUTHWEST);

    // Starting Room Area
    room_start.add_room_exit("north", &room_start_north);
    room_start.add_room_exit("south", &room_start_south);
    room_start.add_room_exit("east", &room_start_east);
    room_start.add_room_exit("west", &room_start_west);

    room_start_north.add_door("north", Door("cell key"));
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

    // Items in Starting Room
    room_start_south.add_item(item_library["cell key"]);
    room_start_east.add_item(item_library["rusted knife"]);

    // Prison Hallway Area
    room_start_north.add_room_exit("north", &room_prison_hallway_1);
    room_prison_hallway_1.add_room_exit("south", &room_start_north);
    room_prison_hallway_1.add_room_exit("north", &room_prison_hallway_2);
    room_prison_hallway_2.add_room_exit("south", &room_prison_hallway_1);
    room_prison_hallway_2.add_room_exit("north", &room_prison_hallway_3);
    room_prison_hallway_3.add_room_exit("south", &room_prison_hallway_2);
    room_prison_hallway_3.add_room_exit("north", &room_prison_hallway_4);
    room_prison_hallway_4.add_room_exit("south", &room_prison_hallway_3);
    room_prison_hallway_4.add_room_exit("north", &room_prison_1_south);
    room_prison_hallway_3.add_room_exit("west", &room_prison_hallway_5);
    room_prison_hallway_5.add_room_exit("east", &room_prison_hallway_3);

    // Prison Room 1
    room_prison_1_south.add_room_exit("south", &room_prison_hallway_4);
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

        } else if (player_action.find("open") != std::string::npos ||
                   player_action.find("use key") != std::string::npos) {
            try_open_door(room_current, player);

        } else if (!extract_direction(player_action).empty()) {
            std::string dir = extract_direction(player_action);
            attempt_move(room_current, dir);

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
