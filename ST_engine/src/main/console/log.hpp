#ifndef LOG_HPP
#define LOG_HPP

#include <console/console_log.hpp>

#ifdef __DEBUG
    #define log(TYPE, MSG) gMessage_bus->send_msg(make_msg(CONSOLE_WRITE, make_data<ST::console_log>(ST::console_log(log_type ::TYPE, MSG))));
#elif defined(__RELEASE)
    #define log(TYPE, MSG)
#endif

#endif
