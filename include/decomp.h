#ifndef DECOMP_H
#define DECOMP_H

/*******************************************************************************
 * headers
 */

/*******************************************************************************
 * Macro helpers
 */

#define DF_CONCAT3_(a, b, c)	a ## b ## c
#define DF_CONCAT3(a, b, c)		DF_CONCAT3_(a, b, c)

#define DF_UNIQUE_IDENT(ident_)	DF_CONCAT3(ident_, _, __LINE__)

#define DF_TYPEOF				typeof

#define DF_SWALLOW_SEMICOLON()	static_assert(1, "")

#if defined(__clang__)

# define DF_SUPPRESS_WARNINGS()									\
	_Pragma("clang diagnostic push")							\
	_Pragma("clang diagnostic ignored \"-Wnull-dereference\"")
# define DF_UNSUPPRESS_WARNINGS()								\
	_Pragma("clang diagnostic pop")

#elif defined(__MWERKS__)

# pragma section RX ".decomp"

# define DF_SUPPRESS_WARNINGS()		__attribute__((section(".decomp")))
# define DF_UNSUPPRESS_WARNINGS()

#else

# define DF_SUPPRESS_WARNINGS()
# define DF_UNSUPPRESS_WARNINGS()

#endif

/*******************************************************************************
 * DECOMP_FORCE macro internals
 */

#define DF_FUNCTION_DECLARATOR_WITH_PROTO(ident_)	\
	extern void (ident_)(void);						\
	extern void (ident_)(void)

// this is done to prevent default promotion of arguments to variadic functions
#define DF_FUNCTION_CALL(ident_, arg_)		\
	extern void (ident_)(DF_TYPEOF(arg_));	\
	(ident_)(arg_)

/*******************************************************************************
 * DECOMP_FORCE macros
 */

/* Forcefully generate orphaned data, early references, or other shenanigans.
 * Only works at file or namespace scope.
 *
 * Examples:
 * DECOMP_FORCE(1.0f);
 * DECOMP_FORCE("I am a string");
 * DECOMP_FORCE(UI2D_CONSTANT);
 */

#define DECOMP_FORCE(arg_)												\
	DF_SUPPRESS_WARNINGS()												\
	DF_FUNCTION_DECLARATOR_WITH_PROTO(DF_UNIQUE_IDENT(DECOMP_FORCE))	\
	{																	\
		DF_FUNCTION_CALL(DF_UNIQUE_IDENT(DECOMP_FORCE_CALL), arg_);		\
	}																	\
	DF_UNSUPPRESS_WARNINGS()											\
	DF_SWALLOW_SEMICOLON()

/* Forcefully instantiate a class's method. For method_, write it as you would
 * an actual function call.
 * Only works at file or namespace scope.
 *
 * If class_  uses commas outside of nested parentheses, you must enclose the
 * entire argument in DF_TYPEOF(). If method_ uses commas outside of nested
 * parentheses, they are handled via __VA_ARGS__.
 *
 * Examples:
 * DECOMP_FORCE_CLASS_METHOD(Class1, ~Class1());
 * DECOMP_FORCE_CLASS_METHOD(DF_TYPEOF(Class2<int, 1>), func<long, int>(2, 3));
 * DECOMP_FORCE_CLASS_METHOD(Class3, operator ,(4));
 */

#if defined(__cplusplus)
# define DECOMP_FORCE_CLASS_METHOD(class_, ...)	\
	DECOMP_FORCE(((void)(static_cast<DF_TYPEOF(class_) *>(0)->__VA_ARGS__), 0))
#endif

/*******************************************************************************
 * DECOMP_FORCE helpers
 */

// Conversion constants from integer to floating-point

extern   signed int DECOMP_SI;
extern unsigned int DECOMP_UI;

#define SI2D_CONSTANT	((float)(DECOMP_SI))
#define UI2D_CONSTANT	((float)(DECOMP_UI))

// clang-format on

#endif // DECOMP_H
