#include "FSU.h"
#include "Constants.h"


void parse(char *buf, int *argc, char *argv[]);

struct action {
    const char* cmd;					// pointer to string
    int (FSU::*action)(char* name, char* size);
} table[12] = {
        { "root" , &FSU::do_root  },
        { "print", &FSU::do_print },
        { "chdir", &FSU::do_chdir },
        { "mkdir", &FSU::do_mkdir },
        { "rmdir", &FSU::do_rmdir },
        { "mvdir", &FSU::do_mvdir },
        { "mkfil", &FSU::do_mkfil },
        { "rmfil", &FSU::do_rmfil },
        { "mvfil", &FSU::do_mvfil },
        { "szfil", &FSU::do_szfil },
        { "exit" , &FSU::do_exit  },
        { NULL, NULL }				/* end marker, do not remove*/
    };



int main(int argc, char *argv[])
{
    FSU util = FSU();

  char in[LINESIZE];
  char *cmd, *fnm, *fsz;
  char dummy[] = "";

  int n;
  char *a[LINESIZE];

  while (fgets(in, LINESIZE, stdin) != NULL)
    {
      // commands are all like "cmd filename filesize\n" with whitespace between*/

      // parse in
      parse(in, &n, a);

      cmd = (n > 0) ? a[0] : dummy;
      fnm = (n > 1) ? a[1] : dummy;
      fsz = (n > 2) ? a[2] : dummy;
      /* if (debug) printf(":%s:%s:%s:\n", cmd, fnm, fsz); */

      if (n == 0) continue; // blank line

      int found = 0;
      for (struct action *ptr = table; ptr->cmd != NULL; ptr++) {
        if (strcmp(ptr->cmd, cmd) == 0)
          {
            found = 1;

            // Calls function here
            int ret = (util.*ptr->action)(fnm, fsz);
            if (ret == -1)
              { printf("  %s %s %s: failed\n", cmd, fnm, fsz); }
            break;
          }
      }
      if (!found)
        { printf("command not found: %s\n", cmd); }
    }

  return 0;
}

/*--------------------------------------------------------------------------------*/

// parse a command line, where buf came from fgets()

// Note - the trailing '\n' in buf is whitespace, and we need it as a delimiter.

void parse(char *buf, int *argc, char *argv[])
{
  char *delim;          // points to first space delimiter
  int count = 0;        // number of args

  char whsp[] = " \t\n\v\f\r";          // whitespace characters

  while (1)                             // build the argv list
    {
      buf += strspn(buf, whsp);         // skip leading whitespace
      delim = strpbrk(buf, whsp);       // next whitespace char or NULL
      if (delim == NULL)                // end of line
        { break; }
      argv[count++] = buf;              // start argv[i]
      *delim = '\0';                    // terminate argv[i]
      buf = delim + 1;                  // start argv[i+1]?
    }
  argv[count] = NULL;

  *argc = count;

  return;
}

/*--------------------------------------------------------------------------------*/


