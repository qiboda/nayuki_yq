set_languages("c++23")

target("exp")
    set_kind("binary")
    add_files("**.cpp")

    set_default(false)
    set_group("examples")

    add_deps("core")