#ifndef NW4R_SND_DISPOSE_CALLBACK_MANAGER_H
#define NW4R_SND_DISPOSE_CALLBACK_MANAGER_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_DisposeCallback.h>

namespace nw4r {
namespace snd {
namespace detail {

class DisposeCallbackManager {
public:
    static DisposeCallbackManager& GetInstance();

    void RegisterDisposeCallback(DisposeCallback* pCallback);
    void UnregisterDisposeCallback(DisposeCallback* pCallback);

    void Dispose(void* pData, u32 size, void* pArg);
    void DisposeWave(void* pData, u32 size, void* pArg);

private:
    DisposeCallbackManager();

private:
    DisposeCallbackList mCallbackList; // at 0x0
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
