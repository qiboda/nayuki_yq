
-- if has_config("test") then
add_requires("gtest", { configs = { gtest_build_tests = false, runtimes = "MT" } })
--end

target("meta_forge_tests")
    set_kind("binary")
    add_files("**.cpp")

    add_tests("default")
    set_group("tests")

    --默认情况下不编译
    set_default(false)

    add_packages("gtest")

    --添加本地target依赖
    add_deps("meta_forge")

    -- if get_config("toolchain") == "clang" then
    --     --使用gtest的宏在windows中的clang中会报错
    --     add_cxxflags("-Wno-unsafe-buffer-usage")
    -- end