// clang-format off

/*******************************************************************************
 * anti-header guard
 */

#ifdef __MWERKS__
# pragma notonce
#endif

/*******************************************************************************
 * NW4RAssert/NW4RCheck base macros
 */

// <assert.h> behavior
#undef NW4RAssertMessage_FileLine
#undef NW4RCheckMessage_FileLine

#if defined(NW4R_ASSERT)
# define NW4RAssertMessage_FileLine(file_, line_, expr_, ...)	((void)((expr_) || (::nw4r::db::Panic  (file_, line_, __VA_ARGS__), 0)))
# define NW4RCheckMessage_FileLine(file_, line_, expr_, ...)	((void)((expr_) || (::nw4r::db::Warning(file_, line_, __VA_ARGS__), 0)))
#else
# define NW4RAssertMessage_FileLine(file_, line_, expr_, ...)	((void)(0))
# define NW4RCheckMessage_FileLine(file_, line_, expr_, ...)	((void)(0))
#endif // defined(NW4R_ASSERT)

/*******************************************************************************
 * Derived macros
 */

/* Main asserts */

// NW4RAssert family
#define NW4RAssertMessage_File(file_, expr_, ...)		NW4RAssertMessage_FileLine(  file_ , __LINE__, expr_, __VA_ARGS__)
#define NW4RAssertMessage_Line(line_, expr_, ...)		NW4RAssertMessage_FileLine(__FILE__,   line_ , expr_, __VA_ARGS__)
#define NW4RAssertMessage(expr_, ...)					NW4RAssertMessage_FileLine(__FILE__, __LINE__, expr_, __VA_ARGS__)

#define NW4RAssert_FileLine(file_, line_, expr_)		NW4RAssertMessage_FileLine(  file_ ,   line_ , expr_, "NW4R:Failed assertion " #expr_)
#define NW4RAssert_File(file_, expr_)					NW4RAssertMessage_FileLine(  file_ , __LINE__, expr_, "NW4R:Failed assertion " #expr_)
#define NW4RAssert_Line(line_, expr_)					NW4RAssertMessage_FileLine(__FILE__,   line_ , expr_, "NW4R:Failed assertion " #expr_)
#define NW4RAssert(expr_)								NW4RAssertMessage_FileLine(__FILE__, __LINE__, expr_, "NW4R:Failed assertion " #expr_)

#define NW4RAssertHeader_FileLine(file_, line_, expr_)	NW4RAssertMessage_FileLine(  file_ ,   line_ , expr_,      "Failed assertion " #expr_)
#define NW4RAssertHeader_File(file_, expr_)				NW4RAssertMessage_FileLine(  file_ , __LINE__, expr_,      "Failed assertion " #expr_)
#define NW4RAssertHeader_Line(line_, expr_)				NW4RAssertMessage_FileLine(__FILE__,   line_ , expr_,      "Failed assertion " #expr_)
#define NW4RAssertHeader(expr_)							NW4RAssertMessage_FileLine(__FILE__, __LINE__, expr_,      "Failed assertion " #expr_)

// NW4RPanic family
#define NW4RPanicMessage_FileLine(file_, line_, ...)	NW4RAssertMessage_FileLine(  file_ ,   line_ , false, __VA_ARGS__)
#define NW4RPanicMessage_File(file_, ...)				NW4RAssertMessage_FileLine(  file_ , __LINE__, false, __VA_ARGS__)
#define NW4RPanicMessage_Line(line_, ...)				NW4RAssertMessage_FileLine(__FILE__,   line_ , false, __VA_ARGS__)
#define NW4RPanicMessage(...)							NW4RAssertMessage_FileLine(__FILE__, __LINE__, false, __VA_ARGS__)

#define NW4RPanic_FileLine(file_, line_)				NW4RAssert_FileLine(  file_ ,   line_ , false)
#define NW4RPanic_File(file_)							NW4RAssert_FileLine(  file_ , __LINE__, false)
#define NW4RPanic_Line(line_)							NW4RAssert_FileLine(__FILE__,   line_ , false)
#define NW4RPanic()										NW4RAssert_FileLine(__FILE__, __LINE__, false)

// NW4RCheck family
#define NW4RCheckMessage_File(file_, expr_, ...)		NW4RCheckMessage_FileLine(  file_ , __LINE__, expr_, __VA_ARGS__)
#define NW4RCheckMessage_Line(line_, expr_, ...)		NW4RCheckMessage_FileLine(__FILE__,   line_ , expr_, __VA_ARGS__)
#define NW4RCheckMessage(expr_, ...)					NW4RCheckMessage_FileLine(__FILE__, __LINE__, expr_, __VA_ARGS__)

