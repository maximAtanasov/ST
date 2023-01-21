/* This file is part of the "ST" project.
 * You may use, distribute or modify this code under the terms
 * of the GNU General Public License version 2.
 * See LICENCE.txt in the root directory of the project.
 *
 * Author: Maxim Atanasov
 * E-mail: maxim.atanasov@protonmail.com
 */

#include <game_manager/level/level.hpp>
#include <fstream>
#include <sstream>
#include <ST_util/string_util.hpp>

/**
 * Constructor for the level.
 * @param lvl_name The name of the level;
 * @param msg_bus A pointer to the global message bus.
 */
ST::level::level(const std::string &lvl_name, message_bus *msg_bus) {
    name = lvl_name;
    gMessage_bus = msg_bus;
}

/**
 * Loads the level.
 * Sends a LOAD_LIST message to load the assets.list in the directory of the level.
 * @return -1 if the input configuration could not be loaded, 0 on success
 */
int8_t ST::level::load() {
    if (load_input_conf() != 0) {
        return -1;
    }
    //Register all the keys this level uses with the input manager.
    for (const auto &i: actions_buttons) {
        for (const auto &key: i.second) {
            if (key != ST::key::UNKNOWN) {
                gMessage_bus->send_msg(new message(REGISTER_KEY, static_cast<uint8_t>(key), nullptr));
            }
        }
    }
    std::string temp = "game/levels/" + name + "/assets.list";
    gMessage_bus->send_msg(new message(LOAD_LIST, make_data(temp)));
    return 0;
}

/**
 * reloads the level.
 */
void ST::level::reload() {
    std::string temp = "game/levels/" + name + "/assets.list";
    gMessage_bus->send_msg(new message(UNLOAD_LIST, make_data(temp)));
    for (const auto &i: actions_buttons) {
        for (const auto &key: i.second) {
            if (key != ST::key::UNKNOWN) {
                gMessage_bus->send_msg(new message(UNREGISTER_KEY, static_cast<uint8_t>(key), nullptr));
            }
        }
    }
    actions_buttons.clear();
    gMessage_bus->send_msg(new message(LOAD_LIST, make_data(temp)));
    load_input_conf();
    for (const auto &i: actions_buttons) {
        for (const auto &key: i.second) {
            if (key != ST::key::UNKNOWN) {
                gMessage_bus->send_msg(new message(REGISTER_KEY, static_cast<uint8_t>(key), nullptr));
            }
        }
    }
}

/**
 * Get the name of the level.
 * @return The name of the level.
 */
std::string ST::level::get_name() const {
    return name;
}

/**
 * Destroys the level.
 * Frees all data.
 */
ST::level::~level() {
    //unload inputConf
    actions_buttons.clear();

    //unload entities.
    entities.clear();

    //unload text objects
    text_objects.clear();

    //unload lights
    lights.clear();
}

/**
 * Unloads a level.
 * Sends a UNLOAD_LIST message to unload all assets and unregisters all keys.
 */
void ST::level::unload() {
    for (const auto &i: actions_buttons) {
        for (const auto &key: i.second) {
            if (key != ST::key::UNKNOWN) {
                gMessage_bus->send_msg(new message(UNREGISTER_KEY, static_cast<uint8_t>(key), nullptr));
            }
        }
    }
    //unload assets
    std::string temp = "game/levels/" + name + "/assets.list";
    gMessage_bus->send_msg(new message(UNLOAD_LIST, make_data(temp)));

    //unload inputConf
    actions_buttons.clear();

    //unload entities
    entities.clear();

    //unload text objects
    text_objects.clear();

    //unload lights
    lights.clear();
}

/**
 * Loads the input configuration of the level.
 * File inputConf.cfg must exist.
 * @return -1 on error or 0 on success.
 */
int8_t ST::level::load_input_conf() {
    std::ifstream file;
    std::string temp = "game/levels/" + name + "/inputConf.cfg";
    file.open(temp.c_str());
    if (file.is_open()) {
        gMessage_bus->send_msg(new message(LOG_INFO, make_data<std::string>("Loading " + temp)));
        std::string action;
        std::string button;
        int actionRead = 0;
        while (!file.eof()) {
            auto a = static_cast<char>(file.get());
            //Ignore comments
            if (a == '#') {
                while (a != '\n') {
                    a = static_cast<char>(file.get());;
                }
            } else if (a == '=') {
                actionRead = 1;
            } else if (actionRead == 0 && a == '\n') { //ignore empty lines
                continue;
            } else if (actionRead == 0) {
                action += a;
            } else if (a == '\n' || file.eof()) {
                std::istringstream buf(button);
                std::istream_iterator<std::string> beg(buf), end;
                std::vector<std::string> tokens(beg, end);
                actions_buttons.emplace(ST::hash_string(action), std::vector<ST::key>());
                std::vector<ST::key> *buttons_for_action = &actions_buttons.at(ST::hash_string(action));
                for (const auto &token: tokens) {
                    buttons_for_action->emplace_back(key_index(token));
                }
                actionRead = 0;
                action.clear();
                button.clear();
            } else {
                button += a;
            }
        }
        file.close();
    } else {
        gMessage_bus->send_msg(new message(LOG_ERROR, make_data<std::string>("File " + temp + " not found")));
        return -1;
    }
    return 0;
}

/**
 * Indexes the keys (for when they are loaded by the level to avoid string comparisons later).
 * @param arg the key string as read from inputConf.cfg.
 * @return a <b>ST::key</b> enum (an uint8_t).
 */
