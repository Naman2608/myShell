#ifndef BACKEND_H
#define BACKEND_H

#include "node.h"

// Just some function prototypes

char *search_path(char *file);
int do_exec_cmd(int argc, char **argv);
int do_simple_command(struct node_s *node);

#endif
