#pragma once

#include <meta_forge/meta_forge.h>
#include <core/core.h>

class META_FORGE_API CommandListParser
{
  public:
    CommandListParser( int& argc, const char** argv );

  public:
    /// A factory method that is similar to the above constructor, except
    /// this returns an error instead exiting the program on error.
    static llvm::Expected<CommandListParser> create( int& argc, const char** argv );

    /// Returns a reference to the loaded compilations database.
    clang::tooling::CompilationDatabase& getCompilations()
    {
        return *Compilations;
    }

    /// Returns a list of source file paths to process.
    const std::vector<std::string>& getSourcePathList() const
    {
        return SourcePathList;
    }

    const std::string GetModuleFolder() const
    {
        return mModuleFolder;
    }

    const std::string GetCompilationsFolder() const
    {
        return mCompilationsFolder;
    }

  private:
    CommandListParser() = default;

    void CollectSourceFiles( const std::string& path );

  private:
    llvm::Error init( int& argc, const char** argv );

    std::unique_ptr<clang::tooling::CompilationDatabase> Compilations;
    std::vector<std::string> SourcePathList;

    std::string mModuleFolder;

    std::string mCompilationsFolder;
};