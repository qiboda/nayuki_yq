module;

#include <module_export.h>

export module meta_forge.forge;

import meta_forge.meta_info;
import core;
import std;
import meta_forge.forge_phase;
import meta_forge.command_list_parser;

export class META_FORGE_API Forge
{
  public:
    Forge()
    {
    }

    void Init( usize argc, const char** argv );

    void Clear()
    {
    }

  public:
    void RunPhase( ForgePhase phase );

  protected:
    void RunParseCommands();

    void RunRunTools();

    void RunGenerateData();

    void RunRenderTemplate();

  protected:
    ForgePhase mPhase = ForgePhase::None;
    usize mArgc = 0;
    const char** mArgv = nullptr;

    std::shared_ptr<MetaInfoManager> mMetaInfoManager;

    CommandListParser* mCommandListParser = nullptr;
};