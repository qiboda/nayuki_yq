#include "Core/Delegate/DelegateHandle.h"

#include <atomic>

namespace DelegateDetail {
// ensure thread safe
std::atomic<ui64> gDelegateHandleId = 0u;
} // namespace DelegateDetail

ui64 DelegateHandle::GenerateNewId() {
    mHandleId = DelegateDetail::gDelegateHandleId.fetch_add(1);

    // handle overflow
    if (mHandleId == 0) {
        mHandleId = DelegateDetail::gDelegateHandleId.fetch_add(1);
    }
    return mHandleId;
}