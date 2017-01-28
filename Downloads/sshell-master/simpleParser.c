//Simple Parser

#include <stdio.h>
#include <string.h>

#define MAXLINE 81

//The actual parser
void parse(char *line, char **argv) {
	while (*line != '\0') {
		while (*line == ' ' || *line == '\t' || *line == '\n') {
			*line++ = '\0';
		}
		*argv++ = line;
		while (*line != '\0' && *line!= ' ' && *line != '\t' && *line != '\n') {
			line++;
		}
	}
	*argv = '\0';
}

//Catches the commands in argv, right now only does so if they had whitespace around them.
void catchCommands(char **argv)
{
	int numArgs = 0;
	int i;

	while(argv[++numArgs] != NULL);
	for (i = 0; i < numArgs; i++) {
		if (strcmp(argv[i], "<") == 0) {
			printf("Argument number %d is command:  %s\n", i, argv[i]);
		}
		if (strcmp(argv[i], ">") == 0) {
                        printf("Argument number %d is command:  %s\n", i, argv[i]);
                }
		if (strcmp(argv[i], "|") == 0) {
                        printf("Argument number %d is command:  %s\n", i, argv[i]);
                }
		if (strcmp(argv[i], "&") == 0) {
                        printf("Argument number %d is command:  %s\n", i, argv[i]);
                }
	}
}

int main(void)
{
	char line[1024];
	char *argv[16];

	while(1) {
		printf("Arguments? ->");
		fgets(line,MAXLINE,stdin);
		printf("\n");
		parse(line, argv);
		if (strcmp(argv[0], "exit") == 0) {
			return 0;
		}
		catchCommands(argv);
	}
	return 0;
}
