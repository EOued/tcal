CFILES = \
	drawer/renderer.c\
	drawer/views.c\
	drawer/pages.c\
	drawer/drawer.c\
	functions/functions.c\
	main.c\

SRCDIR = src
SOURCES = $(foreach file, $(CFILES), $(SRCDIR)/$(file))
OBJDIR = build/obj
INCLUDE = -I$(SRCDIR)/headers
OFILES = $(CFILES:.c=.o)
SUBDIRS = drawer functions
TARGETS = $(foreach file, $(OFILES), $(OBJDIR)/$(file))
EXECDIR = build/target
EXECNAME = tcal
EXEC = $(EXECDIR)/$(EXECNAME)
GXX = gcc

FLAGS=-g -Wall -Wextra -Wvla -pedantic -fsanitize=address,undefined -pthread -lncurses
POSTFLAGS=-lm

debug=0
ifeq ($(debug),1)
FLAGS += -DDEBUG
else
FLAGS += -Werror
endif

test = 0
ifeq ($(test),1)
FLAGS += -DTEST
endif


$(shell mkdir -p $(OBJDIR))
$(shell mkdir -p $(EXECDIR))
$(foreach dir, $(SUBDIRS), $(shell mkdir -p $(OBJDIR)/$(dir)))

# Rule to generate object files from source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(GXX) $(FLAGS) $(INCLUDE) -c $< -o $@ $(POSTFLAGS)

# The final target depends on all object files
$(EXEC): $(TARGETS)
	$(GXX) $(FLAGS) $(INCLUDE) -o $(EXEC) $(TARGETS) $(POSTFLAGS)

# Clean rule
clean:
	rm -rf $(EXEC) $(OBJDIR);


