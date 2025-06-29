
module_name = "meta_forge"
module_name_macro = "META_FORGE"

-- , libunwind = true
-- libcxx = true, libcxxabi = true
add_requires("libllvm", {configs = { }})
-- 因为libllvm仅仅支持 MT
add_requires("reflect-cpp", {debug = true, configs = { runtimes = "MT" }})

add_requires("inja", {debug = true, configs = { runtimes = "MT" }})

target(module_name)
    set_kind("binary")

    set_group("binary")

    -- 因为libllvm仅仅支持 MT
    set_runtimes("MT")

    add_files("src/**.cpp")
    add_includedirs("include", { public = true })
    add_headerfiles("include/**.h", { public = true })

    local pheader_file = path.join("include", module_name, module_name .. ".h");
     set_pcxxheader(pheader_file)

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    -- 这两个选项同时使用，生成独立的debug符号信息。
    -- set_symbols("debug")
    -- set_strip("all")

    add_packages( "libllvm" )
    add_packages( "reflect-cpp" )
    add_packages( "inja" )

    add_deps("meta_core")
    add_deps("core")

    after_build(function (target)
        for _, pkg in pairs(target:pkgs()) do
            local installdir = pkg:installdir()
            if installdir then
                -- 查找并拷贝所有 DLL/SO/DYLIB 文件
                local lib_path = nil;
                if is_plat("windows") then
                    lib_path = path.join(installdir, "bin/*.dll")
                elseif is_plat("macosx") then
                    lib_path = path.join(installdir, "lib/*.so*")
                elseif is_plat("linux") then
                    lib_path = path.join(installdir, "lib/*.so*")
                end
                local files = os.files(lib_path)
                for _, f in ipairs(files) do
                    if f then
                        os.cp(f, target:targetdir())
                        print("on package <" .. pkg:name() .. "> copy " .. f .. " to " .. target:targetdir())
                    end
                end
            end
        end
    end)