
target("core_tests")
    set_kind("binary")
    add_files("**.cpp")

    add_tests("default")
    set_group("tests")

    -- 默认情况下不编译
    set_default(false)

    add_packages("spdlog", "glm", "gtest", "fmt", "tbb", "rpmalloc", "tracy")
    add_deps("core")

    if get_config("toolchain") == "clang" then
        -- 使用gtest的宏在windows中会报错
        add_cxxflags("-Wno-unsafe-buffer-usage")
    end