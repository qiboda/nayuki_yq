import("core.project.project")
import("core.project.config")
import("core.project.task")
import("core.tool.compiler")
import("core.tool.toolchain")
import("core.base.json")

local function can_meta(target)
    local target_name = target:name()
    return target_name ~= "core" and
            target_name ~= "meta_core" and
            target_name ~= "meta" and
            target_name ~= "meta_forge"
end

-- 和 module 同名，作为调用的入口
function main()
    local content = {
        path = project.directory(),
        targets = {}
    }
    local targets = project.targets()
    for _, target in pairs(targets) do
        local compinst = target:compiler("cxx")
        if compinst then
            local flags = compinst:compflags({target = target})

            local files = target:sourcefiles()
            local ixxfiles = {}
            for _, file in ipairs(files) do
                if file:endswith(".ixx") then
                    table.insert(ixxfiles, file)
                end
            end

            is_can_meta = can_meta(target)

            all_ixxfiles_info = {}
            json.mark_as_array(all_ixxfiles_info)
            local base_cmd = "clang-scan-deps --format=p1689 -- clang++ --precompile -fmodules -x c++-module"
            for _, ixx in ipairs(ixxfiles) do
                local out = os.iorunv(base_cmd .. " " .. os.args(flags) .. " " .. ixx)
                local json_out = json.decode(out) -- 解析输出的 JSON
                if #json_out["rules"] > 1 then
                    print("Warning: More than one rule found for " .. ixx)
                end
                if #json_out["rules"][1]["provides"] > 1 then
                    print("Warning: More than one provide found for " .. ixx)
                end
                ixxfile_info = {
                    source_path = ixx,
                    logical_name = json_out["rules"][1]["provides"][1]["logical-name"],
                    requires = json_out["rules"][1].requires,
                }
                table.insert(all_ixxfiles_info, ixxfile_info)

                if is_can_meta then
                    -- 新的logical_name
                    local logical_meta_module_name = ixxfile_info.logical_name .. ".meta"
                    local target_path = project.directory() .. "/" .. ".nayuki/generated/".. target:name()
                    local logical_meta_path = target_path .. "/module/" .. logical_meta_module_name .. ".ixx"
                    local logical_impl_path = target_path .. "/impl/" .. ixxfile_info.logical_name .. ".cpp"

                    if os.isfile(logical_meta_path) == false then
                        io.writefile(logical_meta_path, "export module " .. logical_meta_module_name .. ";\n")
                    end
                    if os.isfile(logical_impl_path) == false then
                        io.writefile(logical_impl_path, "module " .. ixxfile_info.logical_name .. ";\n")
                    end
                end
            end

            table.insert(content["targets"], {
                name = target:name(),
                path = target:scriptdir(),
                can_meta = is_can_meta,
                ixxfiles = all_ixxfiles_info,
                flags = flags,
            })
        end
    end

    json.savefile(".nayuki/compile_args.json", content);
end
