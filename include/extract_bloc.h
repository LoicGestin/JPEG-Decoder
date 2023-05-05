#ifndef _EXTRACT_BLOC_H_
#define _EXTRACT_BLOC_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <decode_entete.h>

int*** extract_bloc(FILE* file, struct data *d);

#endif /* _EXTRACT_BLOC_H_ */