

#ifndef ARDUINO_BOARDS_H
#define ARDUINO_BOARDS_H

#if ARDUINO_BOARD == arduino_nano
    #include "arduino_nano_cfg.h"

#elif ARDUINO_BOARD == arduino_pro_mini_5v
    #include "arduino_pro_mini.h"

#elif ARDUINO_BOARD == arduino_pro_mini_3v3
    #include "arduino_pro_mini.h"

#endif

// include generic functions after board config
// #include ...

#endif /* ARDUINO_BOARDS_H */
