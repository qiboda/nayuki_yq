
target("ecs_tests")
    set_kind("binary")
    add_files("**.cpp")

    add_tests("default")
    set_group("tests")

    --默认情况下不编译
    set_default(false)

    add_packages("gtest", "glm", "tbb", "rpmalloc")

    --添加本地target依赖
    add_deps("core")
    add_deps("ecs")

    if is_host("windows") then
        --使用gtest的宏在windows中会报错
        add_cxxflags("-Wno-unsafe-buffer-usage")
    end

    -- set_toolchains("mingw@llvm-mingw")