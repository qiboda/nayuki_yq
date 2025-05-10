-- 定义一个 task
task("fixcc")
    set_category("plugin")

-- set the menu options, but we put empty options now.
    set_menu {
                -- usage
                usage = "xmake fixcc [options]"
                -- description
            ,   description = "fix compile_commands.json file"
                -- options
            ,   options = {}
            }

    on_run(function ()
        os.exec("xmake project -k compile_commands")
        local cc_file = os.projectdir() .. "/.vscode/compile_commands.json";
        local cc_target_file = os.projectdir() .. "/compile_commands.json";
        local f = io.readfile(cc_file)
        f = f:gsub("%-IC", "-external:IC")
        io.writefile(cc_target_file, f)
        print("Patched compile_commands.json")
    end)