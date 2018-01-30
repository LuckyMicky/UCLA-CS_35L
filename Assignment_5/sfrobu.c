// sfrobu.c

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define SPACE       ' '

static size_t noc = 0;

void Is_MemErr(void* ptr);
void testIOErr(ssize_t ret);
void reportErr(const char* msg);
void strWrite(const char* str);

char decode(const char c);
int  frobcmp(char const* a, char const* b);
int  cmp_Wrapper(const void* a, const void* b);
size_t initLinebuf(char** linebuf, char* buf, size_t size);

int main(void)
{
    /* Declare Variables */
    int (* cmp) (const void*, const void*);
    char* input, * input2, ** linebuf, * line, cur_Char;
    char read_buf[2];
    size_t num_of_line, size_of_line, size_of_buffer, i, size_of_file;
    int isEnd, isSpace;
    ssize_t ret;
    struct stat buf;
    
    /* Initialize variables */
    size_of_buffer = 0;
    num_of_line = 0;
    size_of_line = 0;
    isEnd = 0;
    cmp = &cmp_Wrapper;
    
    /* File Information */
    ret = fstat(STDIN_FILENO, &buf);
    testIOErr(ret);
    size_of_file = buf.st_size + 1;
    
    /* Setup Initial Buffer */
    input = (char*) malloc(sizeof(char) * size_of_file);
    Is_MemErr(input);
    line = input;
    
/*  Using feof(STDIN_FILENO as the terminating condition to avoid a char with value at the end) */
    while (!isEnd)
    {
        ret = read(STDIN_FILENO, read_buf, 1);
        testIOErr(ret);
        cur_Char = read_buf[0];
        isSpace = cur_Char == SPACE;
        isEnd = ! ret;
        if (!size_of_line && isSpace)
            continue;
	/* Expands the size of the buffer */
        if (size_of_buffer == size_of_file)
        {
            size_of_file *= 2;
            input2 = (char*) realloc(input, sizeof(char) * size_of_file);
            Is_MemErr(input2);
            input = input2;
        }
        
        /* Store the current character */
        if (!isEnd)
        {
            input[size_of_buffer++] = cur_Char;
            size_of_line++;
            if (!isSpace)
                continue;
        }
        else
        {
            if (!size_of_buffer)
            {
                free(input);
                return 0;         /* Empty file or a file with only spaces */
            }
            /* Append a space if there is none */
            if (input[size_of_buffer-1] != SPACE)
                input[size_of_buffer++] = SPACE;
            if (!size_of_line)
                break;
        }

        /* New Line */
        num_of_line++;
        size_of_line = 0;
    }
    linebuf = (char**) malloc(sizeof(char*) * num_of_line);
    Is_MemErr(linebuf);
    
    if (num_of_line != initLinebuf(linebuf, input, size_of_buffer))
        reportErr("Wrong Line Number.");
    
    /* Sort the input */
    qsort(linebuf, num_of_line, sizeof(char*), cmp);
    
    /* Output results */
    for (i = 0; i < num_of_line; i++)
        strWrite(linebuf[i]);
    
    /* Free input array */
    free(linebuf);
    free(input);
    fprintf(stderr, "Comparisons: %ld\n", noc);
    return 0;
}

/* Report error */
inline
void reportErr(const char* msg)
{
    fprintf(stderr, "%s Error\n", msg);
    exit(1);
}

/* Test IO Stream Error */
inline
void testIOErr(ssize_t ret)
{
    if (ret < 0)
      reportErr("IO");
}

/* Check if malloc / realloc allocates memory successfully */
inline
void Is_MemErr(void* ptr)
{
    if (ptr == NULL)
      reportErr("Memory Allocation");
}

/*  Decode each character */
inline
char decode(const char c)
{
    return c ^ 42;      /* 00101010 is 0x2a, 42 */
}

/* Wrapper function */
inline
int cmp_Wrapper(const void* a, const void* b)
{
    return frobcmp(*((const char**) a), *((const char**) b));
}

void strWrite(const char* str)
{
    ssize_t ret;
    for (;;)
    {
        ret = write(STDOUT_FILENO, str, 1);
        testIOErr(ret);
        if (*str++ == SPACE)
            return;
    }
}

size_t initLinebuf(char** linebuf, char* buf, size_t size)
{
    size_t i, num_of_line;
    char* line = buf;
    for (i = 0, num_of_line = 0; i < size; i++)
    {
        if (buf[i] == SPACE)
        {
            linebuf[num_of_line++] = line;
            line = buf + i + 1;
        }
    }
    return num_of_line;
}

/* Compare two frobnicated characters */
int frobcmp(char const* a, char const* b)
{
    noc++;
    for (;*a == *b; a++, b++)
        if (*a == SPACE)
            return 0;
    return ((decode(*a) < (decode(*b)) ? -1 : 1));
    
}
