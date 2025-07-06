-- 定义一个 task
task("compile_args")
    set_category("plugin")

    -- set the menu options, but we put empty options now.
    set_menu {
                -- usage
                usage = "xmake compile_args [options]",
                -- description
                description = "compile_args plugin for xmake",
                -- options
                options = {}
            }

    on_run("main")
