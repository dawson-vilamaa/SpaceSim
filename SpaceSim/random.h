#pragma once
#ifndef RANDOM_H
#define RANDOM_H
#include <random>
#include <iostream>

int randomInt(int min, int max) {
	return min + rand() % ((max + 1) - min);
}

float randomFloat() {
	return ((float) rand() / RAND_MAX);
}

float randomFloat(float min, float max) {
	float random = ((float)rand()) / (float)RAND_MAX;
	return min + (random * (max - min));
}

float randomBoolean() {
	return rand() & 1;
}

#endif