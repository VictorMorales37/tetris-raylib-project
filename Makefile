#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

CC      = gcc
CFLAGS  := -Wall -Wextra -g
OUTPUT  := output
SRC     := src
INCLUDE := include
LIB     := lib

ifeq ($(OS),Windows_NT)
    MAIN        := main.exe
    SOURCEDIRS  := $(SRC)
    INCLUDEDIRS := $(INCLUDE)
    LIBDIRS     := $(LIB)/windows
    LFLAGS      := -lraylib -lopengl32 -lgdi32 -lwinmm
    FIXPATH     = $(subst /,\,$1)
    RM          := del /q /f
    MD          := mkdir
else
    MAIN        := main
    SOURCEDIRS  := $(shell find $(SRC) -type d)
    INCLUDEDIRS := $(shell find $(INCLUDE) -type d)
    LIBDIRS     := $(LIB)/linux
    LFLAGS      := -lraylib -lm -lpthread -ldl -lrt -lX11
    FIXPATH     = $1
    RM          := rm -f
    MD          := mkdir -p
endif

INCLUDES    := $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
LIBS        := $(patsubst %,-L%, $(LIBDIRS:%/=%))
SOURCES     := $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))
OBJECTS     := $(SOURCES:.c=.o)
DEPS        := $(OBJECTS:.o=.d)
OUTPUTMAIN  := $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

-include $(DEPS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -MMD $< -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!