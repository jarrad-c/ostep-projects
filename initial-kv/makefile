SRCDIR = src

SRCS = $(SRCDIR)/kv.c $(SRCDIR)/database.c
INC = inc
TARG = kv

CC = clang
OPTS = -Wall -Werror -O -g

OBJS = $(SRCS:.c=.o)

all: $(TARG)

$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS) 

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@ -I$(INC)


clean:
	rm -f $(OBJS) $(TARG)
