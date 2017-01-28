
program_NAME := dickshell
program_SRCS := shell.c parser.c arrow.c noncanmode.c hstack.c
program_OBJS := ${program_SRCS:.c=.o}	# substitution expression to replace ".c" with ".o"
CFLAGS += -g -Wall

.PHONY: all clean distclean # phony targets


all: $(program_NAME)

# link object files to the output file
$(program_NAME): $(program_OBJS)
	$(LINK.cc) $(program_OBJS) -o $(program_NAME)

clean:
	- $(RM) $(program_NAME)
	- $(RM) $(program_OBJS)

distclean: clean

depend:
	makedepend -- $(CFLAGS) -- $(program_SRCS)

# Don't place anything below this line, since
# the make depend program will overwrite it
# DO NOT DELETE THIS LINE -- make depend depends on it.
