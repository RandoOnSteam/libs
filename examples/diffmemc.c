#define DIFF_IMPLEMENTATION
#if 0 /* these are the default */
	#define Stream_Seek(stream, pos, flag) \
		MemoryStream_Seek(stream, (long)pos, flag)
	#define Stream_Read(stream, data, count) \
		MemoryStream_Read(stream, data, (long)count)
	#define Stream_Write(stream, data, count) \
		MemoryStream_Write(stream, data, (long)count)
	#define Stream_Tell(stream) MemoryStream_Tell(stream)
#endif
#include "diff.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
long getFILEsize(FILE *file)
{
    long size;
    long currentPosition = ftell(file);
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, currentPosition, SEEK_SET);
    return size;
}

void cant(
	char			*filename,
	const char		*what,
	int				fatalflag)
{
	fprintf(stderr, "Can't open %s file \"%s\": ", what, filename);
	perror((char*) NULL);
	if (fatalflag) {
		exit(fatalflag);
	}
}

int main(int argc, char** argv)
{
	char* ap;
	int flags = 0;
	linenum_t contextlinecount = 0;
	int i;
	long fsizes[2];
	FILE* infd[2];
	DIFF diff;
	DIFFBLOCK* blocks;
	size_t numBlocks;
	MEMORYSTREAM ms[2];
	MEMORYSTREAM mo;
	while (argc > 1 && *(ap = argv[1]) == '-' && *++ap != '\0')
	{
		while (*ap != '\0')
		{
			switch ((*ap++))
			{
			case 'b':
				flags |= DIFF_COMPARE_IGNOREWHITESPACE;
				break;
			case 'u':
				flags |= DIFF_OUTPUT_UNIFIED;
				break;
			case 'c':
				flags |= DIFF_OUTPUT_CONTEXT;
				if (*ap > '0' && *ap <= '9')
					contextlinecount = *ap++ - '0';
				else
					contextlinecount = 3;
				break;
			case 'e':
				flags |= DIFF_OUTPUT_ED;
				break;
			case 'i':
				flags |= DIFF_COMPARE_IGNORECASE;
				break;
			case 'w':
				flags |= DIFF_COMPARE_BYWORD;
				break;
			case 'a':
				flags |= DIFF_COMPARE_BYCHAR;
				break;
			default:
				fprintf(stderr,
						"Warning, bad option '%c'\n",
						ap[-1]);
				break;
			}
		}

		argc--;
		argv++;
	}

	if (argc != 3)
	{
		fprintf(stderr, "%s", "Usage: diff [-options] file1 file2");
		putc('\n', stderr);
		exit(1);
	}

	if ((flags & (DIFF_OUTPUT_UNIFIED | DIFF_OUTPUT_CONTEXT))
			== (DIFF_OUTPUT_UNIFIED | DIFF_OUTPUT_CONTEXT))
	{
		fprintf(stderr,
				"Warning, -u and -c are incompatible, -u supressed.\n");
		flags = DIFF_OUTPUT_CONTEXT;
	}

	if ((flags & (DIFF_OUTPUT_ED | DIFF_OUTPUT_CONTEXT))
			== (DIFF_OUTPUT_ED | DIFF_OUTPUT_CONTEXT))
	{
		fprintf(stderr,
				"Warning, -c and -e are incompatible, -c suppressed.\n");
		flags = DIFF_OUTPUT_ED;
		contextlinecount = 0;
	}
	argv++;
	for (i = 0; i <= 1; i++)
	{
		infd[i] = fopen(argv[i], "rb");
		if (!infd[i])
			cant(argv[i], "input", 2);		/* Fatal error */
	}

	if (infd[0] == NULL && infd[1] == NULL)
	{
		cant(argv[0], "input", 0);
		cant(argv[1], "input", 1);
	}

#if defined(_WIN32)
	flags |= DIFF_OUTPUT_WINDOWSNEWLINES;
#elif defined(macintosh)
	flags |= DIFF_OUTPUT_MACNEWLINES;
#endif
	/* read in the files */
	fsizes[0] = getFILEsize(infd[0]);
	fsizes[1] = getFILEsize(infd[1]);
	ms[0].mData = (unsigned char*)malloc(fsizes[0]);
	ms[0].mPosition = 0;
	ms[0].mSize = fsizes[0];
	fread(ms[0].mData, 1, fsizes[0], infd[0]);
	ms[1].mData = (unsigned char*)malloc(fsizes[1]);
	ms[1].mPosition = 0;
	ms[1].mSize = fsizes[1];
	fread(ms[1].mData, 1, fsizes[1], infd[1]);
	/* resume our normal programming */
	Diff_Create(&diff, &ms[0], &ms[1]);
	Diff_Compare(&diff, flags);
	blocks = NULL; numBlocks = 0;
	Diff_FillBlocks(&diff, blocks, &numBlocks);
	blocks = (DIFFBLOCK*)malloc(sizeof(DIFFBLOCK) * numBlocks);
	Diff_FillBlocks(&diff, blocks, &numBlocks);
	mo.mData = NULL;
	mo.mPosition = 0;
	mo.mSize = 0;
	Diff_Output(&diff, &mo,
				flags,
				contextlinecount,
				argv[0], argv[1], blocks, numBlocks);
	mo.mData = (unsigned char*)realloc(mo.mData, mo.mSize + 1);
	mo.mData[mo.mSize] = '\0';
	printf("%s", mo.mData);
	/* cleanup */
	free(blocks);
	free(mo.mData);
	free(ms[0].mData);
	free(ms[1].mData);
	Diff_Destroy(&diff);

	fclose(infd[0]);
	fclose(infd[1]);

	return 0;
}