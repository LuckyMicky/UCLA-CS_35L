/* sfrob */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define SPACE     ' '
#define INIT_SIZE 20


void testIOErr(FILE* p);
void printErr(const char* msg);
void strWrite(const char* str);

char decode(const char c);
int  frobcmp(char const* a, char const* b);
int  cmpWrapper(const void* a, const void* b);
void Is_MemErr(void* ptr);
void initLinebuf(char** linebuf, char* buf, size_t size);

int main(void)
{
  /* Declare Variables */
  int (* cmp) (const void*, const void*);
  char* input, * input_2, ** linebuf, cur_char;
  size_t num_of_line, size_of_line, size_of_buffer, i, size_of_file;
  int isEnd, isSpace;
    
  /* Initialize variables */
  size_of_buffer = 0;
  num_of_line = 0; 
  size_of_line = 0; 
  isEnd = feof(stdin);
  size_of_file = INIT_SIZE;
  cmp = &cmpWrapper;
    
  /* Setup Initial Buffer */
  input = (char*) malloc(sizeof(char) * size_of_file);
  Is_MemErr(input);
    
  /*  Using feof(STDIN_FILENO as the terminating condition to avoid a char with value at the end) */      
  while (!isEnd)
    {
      cur_char = getchar();
      testIOErr(stdin);
      isSpace = (cur_char == SPACE);
      isEnd = feof(stdin);
      if (!size_of_line && isSpace)
	continue;
      /* Expands the size of the buffer */
      if (size_of_buffer == size_of_file)
        {
	  size_of_file = 2*size_of_file;
	  input_2 = (char*) realloc(input, sizeof(char) * size_of_file);
	  Is_MemErr(input_2);
	  input = input_2;
        }
        
      /* Store the current character */
      if (!isEnd)
        {
	  input[size_of_buffer++] = cur_char;
	  size_of_line++;
	  if (!isSpace)
	    continue;
        }
      else
        {
	  if (!size_of_buffer)  /* Empty file or a file has only space */
            {
	      free(input);
	      return 0;         
            }
	  if (input[size_of_buffer-1] != SPACE)
	    input[size_of_buffer++] = SPACE; /* Append a space if there is none */
	  if ( ! size_of_line)
	    break;
        }
        
      num_of_line++;  /* New Line */
      size_of_line = 0;
    }
  linebuf = (char**) malloc(sizeof(char*) * num_of_line);
  Is_MemErr(linebuf);
    
  initLinebuf(linebuf, input, size_of_buffer);
    
  /* Sort the input */
  qsort(linebuf, num_of_line, sizeof(char*), cmp);
    
  /* Output results */
  for (i = 0; i < num_of_line; i++)
    strWrite(linebuf[i]);
    
  /* Free input array */
  free(linebuf);
  free(input);
  return 0;
}

/* Report error */
inline
void printErr(const char* msg)
{
  fprintf(stderr, "%s Error: %d\n", msg, errno);
  exit(1);
}

/* Test IO Stream  Error */
inline
void testIOErr(FILE* p)
{
  if (ferror(p))      
    printErr("IO Error");
}

/* Check if malloc / realloc allocates memory successfully */
inline
void Is_MemErr(void* ptr)
{
  if (ptr == NULL)    
    printErr("Memory Allocation Failure");
}

/*  Decode each character */
inline
char decode(const char c)
{
  return c ^ 42;      /* 00101010 is 0x2a, 42 */
}

/* Wrapper function */
inline
int cmpWrapper(const void* a, const void* b)
{
  return frobcmp(*((const char**) a), *((const char**) b));
}

void strWrite(const char* str)
{
  for (;;)
    {
      putchar(*str);
      testIOErr(stdout);
      if (*str++ == SPACE)
	return;
    }
}

void initLinebuf(char** linebuf, char* buf, size_t size)
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
}

/* Compare two frobnicated characters */
int frobcmp(char const* a, char const* b)
{
  for ( ; *a == *b; a++, b++)
    if (*a == SPACE)
      return 0;
  return ((decode(*a) < (decode(*b)) ? -1 : 1));
    
}
