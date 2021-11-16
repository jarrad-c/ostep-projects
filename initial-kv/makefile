
SRCS = src/*.c
TARG = kv
CC = gcc
OPTS = -Wall -O

OBJS = $(SRCS:.c=.o)

all: $(TARG)

$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARG)
