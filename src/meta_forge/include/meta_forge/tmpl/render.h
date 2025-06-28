#pragma once

#include <inja/inja.hpp>

void render()
{
    // With separate input and output path
    inja::Environment env{ "../path/templates/", "../path/results/" };

    // With other opening and closing strings (here the defaults)
    env.set_expression( "{{", "}}" ); // Expressions
    env.set_comment( "{#", "#}" );    // Comments
    env.set_statement( "{%", "%}" );  // Statements {% %} for many things, see below
    env.set_line_statement( "##" );   // Line statements ## (just an opener)

    inja::json data;

    // 读模板文件和数据，渲染到结果
    auto result = env.render_file( "./templates/greeting.txt", data );
    // result = env.render_file_with_json_file( "./templates/greeting.txt", "./data.json" );

    // 读模板文件和数据，渲染到文件
    env.write( "", data, "./result.txt" );
    // env.write_with_json_file( "./templates/greeting.txt", "./data.json", "./result.txt" );
}