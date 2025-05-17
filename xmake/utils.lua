function includes_cur_dirs()
    -- 获得当前下的所有目录
    local curdir = os.curdir()
    local dirs = os.dirs(curdir .. "/*")

    -- 定义导出模块名列表
    local modules = {}

    for _, dir in ipairs(dirs) do
        local name = path.filename(dir)
        -- 过滤掉不需要的目录
        table.insert(modules, name)
    end

    -- 引入其他模块。
    for _, module in ipairs(modules) do
        includes(module)
    end
end
