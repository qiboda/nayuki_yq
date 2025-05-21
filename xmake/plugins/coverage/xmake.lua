
-- 定义一个 task
task("coverage")
    set_category("plugin")

-- set the menu options, but we put empty options now.
    set_menu {
                -- usage
                usage = "xmake coverage [options]"
                -- description
            ,   description = "test coverage"
                -- options
            ,   options = {
                    {nil, "target",  "v",  nil,   "The target name. It will run all default targets if this parameter is not specified."
                                                       , values = function (complete, opt) return import("private.utils.complete_helper.targets")(complete, opt) end }
                }
            }

    on_run(function ()
        import("core.base.option")
        import("core.project.project")

        local target_name = option.get("target");
        local target = project.target(target_name)
        if not target then
            print("target not found: ", target_name)
            return
        end
        if target:kind() ~= "binary" then
            print("target is not a binary: ", target_name)
            return
        end
        local target_file = target:targetdir() .. "/debug/" .. target:basename() .. ".exe"
        local human_time = os.date("%Y-%m-%d_%H-%M-%S", os.time())
        os.exec("OpenCppCoverage.exe --sources src\\ --export_type html:coverage/".. target:basename() .. "_" .. human_time .. " -- " .. target_file)
    end)