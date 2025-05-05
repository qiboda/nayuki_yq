#pragma once

#include <utility>

#include <Core/Misc/NonCopyable.h>

struct CORE_API FSMId {
    FSMId() {}
    FSMId(const std::string &id) : id(id) {}

    bool operator==(const FSMId &fsmId) const { return fsmId.id == id; }

    bool operator!=(const FSMId &fsmId) const { return !(*this == fsmId); }

    std::string id;
};

static const FSMId NullFSMId;

class CORE_API FSMInterface {
  public:
    template <typename T> static T Cast(FSMInterface *fsmInterface) {
        if (fsmInterface && fsmInterface->GetFSMId() == T::GetFSMId_S()) {
            return static_cast<T>(fsmInterface);
        }
    }

  public:
    static FSMId GetFSMId_S() { return NullFSMId; }
    virtual FSMId GetFSMId() const = 0 { return GetFSMId_S(); }

  public:
    virtual void Build() = 0;
};
