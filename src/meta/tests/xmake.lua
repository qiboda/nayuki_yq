
-- -- if has_config("test") then
-- add_requires("gtest", { configs = { gtest_build_tests = false } })
-- --end

target("meta_tests")
    set_kind("binary")
    add_files("**.cpp")

    add_tests("default")
    set_group("tests")

    --默认情况下不编译
    -- set_default(false)

    add_packages("gtest")

    -- 指定二进制执行文件路径作为运行s时查询路径
    add_rpathdirs("@loader_path")

    --添加本地target依赖
    add_deps("core")
    add_deps("meta")

    if get_config("toolchain") == "clang" then
        --使用gtest的宏在windows中的clang中会报错
        add_cxxflags("-Wno-unsafe-buffer-usage")
    end
