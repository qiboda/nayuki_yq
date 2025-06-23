
target("exec")
    set_kind("binary")
    add_files("**.cpp")

    set_default(false)
    set_group("examples")

    add_deps("task")