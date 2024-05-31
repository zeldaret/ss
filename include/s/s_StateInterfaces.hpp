#pragma once

/// @addtogroup state
/// @{

class sStateMethodIf_c {
public:
    virtual ~sStateMethodIf_c();
};

/// @brief The interface for state IDs.
/// @details A state ID is made up of a name string and a unique number, where 0 denotes a null state.
/// Null states do not have any corresponding behaviour. They can be used, for example,
/// if a state holder needs to be initialized but the initial state ID is not known yet.
class sStateIDIf_c {
public:
    virtual ~sStateIDIf_c();

    virtual bool isNull() const = 0; ///<  Returns whether this is a null state.
    virtual bool isEqual(const sStateIDIf_c &other) const = 0; ///<  Returns whether both states have the same number.

    virtual bool operator==(const sStateIDIf_c &other) const = 0; ///<  Overloaded equality operator, using ::isEqual.
    virtual bool operator!=(const sStateIDIf_c &other) const = 0; ///<  Overloaded inequality operator, using ::isEqual.

    virtual bool isSameName(const char *name) const = 0; ///<  Returns whether this state ID is called @p name.
    virtual const char *name() const = 0; ///<  Returns the name of this state ID.
    virtual unsigned int number() const = 0; ///<  Returns the number of this state ID.
};

/// @brief The interface for a state holder.
/// @details A state holder is linked to a state ID and can execute its corresponding behaviour.
class sStateIf_c {
public:
    virtual ~sStateIf_c();
    virtual const void initialize() = 0; ///<  Initializes the state.
    virtual const void execute() = 0; ///<  Executes the state.
    virtual const void finalize() = 0; ///<  Prepares the state for termination.
};

/// @brief The interface for state factories.
/// @details A state factory produces a state holder from a given state ID.
class sStateFctIf_c {
public:
    virtual ~sStateFctIf_c();
    virtual sStateIf_c *build(sStateIDIf_c const &id) = 0; ///< Returns a new state with a given state ID.
    virtual void dispose(sStateIf_c *&id) = 0; ///< Clears out the pointer to a state.
};

/// @brief The interface for state ID checkers.
/// @details [This class is not really used, but it seems to be intended for testing if a state is "normal", most likely a debug leftover].
class sStateIDChkIf_c {
public:
    virtual ~sStateIDChkIf_c();
    virtual bool isNormalID(const sStateIDIf_c &id) const = 0; ///< Returns whether a state ID is normal.
};

/// @brief The interface for state managers.
/// @details A state manager handles execution of and transitioning between state IDs.
class sStateMgrIf_c {
public:
    virtual ~sStateMgrIf_c();
    virtual void initializeState() = 0; ///<  Initializes the current state.
    virtual void executeState() = 0; ///<  Executes the current state.
    virtual void finalizeState() = 0; ///<  Prepares the current state for termination.
    virtual void changeState(const sStateIDIf_c &newStateID) = 0; ///<  Transitions to a new state ID.
    virtual void refreshState() = 0; ///<  Marks the current state to be executed again.
    virtual sStateIf_c *getState() const = 0; ///<  Gets the state holder.
    virtual const sStateIDIf_c *getNewStateID() const = 0; ///<  Gets the next state ID.
    virtual const sStateIDIf_c *getStateID() const = 0; ///<  Gets the current state ID.
    virtual const sStateIDIf_c *getOldStateID() const = 0; ///<  Gets the previous state ID.
};

/// @}
