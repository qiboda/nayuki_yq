
-- if has_config("test") then
    add_requires("gtest", { configs = { gtest_build_tests = false } })
-- end

target("tests")
    set_kind("binary")
    add_files("**.cpp")

    add_tests("Core")

    -- 默认情况下不编译
    set_default(false)

    add_packages("spdlog",  "glm", "gtest")

    -- 添加本地target依赖
    add_deps("Core")

    -- 方便使用gtest的宏
    add_cxxflags("-Wno-unsafe-buffer-usage-in-libc-call")
