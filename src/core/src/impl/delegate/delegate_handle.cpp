module core.delegate;

import std;
import core;

namespace DelegateDetail
{
// ensure thread safe
static std::atomic<u64> gDelegateHandleId = 0u;
} // namespace DelegateDetail

u64 DelegateHandle::GenerateNewId()
{
    mHandleId = DelegateDetail::gDelegateHandleId.fetch_add( 1 );

    // handle overflow
    if ( mHandleId == 0 )
    {
        mHandleId = DelegateDetail::gDelegateHandleId.fetch_add( 1 );
    }
    return mHandleId;
}