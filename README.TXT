This is a quick port of Sun's PCNFSD to OS/2 with IBM TCP/IP 2.0. It
will not run with TCP/IP 1.2.1. The port is based on pcnfsd 1.4 and
was done pretty basic. If you encounter problems, you may want try to
fix them yourself and send me your changes. I will probably not spend
more time on it.

The authentication uses a passwd file in \tcpip\etc which looks like
that included here. To add a new user, just add an appropriate line to
the passwd file, with a * in the password field. Then use the included
passwd.exe program to modify the password for that user.

Printing seems to work too, using the physical port names lpt1, lpt2
etc. as the pcnfsd printer names. The pcnfsd spools the files using
the OS/2 print command, to the appropriate queue. The spooling is a
bit slow, since the pcnfsd has to wait for nfsd to completely receive, 
flush and close the spool file before it can spool it. The IBM nfsd
seems to need a few seconds for that.

The only option for pcnfsd is the spool directory path for PC-NFS
print spooling. The default is \spool\pcnfs on the current drive. You
should in any case give a fully qualifies path name to that directory
as argument to pcnfsd and you must also have that path name exported
for everyone by your NFS server on the same machine (file
\tcpip\etc\exports).

There must be newer versions of Sun's pcnfsd but I have not tried to
get one. This was basically a test to check IBM TCP/IP 2.0 32-bit RPC
calls, nothing more. If you want to improve the port, fine.

To recompile, you need two things:

- IBM C Set++

  The reason for that is that apparently IBM's RPC library uses their
  own _Optlink calling convention (unlike their socket library, for
  example, which uses _System).

- GNU ufc (ultra fast crypt) library

  This is available from sites in the US which archive GNU source code
  (look for GNU glibc) and from sites outside the US too. For example,
  you can find it on ftp.informatik.tu-muenchen.de in the file
  /pub/comp/os/os2/crypt/gnuufc.zip. It works unchanged, just unpack
  it into a directory ufc below the pcnfsd directory.

Kai Uwe Rommel

--
/* Kai Uwe Rommel                                      Muenchen, Germany *
 * rommel@ars.muc.de                              CompuServe 100265,2651 *
 * rommel@informatik.tu-muenchen.de                  Fax +49 89 324 4524 */

DOS ... is still a real mode only non-reentrant interrupt
handler, and always will be.                -Russell Williams
