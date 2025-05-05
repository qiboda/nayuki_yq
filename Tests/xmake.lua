
-- if has_config("test") then
    add_requires("gtest", { configs = { gtest_build_tests = false } })
-- end

target("test")
    set_kind("binary")
    add_files("**.cpp")

    -- 默认情况下不编译
    set_default(false)

    add_packages("gtest")

    -- 添加本地target依赖
    add_deps("Core")
    add_deps("RenderCore")
    add_deps("NayukiYq")