module;

export module meta_forge.forge_phase;

import core;

export enum class ForgePhase : u8 {
    None = 0,
    LoadCompileArgs = 1,
    ParseCommands = 2,
    BuildPcmFiles = 3,
    GenerateCompileCommands = 4,
    RunTools = 5,
    GenerateData = 6,
    RenderTemplate = 7,
    Finish = 8,
};