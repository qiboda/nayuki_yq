set_project("NayukiYq")

set_xmakever("2.9.2")

set_languages("c++23")

add_rules("mode.debug", "mode.release")
-- 自动更新 compile commands 文件
add_rules("plugin.vsxmake.autoupdate")

if is_mode("debug") then
    add_defines("DEBUG")
else
    add_defines("NDEBUG")
end

-- add engine
includes("Src")

-- add tests
includes("Tests")

-- add examples
includes("Examples")