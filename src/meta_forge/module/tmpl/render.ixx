module;

#include <inja/inja.hpp>
#include <module_export.h>

export module meta_forge.tmpl.render;

import core;
import core.fs.paths;
import std;

export class META_FORGE_API TmplRender
{
  public:
    TmplRender( FsPath outFolder )
    {
        FsPath AssetFolder = Paths::EngineAssetFolder();
        mInTemplateFolder = AssetFolder / "templates" / "meta_forge";
        mOutFolder = std::move( outFolder );
        env = new inja::Environment( mInTemplateFolder.string(), mOutFolder.string() );
    }

    ~TmplRender()
    {
        if ( env )
        {
            delete env;
        }
    }

    void Init()
    {
        // With other opening and closing strings (here the defaults)
        env->set_expression( "{{", "}}" ); // Expressions
        env->set_comment( "{#", "#}" );    // Comments
        env->set_statement( "{%", "%}" );  // Statements {% %} for many things, see below
        env->set_line_statement( "##" );   // Line statements ## (just an opener)
    }

    void render( FsPath tmplFile, FsPath dataFile, FsPath resultFile )
    {
        env->write_with_json_file( tmplFile.string(), dataFile.string(), resultFile.string() );
    }

  protected:
    FsPath mInTemplateFolder;
    FsPath mOutFolder;
    inja::Environment* env = nullptr;
};
