#pragma once

#include "meta_forge/command_list_parser.h"
#include "meta_forge/forge_phase.h"

class META_FORGE_API Forge
{
  public:
    Forge()
    {
    }

    void Init( int argc, const char** argv );

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
    i32 mArgc = 0;
    const char** mArgv = nullptr;

    std::shared_ptr<struct MetaInfoManager> mMetaInfoManager;

    class CommandListParser* mCommandListParser = nullptr;
};