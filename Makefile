# set this to your desired output executable filename
TARGET_NAME ?= game

# these lines generate the necessary file list variables
SOURCE_FILES = $(wildcard *.c)
OBJECT_FILES := $(SOURCE_FILES:.c=.o)
DEPENDENCY_FILES := $(OBJECT_FILES:.o=.d)

# set this to your EZAL installation directory
EZAL_HOME ?= $(HOME)/ezal
EZAL_INCLUDE ?= $(EZAL_HOME)/include
EZAL_LIBRARY ?= $(EZAL_HOME)/lib

# EZAL compilation and linking flags
EZAL_CFLAGS ?= -I$(EZAL_INCLUDE)
EZAL_LDFLAGS ?= -L$(EZAL_LIBRARY) -lezal

# set this to your C compiler of choice
CC := gcc

# configure your compilation flags as needed
CFLAGS ?= -DDEBUG -O0 -MMD -MP -g

# configure your linking flags as needed
# LDFLAGS ?=

# EZAL depends on Allegro 5
A5_CFLAGS ?= $(shell pkg-config \
	allegro-5 allegro_main-5 allegro_font-5 allegro_ttf-5 \
	allegro_image-5 allegro_audio-5 allegro_acodec-5 \
	allegro_primitives-5 --cflags)

A5_LDFLAGS ?= $(shell pkg-config \
	allegro-5 allegro_main-5 allegro_font-5 allegro_ttf-5 \
	allegro_image-5 allegro_audio-5 allegro_acodec-5 \
	allegro_primitives-5 --libs)

MKDIRP ?= mkdir -p

.PHONY: clean

$(TARGET_NAME): $(OBJECT_FILES)
	@echo "Linking Executable: $(TARGET_NAME)"
	$(CC) $(A5_LDFLAGS) $(EZAL_LDFLAGS) $(LDFLAGS) $^ -o $@
%.o: %.c
	@echo "Compiling Source"
	$(CC) $(A5_CFLAGS) $(EZAL_CFLAGS) $(CFLAGS) -c $< -o $@
clean:
	@echo "Cleaning Project"
	@$(RM) $(TARGET_NAME) $(OBJECT_FILES) $(DEPENDENCY_FILES)
-include $(DEPENDENCY_FILES)
