#pragma once
#include <s/s_StateID.hpp>
#include <MSL_C/string.h>

/// @brief An implementation of a state ID for a given class.
/// @details It adds the ability to call the three state methods on a state owner class.
/// @tparam T The class that this state belongs to.
/// @ingroup state
template<class T>
class sFStateID_c : public sStateID_c {
public:
    typedef void (T::*stateFunc)();

    /**
     * @brief Constructs a new sFStateID_c instance.
     * 
     * @param name The name of this state ID.
     * @param initialize The initialize method for this state ID.
     * @param execute The execute method for this state ID.
     * @param finalize The finalize method for this state ID.
     */
    sFStateID_c(const char *name, stateFunc initialize, stateFunc execute, stateFunc finalize) :
        sStateID_c(name),
        mpInitialize(initialize),
        mpExecute(execute),
        mpFinalize(finalize) {}

    /// @brief Returns true if the given name matches this state ID's name.
    virtual bool isSameName(const char *otherName) const {
        char *part = strrchr(otherName, ':');
        if (part != nullptr) {
            otherName = part + 1;
        }
        const char *thisName = strrchr(name(), ':') + 1;
        if (strcmp(thisName, otherName) == 0) {
            return true;
        } else {
            return false;
        }
    }

    /// @brief Calls the initialize method on the owner.
    /// @param owner The owner of this state ID.
    virtual void initializeState(T &owner) const { (owner.*mpInitialize)(); }

    /// @brief Calls the execute method on the owner.
    /// @param owner The owner of this state ID.
    virtual void executeState(T &owner) const { (owner.*mpExecute)(); }

    /// @brief Calls the finalize method on the owner.
    /// @param owner The owner of this state ID.
    virtual void finalizeState(T &owner) const { (owner.*mpFinalize)(); }

private:
    stateFunc mpInitialize; ///< The initialize method for this state ID.
    stateFunc mpExecute; ///< The execute method for this state ID.
    stateFunc mpFinalize; ///< The finalize method for this state ID.
};
