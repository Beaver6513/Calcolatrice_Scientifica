CC = gcc
DB = gdb
BINARY = main
TARGETDIR = ./bin/
CODEDIRS = . lib
OBJDIR = obj
DEPDIR = dep
INCDIRS = . ./include/
LIBS = -lm

VALFLAGS = --leak-check=full --show-leak-kinds=all -s --track-origins=yes
DEPFLAGS = -MP -MD
CFLAGS = -Wno-implicit-function-declaration -g -Wall -ggdb3 $(foreach D, $(INCDIRS), -I$(D)) $(DEPFLAGS)

CFILES = $(foreach D, $(CODEDIRS), $(wildcard $(D)/*.c))
OBJFILES = $(patsubst %.c, %.o, $(CFILES))
DEPFILES = $(patsubst %.c, %.d, $(CFILES))

all: $(BINARY)

$(BINARY): $(OBJFILES)
	$(CC) -o $(TARGETDIR)$@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -r $(TARGETDIR)$(BINARY) $(OBJFILES) $(DEPFILES)

run: $(BINARY)
	@$(TARGETDIR)$(BINARY)

debug: $(BINARY)
	@$(DB) $(TARGETDIR)$(BINARY)

val: $(BINARY)
	valgrind $(VALFLAGS) $(TARGETDIR)$(BINARY)

-include $(DEPFILES)
