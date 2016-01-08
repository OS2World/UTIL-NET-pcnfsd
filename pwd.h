/* dummy for OS/2 */

struct passwd
{
  char *pw_name;
  char *pw_dir;
  char *pw_gecos;
  char *pw_passwd;
  int pw_uid;
  int pw_gid;
};

#define gcos_name(x, y)  x

struct passwd *getpwnam(char *);
int setpwnam(char *, char*);
