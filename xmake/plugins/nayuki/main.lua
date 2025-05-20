import("core.project.project")
import("core.base.json")

-- 参考的官方源码：xmake: xmake\actions\test\main.lua
local function get_test_module_info()
    local test_module_info = {}

    for _, target in ipairs(project.ordertargets()) do
        local tests = target:get("tests");
        if tests then
            local target_name = target:name()
            local target_tests_info = { ["target_name"] = target_name, ["test_names"] = {} }
            for _, test_name in ipairs(target:get("tests")) do
                table.insert(target_tests_info["test_names"], test_name)
            end
            table.insert(test_module_info, target_tests_info)
        end
    end

    return test_module_info
end

-- 和 module 同名，作为调用的入口
function main()
    local test_module_info = get_test_module_info();

    -- generate nayuki.json and save to .nayuki folder
    local nayuki_json = {
        ["name"] = project.name(),
        ["version"] = project.version(),
        ["targets"] = {},
        ["tests"] = test_module_info,
    }
    for _, target in pairs(project.ordertargets()) do
        local target_name = target:name()
        local target_dir = target:scriptdir()
        local target_json = {
            ["name"] = target_name,
            ["path"] = target_dir,
            ["group"] = target:get("group"),
            ["kind"] = target:get("kind"),
        }
        table.insert(nayuki_json["targets"], target_json)
    end

    local nayuki_json_path = path.join(os.projectdir(), ".nayuki", "nayuki.json")
    os.mkdir(path.join(os.projectdir(), ".nayuki"))
    json.savefile(nayuki_json_path, nayuki_json)
end
