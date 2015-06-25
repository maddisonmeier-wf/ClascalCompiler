/* usrdef.c
 *
 * Implements the userdefined datatypes hash table.
 *
 */

#include "usrdef.h"
#include "rulefuncs.h"


char * usrdef_new_name()
{
  char *n;

  n = (char *) malloc(MAX_NEW_CHAR_SIZE);
  strcpy(n, "__must_create_a_UNIQUE_anonymous_datatype_here_");

  return  n;
}


/* ------------------------------------------------------------
 * Initializes the usrdef
 * Adds into it the primitive data types
 * ------------------------------------------------------------
 */
void usrdef_init()
{

}



/* ------------------------------------------------------------
 * Prints the user defined data types table
 * ------------------------------------------------------------
 */
void usrdef_print()
{

}
