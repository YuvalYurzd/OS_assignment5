# Define compiler
CC = gcc

# Define compiler flags
CFLAGS = -Wall -Werror -O2

# Define linker
LD = gcc

# Define linker flags
LDFLAGS = 

# Define the source files
SRCS = st_pipeline.c active_object.c queue.c

# Define the object files
OBJS = $(SRCS:.c=.o)

# Define the output name
OUT = st_pipeline

# The "all" target will build the final binary
all: $(OUT)

# The final binary depends on the object files
$(OUT): $(OBJS)
	$(LD) -o $(OUT) $(OBJS) $(LDFLAGS)

# This is a "Pattern rule" to convert .c files into .o files
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove temporary and binary files
clean:
	rm -f $(OBJS) $(OUT)
