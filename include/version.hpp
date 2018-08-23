#pragma once
#ifndef VERSION_H
#define VERSION_H

#ifndef SIZEOF
#define SIZEOF( X ) sizeof( X ) / sizeof( X[0] )
#endif // SIZEOF

// std
#include <iostream>
#include <cmath>

// library

// dekwan library


#define FULL_VERSION "Bluetooth 0.18.08.23.23"
#define Bluetooth_VERSION "0.18.08.23.23"
#define Bluetooth_VERSION_SHORT "0.18.08"
#define Bluetooth_VERSION_MAJOR 18
#define Bluetooth_VERSION_MINOR 08
#define Bluetooth_VERSION_RELEASE 23
#define Bluetooth_VERSION_BUILD 23

const std::string PROJEK_DIR = "/home/sarah/.Bluetooth/";

#endif /* VERSION_H */
