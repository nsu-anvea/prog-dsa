#pragma once

#include "stack.h"

int isDigit(char x);
int isSign(char x);
int isUnaryMinus(char * ex, int i);
int isRightPriority(Stack * S, char x);