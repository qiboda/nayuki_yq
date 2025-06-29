
#include "render_core/shader/shader_compiler.h"
#include "core/logger/log.h"
#include "render_core/render_core.h"

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
