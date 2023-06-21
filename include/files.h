#pragma once
#include "memory.h"
#include <stdbool.h>

bool file_exists(const char *filename);

int save_mem(memory mem);

int load_mem(memory* mem);