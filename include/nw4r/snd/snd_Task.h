#ifndef NW4R_SND_TASK_H
#define NW4R_SND_TASK_H
#include <nw4r/types_nw4r.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

class Task : private ut::NonCopyable {
    friend class TaskManager;

public:
    Task() : mBusyFlag(false) {}
    virtual ~Task(); // at 0x8

    virtual void Execute() = 0;  // at 0xC
    virtual void Cancel() = 0;   // at 0x10
    virtual void OnCancel() = 0; // at 0x14

public:
    NW4R_UT_LIST_NODE_DECL(); // at 0x4

private:
    bool mBusyFlag; // at 0xC
};

NW4R_UT_LIST_TYPEDEF_DECL(Task);

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
