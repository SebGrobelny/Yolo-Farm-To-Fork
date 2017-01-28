/* Source Code */

#include <stdio.h>
#include <stdlib.h>

//prototypes
char *read_line(void);


int main()
{
  char *myline = read_line();
  puts (myline);
  return 0;
}


char *read_line(void)
{

    char *line = NULL;
    size_t nbytes = 0;
    ssize_t read = getline(&line,&nbytes,stdin);
    if (read < 0)
    {
      puts ("Error reading line\n");
      return NULL;
    }

    return line;

}
