
#include <stdio.h>
#include "dprstrings.h"

int main(int argc, char **argv)
{
  char **aa = NULL;
  int ii;
  
  aa = strarrayapp(aa, "abc"); for(ii=0 ; aa[ii] != NULL ; ii++) { printf("%s\n", aa[ii]); } printf("\n");
  aa = strarrayapp(aa, "def"); for(ii=0 ; aa[ii] != NULL ; ii++) { printf("%s\n", aa[ii]); } printf("\n");
  aa = strarrayapp(aa, "ghi"); for(ii=0 ; aa[ii] != NULL ; ii++) { printf("%s\n", aa[ii]); } printf("\n");
  aa = strarrayapp(aa, "jkl"); for(ii=0 ; aa[ii] != NULL ; ii++) { printf("%s\n", aa[ii]); } printf("\n");
  aa = strarrayapp(aa, "mno"); for(ii=0 ; aa[ii] != NULL ; ii++) { printf("%s\n", aa[ii]); } printf("\n");
  aa = strarrayapp(aa, "pqr"); for(ii=0 ; aa[ii] != NULL ; ii++) { printf("%s\n", aa[ii]); } printf("\n");
  aa = strarrayapp(aa, "stu"); for(ii=0 ; aa[ii] != NULL ; ii++) { printf("%s\n", aa[ii]); } printf("\n");

  free_null_terminated_array(&aa);

  return 0;
}
