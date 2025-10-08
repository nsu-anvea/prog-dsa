#pragma once
#include "stack.h"
#include "checks.h"

char * createExpression();

void append(char * postEntry, int * len, char x);
char * createPostEntry(char * ex, Stack * S, int * lenPostEntry);

double count(double b, double a, char action);
double calculate(char * postEntry, int len, Stack * S);
void execute(char * ex);