ST::key ST::level::key_index(const std::string &arg) {
    key index = key::UNKNOWN;
    if (arg == "left") {
        index = key::LEFT;
    } else if (arg == "right") {
        index = key::RIGHT;
    } else if (arg == "up") {
        index = key::UP;
    } else if (arg == "down") {
        index = key::DOWN;
    } else if (arg == "a") {
        index = key::A;
    } else if (arg == "b") {
        index = key::B;
    } else if (arg == "c") {
        index = key::C;
    } else if (arg == "d") {
        index = key::D;
    } else if (arg == "e") {
        index = key::E;
    } else if (arg == "f") {
        index = key::F;
    } else if (arg == "g") {
        index = key::G;
    } else if (arg == "h") {
        index = key::H;
    } else if (arg == "i") {
        index = key::I;
    } else if (arg == "j") {
        index = key::J;
    } else if (arg == "k") {
        index = key::K;
    } else if (arg == "l") {
        index = key::L;
    } else if (arg == "m") {
        index = key::M;
    } else if (arg == "n") {
        index = key::N;
    } else if (arg == "o") {
        index = key::O;
    } else if (arg == "p") {
        index = key::P;
    } else if (arg == "q") {
        index = key::Q;
    } else if (arg == "r") {
        index = key::R;
    } else if (arg == "s") {
        index = key::S;
    } else if (arg == "t") {
        index = key::T;
    } else if (arg == "u") {
        index = key::U;
    } else if (arg == "v") {
        index = key::V;
    } else if (arg == "w") {
        index = key::W;
    } else if (arg == "x") {
        index = key::X;
    } else if (arg == "y") {
        index = key::Y;
    } else if (arg == "z") {
        index = key::Z;
    } else if (arg == "1") {
        index = key::ONE;
    } else if (arg == "2") {
        index = key::TWO;
    } else if (arg == "3") {
        index = key::THREE;
    } else if (arg == "4") {
        index = key::FOUR;
    } else if (arg == "5") {
        index = key::FIVE;
    } else if (arg == "6") {
        index = key::SIX;
    } else if (arg == "7") {
        index = key::SEVEN;
    } else if (arg == "8") {
        index = key::EIGHT;
    } else if (arg == "9") {
        index = key::NINE;
    } else if (arg == "0") {
        index = key::ZERO;
    } else if (arg == "escape") {
        index = key::ESCAPE;
    } else if (arg == "enter") {
        index = key::ENTER;
    } else if (arg == "spacebar") {
        index = key::SPACEBAR;
    } else if (arg == "tilde") {
        index = key::TILDE;
    } else if (arg == "lshift") {
        index = key::LSHIFT;
    } else if (arg == "backspace") {
        index = key::BACKSPACE;
    } else if (arg == "backslash") {
        index = key::BACKSLASH;
    } else if (arg == "capslock") {
        index = key::CAPSLOCK;
    } else if (arg == "comma") {
        index = key::COMMA;
    } else if (arg == "equals") {
        index = key::EQUALS;
    } else if (arg == "lalt") {
        index = key::LALT;
    } else if (arg == "lctrl") {
        index = key::LCTRL;
    } else if (arg == "lbracket") {
        index = key::LBRACKET;
    } else if (arg == "rbracket") {
        index = key::RBRACKET;
    } else if (arg == "minus") {
        index = key::MINUS;
    } else if (arg == "delete") {
        index = key::DELETE;
    } else if (arg == "ralt") {
        index = key::RALT;
    } else if (arg == "rctrl") {
        index = key::RCTRL;
    } else if (arg == "semicolon") {
        index = key::SEMICOLON;
    } else if (arg == "slash") {
        index = key::SLASH;
    } else if (arg == "tab") {
        index = key::TAB;
    } else if (arg == "mouseLeft") {
        index = key::MOUSELEFT;
    } else if (arg == "mouseMiddle") {
        index = key::MOUSEMIDDLE;
    } else if (arg == "mouseRight") {
        index = key::MOUSERIGHT;
    } else if (arg == "controllerA") {
        index = key::CONTROLLER_BUTTON_A;
    } else if (arg == "controllerB") {
        index = key::CONTROLLER_BUTTON_B;
    } else if (arg == "controllerX") {
        index = key::CONTROLLER_BUTTON_X;
    } else if (arg == "controllerY") {
        index = key::CONTROLLER_BUTTON_Y;
    } else if (arg == "controllerSelect") {
        index = key::CONTROLLER_BUTTON_SELECT;
    } else if (arg == "controllerStart") {
        index = key::CONTROLLER_BUTTON_START;
    } else if (arg == "controllerLeftStick") {
        index = key::CONTROLLER_BUTTON_LEFTSTICK;
    } else if (arg == "controllerRightStick") {
        index = key::CONTROLLER_BUTTON_RIGHTSTICK;
    } else if (arg == "controllerLeftShoulder") {
        index = key::CONTROLLER_BUTTON_LEFTSHOULDER;
    } else if (arg == "controllerRightShoulder") {
        index = key::CONTROLLER_BUTTON_RIGHTSHOULDER;
    } else if (arg == "controllerDpadUp") {
        index = key::CONTROLLER_BUTTON_DPAD_UP;
    } else if (arg == "controllerDpadDown") {
        index = key::CONTROLLER_BUTTON_DPAD_DOWN;
    } else if (arg == "controllerDpadLeft") {
        index = key::CONTROLLER_BUTTON_DPAD_LEFT;
    } else if (arg == "controllerDpadRight") {
        index = key::CONTROLLER_BUTTON_DPAD_RIGHT;
    } else if (arg == "controllerRightTrigger") {
        index = key::CONTROLLER_BUTTON_RIGHT_TRIGGER;
    } else if (arg == "controllerLeftTrigger") {
        index = key::CONTROLLER_BUTTON_LEFT_TRIGGER;
    }
    return index;
}
