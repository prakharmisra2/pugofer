.SUFFIXES	: .y .c .h .o .gs
OBJECTS		= storage.o input.o static.o type.o compiler.o \
		  parser.o pug.o
IOBJECTS	= gofer.o builtin.o  machine.o output.o $(OBJECTS)
COBJECTS	= gofc.o cbuiltin.o cmachine.o $(OBJECTS)

# Edit the following settings as required.
# There are two choices of command line editor that can be used with Gofer:
#
#  GNU readline:		usual GNU sources (e.g. bash distribution)
#  add -DUSE_READLINE=1 to CFLAGS and libreadline.a -ltermcap to LDFLAGS
#				      (or maybe -lreadline -ltermcap)
#
#  editline:			(comp.sources.misc, vol 31, issue 71)
#  add -DUSE_READLINE=1 to CFLAGS and libedit.a to LDFLAGS
#				      (or maybe -ledit)
#
# The best bet is to `touch prelude.h' after changing these settings to
# ensure that the whole collection of files is recompiled with the correct
# settings.

# IMPORTANT: make sure that you use the same compiler to build gofc
# executables (often by setting an appropriate value in the gofcc script)
# as you use to build the file runtime.o (as specified by the CC macro
# below).

CC		?= gcc

#CFLAGS		= -DUSE_READLINE=1
CFLAGS		?=
LDFLAGS    	?= -lm
OPTFLAGS	= -O
OPT1		=
#OPT1		= -O1

#all		: gofer gofc runtime.o

pug		: $(IOBJECTS)
		  $(CC) $(CFLAGS) $(OPTFLAGS) $(IOBJECTS) -s -o pug $(LDFLAGS)
##		  strip pug

gofc		: $(COBJECTS)
		  $(CC) $(CFLAGS) $(OPTFLAGS) $(COBJECTS) -o gofc $(LDFLAGS)
		  strip gofc

test     : pug
		   cd .. && python3 -m unittest discover -v

test-reports : pug
		   cd .. && python3 test/run_tests_with_reports.py --verbose --timeout 180

test-ci : pug
		   cd .. && python3 test/run_tests_with_reports.py --verbose --xml-output test-reports --timeout 180

.c.o		:
		  $(CC) -c $(CFLAGS) $(OPTFLAGS) $<

clean		:
		  rm *.o $(TESTS)

install		:
		  mv gofer ..

.gs		:
		  ./gofc $*.gs
		  $(CC) $(OPTFLAGS) $*.c runtime.o -o $* $(LDFLAGS)
		  rm $*.c
		  strip $*

.gp		:
		  ./gofc + $*.gp
		  $(CC) $(OPTFLAGS) $*.c runtime.o -o $* $(LDFLAGS)
		  rm $*.c
		  strip $*

storage.h	: pug.h
parser.c	: parser.y input.h
		  yacc -d parser.y
		  mv y.tab.c parser.c
		  mv y.tab.h parser.h

gofer.o		: prelude.h storage.h connect.h errors.h \
		  command.h machdep.c commonui.c
gofc.o		: prelude.h storage.h connect.h errors.h \
		  command.h machdep.c commonui.c output.c
runtime.o	: prelude.h gofc.h machdep.c markscan.c twospace.c
storage.o	: prelude.h storage.h connect.h errors.h
input.o		: prelude.h storage.h connect.h errors.h command.h
parser.o	: prelude.h storage.h connect.h errors.h command.h
static.o	: prelude.h storage.h connect.h errors.h scc.c
type.o		: prelude.h storage.h connect.h errors.h preds.c kind.c subst.c
output.o	: prelude.h storage.h connect.h errors.h
compiler.o	: prelude.h storage.h connect.h errors.h
		  $(CC) -c $(OPT1) $(CFLAGS) compiler.c
machine.o	: prelude.h storage.h connect.h errors.h
cmachine.o	: prelude.h storage.h connect.h errors.h
		  $(CC) -c $(OPT1) $(CFLAGS) cmachine.c
builtin.o	: prelude.h storage.h connect.h errors.h prims.c
cbuiltin.o	: prelude.h storage.h connect.h errors.h prims.c
pug.o		: prelude.h pug.h