#define NW4RCheck_FileLine(file_, line_, expr_)			NW4RCheckMessage_FileLine(  file_ ,   line_ , expr_, "NW4R:Failed check " #expr_)
#define NW4RCheck_File(file_, expr_)					NW4RCheckMessage_FileLine(  file_ , __LINE__, expr_, "NW4R:Failed check " #expr_)
#define NW4RCheck_Line(line_, expr_)					NW4RCheckMessage_FileLine(__FILE__,   line_ , expr_, "NW4R:Failed check " #expr_)
#define NW4RCheck(expr_)								NW4RCheckMessage_FileLine(__FILE__, __LINE__, expr_, "NW4R:Failed check " #expr_)

#define NW4RCheckHeader_FileLine(file_, line_, expr_)	NW4RCheckMessage_FileLine(  file_ ,   line_ , expr_,      "Failed check " #expr_)
#define NW4RCheckHeader_File(file_, expr_)				NW4RCheckMessage_FileLine(  file_ , __LINE__, expr_,      "Failed check " #expr_)
#define NW4RCheckHeader_Line(line_, expr_)				NW4RCheckMessage_FileLine(__FILE__,   line_ , expr_,      "Failed check " #expr_)
#define NW4RCheckHeader(expr_)							NW4RCheckMessage_FileLine(__FILE__, __LINE__, expr_,      "Failed check " #expr_)

// NW4RWarning family
#define NW4RWarningMessage_FileLine(file_, line_, ...)	NW4RCheckMessage_FileLine(  file_ ,   line_ , false, __VA_ARGS__)
#define NW4RWarningMessage_File(file_, ...)				NW4RCheckMessage_FileLine(  file_ , __LINE__, false, __VA_ARGS__)
#define NW4RWarningMessage_Line(line_, ...)				NW4RCheckMessage_FileLine(__FILE__,   line_ , false, __VA_ARGS__)
#define NW4RWarningMessage(...)							NW4RCheckMessage_FileLine(__FILE__, __LINE__, false, __VA_ARGS__)

#define NW4RWarning_FileLine(file_, line_)				NW4RCheck_FileLine(  file_ ,   line_ , false)
#define NW4RWarning_File(file_)							NW4RCheck_FileLine(  file_ , __LINE__, false)
#define NW4RWarning_Line(line_)							NW4RCheck_FileLine(__FILE__,   line_ , false)
#define NW4RWarning()									NW4RCheck_FileLine(__FILE__, __LINE__, false)

/* Extended asserts */

// PointerNonnull
#define NW4RAssertPointerNonnull_FileLine(file_, line_, ptr_)								NW4RAssertMessage_FileLine(  file_ ,   line_ , (ptr_) != 0, "NW4R:Pointer must not be NULL (" #ptr_ ")")
#define NW4RAssertPointerNonnull_File(file_, ptr_)											NW4RAssertMessage_FileLine(  file_ , __LINE__, (ptr_) != 0, "NW4R:Pointer must not be NULL (" #ptr_ ")")
#define NW4RAssertPointerNonnull_Line(line_, ptr_)											NW4RAssertMessage_FileLine(__FILE__,   line_ , (ptr_) != 0, "NW4R:Pointer must not be NULL (" #ptr_ ")")
#define NW4RAssertPointerNonnull(ptr_)														NW4RAssertMessage_FileLine(__FILE__, __LINE__, (ptr_) != 0, "NW4R:Pointer must not be NULL (" #ptr_ ")")

#define NW4RAssertHeaderPointerNonnull_FileLine(file_, line_, ptr_)							NW4RAssertMessage_FileLine(  file_ ,   line_ , (ptr_) != 0,      "Pointer must not be NULL (" #ptr_ ")")
#define NW4RAssertHeaderPointerNonnull_File(file_, ptr_)									NW4RAssertMessage_FileLine(  file_ , __LINE__, (ptr_) != 0,      "Pointer must not be NULL (" #ptr_ ")")
#define NW4RAssertHeaderPointerNonnull_Line(line_, ptr_)									NW4RAssertMessage_FileLine(__FILE__,   line_ , (ptr_) != 0,      "Pointer must not be NULL (" #ptr_ ")")
#define NW4RAssertHeaderPointerNonnull(ptr_)												NW4RAssertMessage_FileLine(__FILE__, __LINE__, (ptr_) != 0,      "Pointer must not be NULL (" #ptr_ ")")

// Aligned
#define NW4R_IS_ALIGNED_(x, align)	(((unsigned long)(x) & ((align) - 1)) == 0) // just redefine instead of pulling in <macros.h>

