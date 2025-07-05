module;

export module meta_forge.forge_phase;

import core;

export enum class ForgePhase : u8 {
    None = 0,
    ParseCommands = 1,
    RunTools = 2,
    GenerateData = 3,
    RenderTemplate = 4,
    Finish = 5,
};