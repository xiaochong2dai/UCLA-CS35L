#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <stdlib.h>

int comp=0;

int rot13cmp (const char *a, const char *b)
{
  int i;
  char tempa;
  char tempb;
  //as long as either a or b reaches '\n', then stop comparison
  for (i=0; *(*(const char **)a+i)!='\n' && *(*(const char **)b+i)!='\n'; i++)
    {
      tempa=*(*(const char **)a+i);
      tempb=*(*(const char **)b+i);
      //convert the character to the encoded version for later comparison
      if ((tempa>64 && tempa<91) && (tempb>96 && tempb<123))
	return -1;
      else if ((tempa>96 && tempa<123) && (tempb>64 && tempb<91))
	return 1;
      else
	{
	  if (((tempa>64 && tempa<78) && (tempb>77 && tempb<91))
		   || ((tempa>96 && tempa<110) && (tempb>109 && tempb<123)))
	    return 1;
	  else if (((tempa>77 && tempa<91) && (tempb>64 && tempb<78))
                   || ((tempa>109 && tempa<123) && (tempb>96 && tempb<110)))
	    return -1;
	  else
	    {
	      if ((tempa - tempb)!=0)
                return (tempa - tempb);
              else
                continue;
	    }
	}
    }
  //from above can't decide which one is bigger, check who reached '\n' first
  if (*(*(const char **)a+i) =='\n' && *(*(const char **)b+i)!='\n')
    return -1;
  else if (*(*(const char **)a+i) !='\n' && *(*(const char **)b+i)=='\n')
    return 1;
  //the only case left is when both the characters are the newline characters
  else
    return 0;
}

int cmp(const void *a, const void *b)
{
  comp++;
  return rot13cmp((const char *)a , (const char  *)b); 
}

int main()
{
  struct stat buf;
  char *ArrayPtr;
  int m_size;
  //check if the file is a regular file, if it is
  if((buf.st_mode & S_IFMT) == S_IFREG)
  {
    int file_info= fstat(0, &buf);
    if (file_info ==-1)
      {
	fprintf(stderr, "fstat function failed.");
	exit(1);
      }
    (off_t) m_size;
    m_size=buf.st_size;
    ArrayPtr = (char*) malloc(m_size);
  }
  //if the input is not a regular file
  else
  {
    m_size=2048;
    ArrayPtr = (char*) malloc(sizeof(char) * m_size);
  }
  //use an array to store all the inputs and allocate space for it
  if (ArrayPtr == NULL){
    fprintf(stderr, "An error occurred when allocating memory.");
    exit(1);
  }
  //set a flag to check if the file is empty, initially set it to be 1 (empty) 
  int flag=1, len=0, counter=0, wordCounter=0;
  int j,k,m,wl_index;
  //  char in_char;
  while(read(0,ArrayPtr+counter,1))
    {
       //not EOF, so set file not be empty
      flag=0;
      //      write(1,ArrayPtr+counter,1);
      //      ArrayPtr[counter]=in_char;
      //read(0,&in_char,1);
      counter++;
      //If all the space is used, then we need to reallocate memory
      if (counter == m_size)
        {
       	  if((buf.st_mode == S_IFREG && read(0,ArrayPtr+counter,1) !=0) || buf.st_mode != S_IFREG)
	    {
	      //everytime reallocating, we can double the size
	      ArrayPtr = (char*) realloc(ArrayPtr, m_size * 2);
	      if (ArrayPtr == NULL)
		{
		  fprintf(stderr, "An error occurred when reallocating memory.");
		  exit(1);
		}
	      //update memory size
	      m_size *= 2;
	    }
        }
    }
  //check if the file is empty
  if (flag)
    return 0;
  //Append newline if no newline at end of file
  if (ArrayPtr[counter-1] != '\n')
    {
      ArrayPtr[counter]='\n';
      counter++;
    }
  //get the number of total words 
  for (j=0; j<counter; j++)
    {
      if (ArrayPtr[j]=='\n')
	wordCounter++;
    }
  //allocate memory for the second array that points to beginning of each word
  char **wordPtr = (char **) malloc(sizeof(char *) * wordCounter);
  if (wordPtr == NULL){
    fprintf(stderr, "An error occurred when allocating memory.");
    exit(1);
  }
  //allocate memory for the third array that record the length of each string word
  int *wordLength = (int *) malloc(sizeof(int) * wordCounter);
  if (wordLength == NULL){
    fprintf(stderr, "An error occurred when allocating memory.");
    exit(1);
  }
  //create a copy of the first array pointer which points to the beginning of the input
  char *APtrCopy=ArrayPtr;
  //locate each element of array wordPtr, let it point to begining of each word
  for (k=0;k<wordCounter;k++)
    {
      wordPtr[k]=APtrCopy;
      while(*APtrCopy!='\n')
        {
	  APtrCopy++;
        }
      APtrCopy++;
    }
  //apply qsort function to sort the words
  qsort(wordPtr, wordCounter, sizeof(char*), cmp);
  //the words are sorted now, we need to know the lengths and print them out
  for (wl_index=0;wl_index<wordCounter;wl_index++)
    {
      APtrCopy=wordPtr[wl_index];
      while (*APtrCopy!='\n')
        {
	  APtrCopy++;
	  len++;
        }
      //the len doesn't include '\n', so we need to use len +1
      for (m=0; m<len+1;m++)
        {
	  //	  printf("%c", *wordPtr[wl_index]);
	  write(1,wordPtr[wl_index],1);
	  wordPtr[wl_index]++;
        }
      len=0;
    }
  //free allocated memory after printing
  fprintf(stderr,"Number of comparisons: %d\n",comp);
  free(ArrayPtr);
  free(wordPtr);
  free(wordLength);
  return 0;
}
