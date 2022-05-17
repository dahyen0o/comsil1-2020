#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n, m; // width and height of maze
int width, height; // w and h of maze array
int **maze;

void Makemaze();
void Printmaze();