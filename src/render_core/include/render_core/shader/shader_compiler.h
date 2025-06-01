#pragma once

#include <render_core/minimal.h>
#include <core/minimal.h>

#include <shaderc/shaderc.hpp>

class RENDER_CORE_API ShaderCompiler : public Singleton<ShaderCompiler>, public IRAII
{
  public:
    ShaderCompiler();
    virtual ~ShaderCompiler() override;

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }

#pragma region ShaderCompiler
  public:
    // Returns GLSL shader source text after preprocessing.
    std::string PreprocessShader( const std::string& source_name, shaderc_shader_kind kind, const std::string& source );

    // Compiles a shader to SPIR-V assembly. Returns the assembly text
    // as a string.
    std::string CompileFileToAssembly( const std::string& source_name,
                                       shaderc_shader_kind kind,
                                       const std::string& source,
                                       bool optimize = false );

    // Compiles a shader to a SPIR-V binary. Returns the binary as
    // a vector of 32-bit words.
    std::vector<u32> CompileFile( const std::string& source_name,
                                  shaderc_shader_kind kind,
                                  const std::string& source,
                                  bool optimize = false );
#pragma endregion // ShaderCompiler

  public:
    // Adds a macro definition to the compiler options.
    void AddMacroDefinition( const std::string& name, const std::string& value )
    {
        mMacroDefinition.insert_or_assign( name, value );
    }

    // Adds a macro definition to the compiler options.
    void AddMacroDefinition( const std::string& name )
    {
        mMacroDefinition.insert_or_assign( name, "" );
    }

    void ClearMacroDefinition()
    {
        mMacroDefinition.clear();
    }

    void RemoveMacroDefinition( const std::string& name )
    {
        auto it = mMacroDefinition.find( name );
        if ( it != mMacroDefinition.end() )
        {
            mMacroDefinition.erase( it );
        }
    }

    // fill macro definition to shaderc::CompileOptions
    void FillMacroDefinition( shaderc::CompileOptions& options )
    {
        for ( const auto& macro : mMacroDefinition )
        {
            options.AddMacroDefinition( macro.first, macro.second );
        }
    }

  protected:
    shaderc::Compiler mCompiler;

    using MacroDefinition = std::map<std::string, std::string>;
    MacroDefinition mMacroDefinition;
};
