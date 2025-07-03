
#include "ecs/systems/system.h"

const SystemId SystemId::Invalid = SystemId();

SystemId SystemIdRegistry::sId = {};

std::unordered_map<void*, SystemId> SystemIdRegistry::mFuncToIdMap = {};