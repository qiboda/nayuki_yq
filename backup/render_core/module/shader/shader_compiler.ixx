module;

#include <shaderc/shaderc.hpp>
#include <module_export.h>
import core;
#include <render_core/render_core.h>
#include <core/logger/log.h>
import core.logger;

export module render_core.shader_compiler;

export class RENDER_CORE_API ShaderCompiler : public Singleton<ShaderCompiler>, public IRAII
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

ShaderCompiler::ShaderCompiler()
{
}

ShaderCompiler::~ShaderCompiler()
{
}

std::string
ShaderCompiler::PreprocessShader( const std::string& source_name, shaderc_shader_kind kind, const std::string& source )
{
    shaderc::CompileOptions options;
    FillMacroDefinition( options );

    shaderc::PreprocessedSourceCompilationResult result =
        mCompiler.PreprocessGlsl( source, kind, source_name.c_str(), options );

    if ( result.GetCompilationStatus() != shaderc_compilation_status_success )
    {
        NY_LOG_WARN( LogRenderCore, "ShaderCompiler::PreprocessShader error: {}", result.GetErrorMessage() );
        return "";
    }

    return { result.cbegin(), result.cend() };
}

std::string ShaderCompiler::CompileFileToAssembly( const std::string& source_name,
                                                   shaderc_shader_kind kind,
                                                   const std::string& source,
                                                   bool optimize )
{
    shaderc::CompileOptions options;
    FillMacroDefinition( options );
    if ( optimize )
        options.SetOptimizationLevel( shaderc_optimization_level_size );

    shaderc::AssemblyCompilationResult result =
        mCompiler.CompileGlslToSpvAssembly( source, kind, source_name.c_str(), options );

    if ( result.GetCompilationStatus() != shaderc_compilation_status_success )
    {
        NY_LOG_WARN( LogRenderCore, "ShaderCompiler::CompileFileToAssembly error: {}", result.GetErrorMessage() );
        return "";
    }

    return { result.cbegin(), result.cend() };
}

std::vector<u32> ShaderCompiler::CompileFile( const std::string& source_name,
                                              shaderc_shader_kind kind,
                                              const std::string& source,
                                              bool optimize )
{
    shaderc::CompileOptions options;
    FillMacroDefinition( options );
    if ( optimize )
        options.SetOptimizationLevel( shaderc_optimization_level_size );

    shaderc::SpvCompilationResult module = mCompiler.CompileGlslToSpv( source, kind, source_name.c_str(), options );

    if ( module.GetCompilationStatus() != shaderc_compilation_status_success )
    {
        NY_LOG_WARN( LogRenderCore, "ShaderCompiler::CompileFile error: {}", module.GetErrorMessage() );
        return std::vector<uint32_t>();
    }

    return { module.cbegin(), module.cend() };
}
