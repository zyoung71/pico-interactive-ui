#pragma once

#include "DisplayInterface.h"

class ScreenManager;
class Screen;

struct ScreenData
{
    DisplayInterface* display;
    ScreenManager* manager;
    Screen* screen;
};