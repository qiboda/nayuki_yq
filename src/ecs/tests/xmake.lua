
target("ecs_tests")
    set_kind("binary")
    add_files("**.cpp")

    add_tests("default")
    set_group("tests")

    --默认情况下不编译
    -- set_default(false)

    --添加本地target依赖
    add_packages("gtest")

    add_deps("core")
    add_deps("ecs")

    if get_config("toolchain") == "clang" then
        --使用gtest的宏在windows中会报错
        add_cxxflags("-Wno-unsafe-buffer-usage")
    end