#define NW4RAssertAligned_FileLine(file_, line_, val_, align_)								NW4RAssertMessage_FileLine(  file_ ,   line_ , NW4R_IS_ALIGNED_(val_, align_), "NW4R:Alignment Error(0x%x)\n" #val_ " must be aligned to " #align_ " bytes boundary.", val_)
#define NW4RAssertAligned_File(file_, val_, align_)											NW4RAssertMessage_FileLine(  file_ , __LINE__, NW4R_IS_ALIGNED_(val_, align_), "NW4R:Alignment Error(0x%x)\n" #val_ " must be aligned to " #align_ " bytes boundary.", val_)
#define NW4RAssertAligned_Line(line_, val_, align_)											NW4RAssertMessage_FileLine(__FILE__,   line_ , NW4R_IS_ALIGNED_(val_, align_), "NW4R:Alignment Error(0x%x)\n" #val_ " must be aligned to " #align_ " bytes boundary.", val_)
#define NW4RAssertAligned(val_, align_)														NW4RAssertMessage_FileLine(__FILE__, __LINE__, NW4R_IS_ALIGNED_(val_, align_), "NW4R:Alignment Error(0x%x)\n" #val_ " must be aligned to " #align_ " bytes boundary.", val_)

#define NW4RAssertHeaderAligned_FileLine(file_, line_, val_, align_)						NW4RAssertMessage_FileLine(  file_ ,   line_ , NW4R_IS_ALIGNED_(val_, align_),      "Alignment Error(0x%x)\n" #val_ " must be aligned to " #align_ " bytes boundary.", val_)
#define NW4RAssertHeaderAligned_File(file_, val_, align_)									NW4RAssertMessage_FileLine(  file_ , __LINE__, NW4R_IS_ALIGNED_(val_, align_),      "Alignment Error(0x%x)\n" #val_ " must be aligned to " #align_ " bytes boundary.", val_)
#define NW4RAssertHeaderAligned_Line(line_, val_, align_)									NW4RAssertMessage_FileLine(__FILE__,   line_ , NW4R_IS_ALIGNED_(val_, align_),      "Alignment Error(0x%x)\n" #val_ " must be aligned to " #align_ " bytes boundary.", val_)
#define NW4RAssertHeaderAligned(val_, align_)												NW4RAssertMessage_FileLine(__FILE__, __LINE__, NW4R_IS_ALIGNED_(val_, align_),      "Alignment Error(0x%x)\n" #val_ " must be aligned to " #align_ " bytes boundary.", val_)

// ClampedLValue
#define NW4RAssertClampedLValue_FileLine(file_, line_, var_, minValue_, maxValue_)			NW4RAssertMessage_FileLine(  file_ ,   line_ , (var_) >= (minValue_) && (var_) <  (maxValue_), "NW4R:" #var_ " is out of bounds(%d)\n%d <= " #var_ " < %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertClampedLValue_File(file_, var_, minValue_, maxValue_)						NW4RAssertMessage_FileLine(  file_ , __LINE__, (var_) >= (minValue_) && (var_) <  (maxValue_), "NW4R:" #var_ " is out of bounds(%d)\n%d <= " #var_ " < %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertClampedLValue_Line(line_, var_, minValue_, maxValue_)						NW4RAssertMessage_FileLine(__FILE__,   line_ , (var_) >= (minValue_) && (var_) <  (maxValue_), "NW4R:" #var_ " is out of bounds(%d)\n%d <= " #var_ " < %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertClampedLValue(var_, minValue_, maxValue_)									NW4RAssertMessage_FileLine(__FILE__, __LINE__, (var_) >= (minValue_) && (var_) <  (maxValue_), "NW4R:" #var_ " is out of bounds(%d)\n%d <= " #var_ " < %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))

#define NW4RAssertHeaderClampedLValue_FileLine(file_, line_, var_, minValue_, maxValue_)	NW4RAssertMessage_FileLine(  file_ ,   line_ , (var_) >= (minValue_) && (var_) <  (maxValue_),         #var_ " is out of bounds(%d)\n%d <= " #var_ " < %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertHeaderClampedLValue_File(file_, var_, minValue_, maxValue_)				NW4RAssertMessage_FileLine(  file_ , __LINE__, (var_) >= (minValue_) && (var_) <  (maxValue_),         #var_ " is out of bounds(%d)\n%d <= " #var_ " < %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertHeaderClampedLValue_Line(line_, var_, minValue_, maxValue_)				NW4RAssertMessage_FileLine(__FILE__,   line_ , (var_) >= (minValue_) && (var_) <  (maxValue_),         #var_ " is out of bounds(%d)\n%d <= " #var_ " < %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertHeaderClampedLValue(var_, minValue_, maxValue_)							NW4RAssertMessage_FileLine(__FILE__, __LINE__, (var_) >= (minValue_) && (var_) <  (maxValue_),         #var_ " is out of bounds(%d)\n%d <= " #var_ " < %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))

