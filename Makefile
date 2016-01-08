# Makefile for pcnfsd

CC = icc -q -O
CFLAGS = -I. -DOS2

OBJN = pcnfsd.obj pwd.obj crypt.obj crypt_util.obj
OBJP = passwd.obj getpass.obj pwd.obj crypt.obj crypt_util.obj
LIBS = rpc32dll.lib

.SUFFIXES: .c .obj

.c.obj:
	$(CC) $(CFLAGS) -c $<

all: pcnfsd.exe passwd.exe

pcnfsd.exe: $(OBJN)
	$(CC) -Fe $@ $(OBJN) $(LIBS)

passwd.exe: $(OBJP)
	$(CC) -Fe $@ $(OBJP)

crypt.obj: ufc/crypt.c
	$(CC) $(CFLAGS) -c ufc/$*.c

crypt_util.obj: ufc/crypt_util.c
	$(CC) $(CFLAGS) -c ufc/$*.c
