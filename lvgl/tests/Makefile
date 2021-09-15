#
# Makefile
#
CC ?= gcc
LVGL_DIR ?= ${shell pwd}/../..
LVGL_DIR_NAME ?= lvgl

WARNINGS = -Werror -Wall -Wextra \
           -Wshadow -Wundef -Wmaybe-uninitialized -Wmissing-prototypes -Wpointer-arith -Wuninitialized \
           -Wunreachable-code -Wreturn-type -Wmultichar -Wformat-security -Wdouble-promotion -Wclobbered -Wdeprecated  \
           -Wempty-body -Wshift-negative-value -Wstack-usage=2048 -pedantic-errors \
           -Wtype-limits -Wsizeof-pointer-memaccess -Wpedantic -Wmissing-prototypes -Wno-discarded-qualifiers

#-Wno-unused-value -Wno-unused-parameter
OPTIMIZATION ?= -O3 -g0

CFLAGS ?= -I$(LVGL_DIR)/ $(DEFINES) $(WARNINGS) $(OPTIMIZATION) -I$(LVGL_DIR) -I.

LDFLAGS ?=  -lpng
BIN ?= demo

#Collect the files to compile
MAINSRC = ./lv_test_main.c

include ../lvgl.mk


OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

## MAINOBJ -> OBJFILES

all: default

%.o: %.c
	@$(CC)  $(CFLAGS) -c $< -o $@
	@echo "CC $<"

default: $(AOBJS) $(COBJS) $(MAINOBJ)
	$(CC) -o $(BIN) $(MAINOBJ) $(AOBJS) $(COBJS) $(LDFLAGS)

clean:
	rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ)
