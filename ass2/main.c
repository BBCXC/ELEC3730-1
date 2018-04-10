//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

// Assignment 2 WINNT main entry point

#include "Ass-02.h"

int main(void)
{
  // STEPIEN: Set the buffer sizes to 1 
  //
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  Ass_02_Main();
}
