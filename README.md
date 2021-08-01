# LED Matrix
Experiments with a controllable RGB LED matrix. This application currently runs an exercise interval timer application that displays the current interval time via the LED matrix. Timers are started through a local application TCP server that accepts specifically formatted JSON packets that contain the timer configuration payload. This project was a good learning experience in integrating third-party libraries into a C++ project that requires cross-compilation for an embedded target. 

Libraries included:
>- [Boost](https://www.boost.org/): Primarily using Boost ASIO for networking drivers as this is cross-platform and much more elegant than using low-level linux sockets
>- [JSON for Modern C++](https://github.com/nlohmann/json): A wonderful JSON library for modern C++ style JSON usage
>- [fmt](https://github.com/fmtlib/fmt): An excellent library for better C++ IO formatting
>- [range-v3](https://github.com/ericniebler/range-v3): The precursor library to C++20 ranges and a super useful functional programming toolkit for expressing algorithms in more compact code
>- [rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix): A driver for the led-matrix for Raspberry Pi



