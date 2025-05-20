rule("mode.llvm.coverage")
    on_load(function(target)
        -- 使用 -fprofile-arcs 和 -ftest-coverage
        -- target:add("cxxflags", "-fprofile-arcs", "-ftest-coverage")
        -- target:add("ldflags", "-fprofile-arcs", "-ftest-coverage")
        -- target:add("cxxflags", "--coverage")
        -- target:add("ldflags", "--coverage")
        -- target:add("mxflags", "--coverage")
    end)

    on_run("coverage", function(target)
        import("core.project.task")

        local name = target:name()
        local bin = target:targetfile()
        local profraw = name .. ".profraw"
        local profdata = name .. ".profdata"
        local outputdir = "coverage_" .. name

        -- Run tests
        os.execv(bin, {}, { envs = { LLVM_PROFILE_FILE = profraw } })
        os.exec("llvm-profdata merge -sparse %s -o %s", profraw, profdata)
        os.exec("llvm-cov show %s -instr-profile=%s -format=html -output-dir=%s", bin, profdata, outputdir)
        cprint("${green}coverage html generated: %s/index.html", outputdir)
    end)
