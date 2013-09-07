#ifndef GLOBALS_H
#define GLOBALS_H

/**
 * Evil global variables needed for scripting access.
 */

#include "World.h"
extern World g_World;

void DisplayTextFromFile(const std::string& path);

#endif //GLOBALS_H
