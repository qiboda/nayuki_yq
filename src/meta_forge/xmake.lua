
module_name = "meta_forge"
module_name_macro = "META_FORGE"

add_requires("libllvm", {configs = { }})
add_requires("reflect-cpp", {debug = true, configs = {}})

add_requires("inja", {debug = true, configs = { }})

target(module_name)
    set_kind("binary")

    set_group("binary")

    -- modules
    add_files("module/**.ixx", { public = true })
    add_files("src/**.cpp")
    add_includedirs("include", { public = true })
    add_headerfiles("include/**.h", { public = true })

    -- 必须定义
    add_defines(module_name_macro .. "_EXPORTS")

    add_packages( "libllvm" )
    add_packages( "reflect-cpp" )
    add_packages( "inja" )

    add_deps("meta_core")
    add_deps("core")

    add_rpathdirs("/home/skwy/repos/nayuki_yq/build/linux/x86_64/release")

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