// ClampedLRValue
#define NW4RAssertClampedLRValue_FileLine(file_, line_, var_, minValue_, maxValue_)			NW4RAssertMessage_FileLine(  file_ ,   line_ , (var_) >= (minValue_) && (var_) <= (maxValue_), "NW4R:" #var_ " is out of bounds(%d)\n%d <= " #var_ " <= %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertClampedLRValue_File(file_, var_, minValue_, maxValue_)					NW4RAssertMessage_FileLine(  file_ , __LINE__, (var_) >= (minValue_) && (var_) <= (maxValue_), "NW4R:" #var_ " is out of bounds(%d)\n%d <= " #var_ " <= %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertClampedLRValue_Line(line_, var_, minValue_, maxValue_)					NW4RAssertMessage_FileLine(__FILE__,   line_ , (var_) >= (minValue_) && (var_) <= (maxValue_), "NW4R:" #var_ " is out of bounds(%d)\n%d <= " #var_ " <= %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertClampedLRValue(var_, minValue_, maxValue_)								NW4RAssertMessage_FileLine(__FILE__, __LINE__, (var_) >= (minValue_) && (var_) <= (maxValue_), "NW4R:" #var_ " is out of bounds(%d)\n%d <= " #var_ " <= %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))

#define NW4RAssertHeaderClampedLRValue_FileLine(file_, line_, var_, minValue_, maxValue_)	NW4RAssertMessage_FileLine(  file_ ,   line_ , (var_) >= (minValue_) && (var_) <= (maxValue_),         #var_ " is out of bounds(%d)\n%d <= " #var_ " <= %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertHeaderClampedLRValue_File(file_, var_, minValue_, maxValue_)				NW4RAssertMessage_FileLine(  file_ , __LINE__, (var_) >= (minValue_) && (var_) <= (maxValue_),         #var_ " is out of bounds(%d)\n%d <= " #var_ " <= %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertHeaderClampedLRValue_Line(line_, var_, minValue_, maxValue_)				NW4RAssertMessage_FileLine(__FILE__,   line_ , (var_) >= (minValue_) && (var_) <= (maxValue_),         #var_ " is out of bounds(%d)\n%d <= " #var_ " <= %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))
#define NW4RAssertHeaderClampedLRValue(var_, minValue_, maxValue_)							NW4RAssertMessage_FileLine(__FILE__, __LINE__, (var_) >= (minValue_) && (var_) <= (maxValue_),         #var_ " is out of bounds(%d)\n%d <= " #var_ " <= %d not satisfied.", (int)(var_), (int)(minValue_), (int)(maxValue_))

/*******************************************************************************
 * Strings, for deadstripping
 */

/* Main asserts */

#define NW4RAssert_String(expr_)						"NW4R:Failed assertion " #expr_
#define NW4RAssertHeader_String(expr_)					     "Failed assertion " #expr_

#define NW4RCheck_String(expr_)							"NW4R:Failed check " #expr_
#define NW4RCheckHeader_String(expr_)					     "Failed check " #expr_

/* Extended asserts */

#define NW4RAssertPointerNonnull_String(ptr_)			"NW4R:Pointer must not be NULL (" #ptr_ ")"
#define NW4RAssertHeaderPointerNonnull_String(ptr_)		     "Pointer must not be NULL (" #ptr_ ")"

#define NW4RAssertAligned_String(val_, align_)			"NW4R:Alignment Error(0x%x)\n" #val_ " must be aligned to " #align_ " bytes boundary."
#define NW4RAssertHeaderAligned_String(val_, align_)	     "Alignment Error(0x%x)\n" #val_ " must be aligned to " #align_ " bytes boundary."

#define NW4RAssertClampedLValue_String(var_)			"NW4R:" #var_ " is out of bounds(%d)\n%d <= " #var_ " < %d not satisfied."
#define NW4RAssertHeaderClampedLValue_String(var_)		        #var_ " is out of bounds(%d)\n%d <= " #var_ " < %d not satisfied."

#define NW4RAssertClampedLRValue_String(var_)			"NW4R:" #var_ " is out of bounds(%d)\n%d <= " #var_ " <= %d not satisfied."
#define NW4RAssertHeaderClampedLRValue_String(var_)		        #var_ " is out of bounds(%d)\n%d <= " #var_ " <= %d not satisfied."

/*******************************************************************************
 * Declarations
 */

namespace nw4r { namespace db
{
	__attribute__((weak)) extern void Panic(char const *file, int line, char const *msg, ...);
	__attribute__((weak)) extern void Warning(char const *file, int line, char const *msg, ...);
}} // namespace nw4r::db

// clang-format on
