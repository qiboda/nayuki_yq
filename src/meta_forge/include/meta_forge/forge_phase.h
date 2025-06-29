#pragma once

#include "core/core.h"

enum class ForgePhase : u8
{
    None = 0,
    ParseCommands = 1,
    RunTools = 2,
    GenerateData = 3,
    RenderTemplate = 4,
    Finish = 5,
};