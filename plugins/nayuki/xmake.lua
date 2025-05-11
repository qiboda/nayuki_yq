-- 定义一个 task
task("nayuki")
    set_category("plugin")

    -- set the menu options, but we put empty options now.
    set_menu {
                -- usage
                usage = "xmake nayuki [options]",
                -- description
                description = "nayuki plugin for xmake",
                -- options
                options = {}
            }

    on_run("main")