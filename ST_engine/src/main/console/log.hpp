/* Copyright (C) 2018 Maxim Atanasov - All Rights Reserved
 * You may not use, distribute or modify this code.
 * This code is proprietary and belongs to the "slavicTales"
 * project. See LICENCE.txt in the root directory of the project.
 *
 * E-mail: atanasovmaksim1@gmail.com
 */

#ifndef LOG_HPP
#define LOG_HPP

#include <console/console_log.hpp>

//A macro that will a send a log message to the console window.
//Requires the gMessage_bus object to be defined and pointing to the global message bus.

#ifdef __DEBUG
    #define log(TYPE, MSG) gMessage_bus->send_msg(make_msg(CONSOLE_WRITE, make_data<ST::console_log>(ST::console_log(ST::log_type ::TYPE, MSG))));
#elif defined(__RELEASE)
    #define log(TYPE, MSG)
#endif

#endif
