module;

#include <module_export.h>
#include <meta_forge/meta_forge.h>

export module meta_forge.command_list_parser;

import core;

export class META_FORGE_API CommandListParser
{
  public:
    CommandListParser( int& argc, const char** argv );

  public:
    /// A factory method that is similar to the above constructor, except
    /// this returns an error instead exiting the program on error.
    static llvm::Expected<CommandListParser> create( int& argc, const char** argv );

    const std::string GetTargetName() const
    {
        return mTargetName;
    }

  private:
    CommandListParser() = default;

  private:
    llvm::Error init( int& argc, const char** argv );

    std::string mTargetName;
};