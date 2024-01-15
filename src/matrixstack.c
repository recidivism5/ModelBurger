#include <matrixstack.h>
#include <error.h>

static mat4 stack[32];
static int index = 0;
static mat4 mat;

static void mul(){
	mat4_mul(stack[index],mat,stack[index]);
}

void push_matrix(){
	ASSERT(index < (sizeof(stack)/sizeof(*stack))-1);
	index++;
}

void pop_matrix(){
	ASSERT(index > 0);
	index--;
}

void load_identity(){
	mat4_identity(stack[index]);
}

void rotate(vec3 a, float r){
	mat4_rotate(mat,a,r);
	mul();
}

void translate(vec3 t){
	mat4_translate(mat,t);
	mul();
}

void ortho(float left, float right, float bottom, float top, float nearZ, float farZ){
	mat4_ortho_lh_zo(mat,left,right,bottom,top,nearZ,farZ);
	mul();
}

void persp(float fovy, float aspect, float nearZ, float farZ){
	mat4_persp_lh_zo(mat,fovy,aspect,nearZ,farZ);
	mul();
}