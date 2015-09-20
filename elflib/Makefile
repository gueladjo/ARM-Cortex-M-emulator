TARGET=test_read_elf_file
TESTOBJ=armelf_test.o
# noms des executables utilisés durant la compilation/edition des liens
CC=`which gcc`
LD=`which gcc`
AR=`which ar`
RM=`which rm` -f
DOXYGEN=`which doxygen`

#options de compilation/edition des liens (compilation uniquement en DEBUG)
INCLUDE=-I$(INCDIR)/
CFLAGS=-Wall $(INCLUDE) -g -DDEBUG -DVERBOSE
LFLAGS=
ARFLAGS= ruc 

# definition des repertoires de source/destination
SRCDIR=src
TESTDIR=test
INCDIR=include
DOCDIR=doc

# les fichiers dont on peut se débarasser
GARBAGE=*~ $(SRCDIR)/*~ $(INCDIR)/*~ $(TESTDIR)/*~ $(SRCDIR)/*.orig $(INCDIR)/*.orig

# ou se trouve les sources (i.e., le *.c)
SRC=$(wildcard $(SRCDIR)/*.c)
# et les objets (i.e., le *.o)
OBJ_DBG=$(SRC:.c=.o)


# 1er target (celui executé par défaut quand make est lancé sans nom de cible) 
# affiche l'aide
all : 
	@echo ""
	@echo "Usage:"
	@echo ""
	@echo "make test     => build version with DEBUG messages enabled"
	@echo "make ar       => static library with DEBUG messages enabled"
	@echo "make doc      => produce the doxygen documentation"
	@echo "make clean    => clean everything"

# créé l'executable de test
test   : $(OBJ_DBG) $(TESTDIR)/$(TESTOBJ)
	$(LD) $^ $(LFLAGS) -o $(TARGET)

ar : $(OBJ_DBG)
	$(AR)  $(ARFLAGS) libelfapi.a $^
	
%.o : %.c
	$(CC) $< $(CFLAGS) -c -o $(basename $<).o

doc : 
	$(DOXYGEN)

clean : 
	$(RM) $(TARGET) $(SRCDIR)/*.o $(GARBAGE) 
	$(RM) -r $(DOCDIR)/*
