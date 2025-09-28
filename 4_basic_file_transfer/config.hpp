#ifndef CONFIG_H
#define CONFIG_H

#define PORT 8000					// Server PORT
#define BUFFER_SIZE 1024			// Buffer Size of messages
#define END_COMMAND	"end"			// End command to close communication
#define IN_FILENAME "input.pdf"		// Input file name
#define OUT_FILENAME "output.pdf"	// Output file name

// ANSI CODE
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE_BG "\033[44;97m"
#define HIDE_CURSOR "\033[?25l"
#define SHOW_CURSOR "\033[?25h"

#endif