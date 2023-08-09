#pragma once

// Ported from https://github.com/NSMBW-Community/NSMBW-Decomp/blob/master/include/dol/framework/f_profile.hpp

#include <f/f_profile_name.h>


/// @brief Creates a profile of a base with given values for execute and draw order.
#define SPECIAL_BASE_PROFILE(profName, className, executeOrder, drawOrder, baseProperties) void *className##_classInit() { return new className(); } \
    fProfile::fBaseProfile_c g_profile_##profName = { &className##_classInit, executeOrder, drawOrder, baseProperties }

/// @brief Creates a profile of an actor with given values for execute and draw order and the actor properties. @see SPECIAL_BASE_PROFILE
#define SPECIAL_ACTOR_PROFILE(profName, className, executeOrder, drawOrder, baseProperties, properties) void *className##_classInit() { return new className(); } \
    const fProfile::fActorProfile_c g_profile_##profName = { &className##_classInit, executeOrder, drawOrder, baseProperties, properties }

/// @brief Creates a profile for a base, with the profile number as the priority for both the draw and execute order. @see SPECIAL_BASE_PROFILE
#define DEFAULT_BASE_PROFILE(profName, className) SPECIAL_BASE_PROFILE(profName, className, fProfile::profName, fProfile::profName);

/// @brief Creates a profile of an actor with default values. @see DEFAULT_BASE_PROFILE
#define DEFAULT_ACTOR_PROFILE(profName, className, baseProperties, properties) SPECIAL_ACTOR_PROFILE(profName, className, fProfile::profName, fProfile::profName, baseProperties, properties);
/*

fProfile::fActorProfile_c g_profile_NAME = {
    *class_init()
    profName executreOrder
    profName drawOrder
    properties
}

*/

/// @brief The name of a profile. Value is a fProfile::PROFILE_NAME_e.
/// @note The compilation order suggests that this file might have been grouped together
/// with the other f classes, so a similar naming scheme has been applied here.
typedef u16 ProfileName;

/**
 * @brief Gets a string representing the profile name.
 *
 * @param profName The profile name for which to get the name string of.
 * @return A string containing the profile name.
 */
char *dProf_getName(ProfileName profName);

/// @brief For all profile related structures.
/// @note Unofficial name.
namespace fProfile {

    /// @brief A set of basic information needed to construct a base.
    /// @details A profile consists of a pointer to a constructor function
    /// and a priority value for execution and drawing order.
    struct fBaseProfile_c {
        void *(*mpClassInit)(); ///< The constructor function.
        u16 mExecuteOrder; ///< The execution priority of the base.
        u16 mDrawOrder; ///< The draw priority of the base.
        u32 mBaseProperties;
    };

    /// @brief A set of basic information needed to construct an actor.
    /// @details In addition to the fields in fBaseProfile_c, it also contains some properties about the actor.
    struct fActorProfile_c : fBaseProfile_c {
        u32 mActorProperties; ///< Some actor-related properties. @todo Document the bitfield.
    };

    extern fBaseProfile_c *(*sProfileList)[NUMBER_OF_ACTORS]; ///< A list of all profiles.

} // namespace fProfile