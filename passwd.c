/* passwd.c
 *
 * Author:  Kai Uwe Rommel <rommel@jonas>
 * Created: Wed Mar 16 1994
 */

#include <stdio.h>
#include <pwd.h>

#define SALT "pc"

extern char *getpass(char *);
extern char *crypt(char *, char*);

int main(int argc, char **argv)
{
  struct passwd *pw;
  char user[64], password[64], crypted[64];
  char *cp;

  if (argc != 2)
  {
    printf("Usage: passwd <user>\n");
    return 1;
  }

  strcpy(user, argv[1]);
  
  if ((pw = getpwnam(user)) == NULL)
  {
    printf("No such user.\n");
    return 1;
  }

  if (strcmp(pw->pw_passwd, "*"))
  {
    strcpy(password, getpass("Old password:"));
    if (strcmp(pw->pw_passwd, crypt(password, pw->pw_passwd)))
    {
      printf("Incorrect password.\n");
      return 1;
    }
  }
  
  strcpy(password, getpass("New password:"));
  strcpy(crypted, getpass("Re-type new password:"));

  if (strcmp(password, crypted))
  {
    printf("New passwords don't match.\n");
    return 1;
  }
  
  strcpy(crypted, crypt(password, SALT));

  if (strcmp(pw->pw_passwd, crypted) == 0)
  {
    printf("Password unchanged.\n");
    return 1;
  }
  
  if (setpwnam(user, crypted))
  {
    printf("Update of passwd file failed.\n");
    return 1;
  }

  printf("Password changed.\n");

  return 0;
}

/* end of passwd.c */
