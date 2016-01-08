#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pwd.h>

struct passwd *getpwnam(char *name)
{
  static struct passwd pw;
  static char buffer[256];
  char *ptr;
  FILE *passwd;
  int found = 0;

  if ((ptr = getenv("ETC")) == NULL)
    return NULL;

  strcpy(buffer, ptr);
  strcat(buffer, "/passwd");

  if ( (passwd = fopen(buffer, "r")) == NULL )
    return NULL;

  while ( fgets(buffer, sizeof(buffer), passwd) != NULL )
  {
    if (buffer[strlen(buffer) - 1] == '\n')
      buffer[strlen(buffer) - 1] = 0;

    if ( buffer[0] == '#' )
      continue;

    pw.pw_name = buffer;

    if ( (ptr = strchr(buffer, ':')) != NULL )
      *ptr++ = 0;
    else
      continue;

    if ( strcmp(name, pw.pw_name) == 0 )
    {
      pw.pw_passwd = ptr;

      if ( (ptr = strchr(ptr, ':')) != NULL )
	*ptr++ = 0;
      else
	continue;

      pw.pw_uid = atoi(ptr);

      if ( (ptr = strchr(ptr, ':')) != NULL )
	*ptr++ = 0;
      else
	continue;

      pw.pw_gid = atoi(ptr);

      if ( (ptr = strchr(ptr, ':')) != NULL )
	*ptr++ = 0;
      else
	continue;

      pw.pw_gecos = ptr;

      if ( (ptr = strchr(ptr, ':')) != NULL )
        *ptr++ = 0;
      else
        continue;

      pw.pw_dir = ptr;

      if ( ptr[0] && ptr[1] && (ptr = strchr(ptr + 2, ':')) != NULL )
        *ptr++ = 0;   /* skip drive: */

      found = 1;

      break;
    }
  }

  fclose(passwd);

  if ( !found )
    return NULL;

  return &pw;
}

int setpwnam(char *name, char *crypted)
{
  static struct passwd pw;
  static char old[256], new[256], bak[256], line[256], buffer[256];
  char *ptr;
  FILE *passwd, *newpasswd;
  int found = 0;

  if ((ptr = getenv("ETC")) == NULL)
    return NULL;

  strcpy(old, ptr);
  strcat(old, "/passwd");
  strcpy(new, old);
  strcat(new, ".new");
  strcpy(bak, old);
  strcat(bak, ".bak");

  if ( (passwd = fopen(old, "r")) == NULL )
    return -1;

  if ( (newpasswd = fopen(new, "w")) == NULL )
    return -1;

  while ( fgets(line, sizeof(line), passwd) != NULL )
  {
    strcpy(buffer, line);

    if (buffer[strlen(buffer) - 1] == '\n')
      buffer[strlen(buffer) - 1] = 0;

    if ( buffer[0] == '#' )
      goto done;

    pw.pw_name = buffer;

    if ( (ptr = strchr(buffer, ':')) != NULL )
      *ptr++ = 0;
    else
      goto done;

    if (strcmp(name, pw.pw_name) == 0)
    {
      pw.pw_passwd = ptr;

      if ( (ptr = strchr(ptr, ':')) != NULL )
	*ptr++ = 0;
      else
	goto done;

      pw.pw_uid = atoi(ptr);

      if ( (ptr = strchr(ptr, ':')) != NULL )
	*ptr++ = 0;
      else
	goto done;

      pw.pw_gid = atoi(ptr);

      if ( (ptr = strchr(ptr, ':')) != NULL )
	*ptr++ = 0;
      else
	goto done;

      pw.pw_gecos = ptr;

      if ( (ptr = strchr(ptr, ':')) != NULL )
        *ptr++ = 0;
      else
	goto done;

      pw.pw_dir = ptr;

      if ( ptr[0] && ptr[1] && (ptr = strchr(ptr + 2, ':')) != NULL )
        *ptr++ = 0;   /* skip drive: */

      found = 1;

      sprintf(line, "%s:%s:%d:%d:%s:%s:\n", pw.pw_name, crypted,
	      pw.pw_uid, pw.pw_gid, pw.pw_gecos, pw.pw_dir);
    }

  done:
    fputs(line, newpasswd);
  }

  fclose(passwd);
  fclose(newpasswd);

  if ( !found )
  {
    unlink(new);
    return -1;
  }

  unlink(bak);
  if (rename(old, bak))
    return -1;
  if (rename(new, old))
    return -1;

  return 0;
}
