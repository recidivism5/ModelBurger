#pragma once

#include "linmath.h"

void ms_push();
void ms_pop();

void ms_identity();

void ms_scale(vec3 s);
void ms_translate(vec3 t);
void ms_rotate(vec3 a, float r);
void ms_ortho(float l, float r, float b, float t, float n, float f);
void ms_perspective(