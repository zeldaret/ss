#ifndef MSL_STDIO_H_
#define MSL_STDIO_H_

#include "char_io.h"
#include "extras.h"
#include "file_io.h"
#include "printf.h"

#define stdin (&__files._stdin)
#define stdout (&__files._stdout)
#define stderr (&__files._stderr)

#endif
