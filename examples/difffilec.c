/* Use FILE* and routines instead of the default memory */
#define Stream_Seek(stream, pos, flag) \
	(fseek((FILE*)stream, (long)pos, flag) == 0 ? pos : -1)
#define Stream_Read(stream, data, count) \
	fread(data, 1, (long)count, (FILE*)stream)
#define Stream_Write(stream, data, count) \
	fwrite(data, 1, (long)count, (FILE*)stream)
/* NB: ftell returns full newline string length,
	while others translate to \n */
#define Stream_Tell(stream) ftell((FILE*)stream)
#define DIFF_IMPLEMENTATION
#define DIFFSELFCONTAINEDEXAMPLE
#include "diff.h"
