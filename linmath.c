#include "linmath.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <stdbool.h>

void vec3_copy(vec3 dst, vec3 src){
	memcpy(dst,src,sizeof(dst));
}

void vec3_add(vec3 a, vec3 b, vec3 dst){
	dst[0] = a[0] + b[0];
	dst[1] = a[1] + b[1];
	dst[2] = a[2] + b[2];
}

void vec3_sub(vec3 a, vec3 b, vec3 dst){
	dst[0] = a[0] - b[0];
	dst[1] = a[1] - b[1];
	dst[2] = a[2] - b[2];
}

void vec3_negate(vec3 v, vec3 dst){
	dst[0] = -v[0];
	dst[1] = -v[1];
	dst[2] = -v[2];
}

void vec3_scale(vec3 v, float s, vec3 dst){
	dst[0] = v[0] * s;
	dst[1] = v[1] * s;
	dst[2] = v[2] * s;
}

void vec3_mul(vec3 a, vec3 b, vec3 dst){
	dst[0] = a[0] * b[0];
	dst[1] = a[1] * b[1];
	dst[2] = a[2] * b[2];
}

void vec3_div(vec3 a, vec3 b, vec3 dst){
	dst[0] = a[0] / b[0];
	dst[1] = a[1] / b[1];
	dst[2] = a[2] / b[2];
}

float vec3_dot(vec3 a, vec3 b){
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

void vec3_cross(vec3 a, vec3 b, vec3 dst){
	dst[0] = a[1]*b[2] - a[2]*b[1];
	dst[1] = a[2]*b[0] - a[0]*b[2];
	dst[2] = a[0]*b[1] - a[1]*b[0];
}

float vec3_length(vec3 v){
	return sqrtf(vec3_dot(v,v));
}

float vec3_distance(vec3 a, vec3 b){
	vec3 d;
	vec3_sub(b,a,d);
	return vec3_length(d);
}

void vec3_midpoint(vec3 a, vec3 b, vec3 dst){
	vec3_sub(b,a,dst);
	vec3_scale(dst,0.5f,dst);
	vec3_add(a,dst,dst);
}

void vec3_normalize(vec3 v, vec3 dst){
	vec3_scale(v,1/vec3_length(v),dst);
}

void vec3_set_length(vec3 v, float l, vec3 dst){
	vec3_scale(v,l/vec3_length(v),dst);
}

void vec3_lerp(vec3 a, vec3 b, float t, vec3 dst){
	dst[0] = LERP(a[0],b[0],t);
	dst[1] = LERP(a[1],b[1],t);
	dst[2] = LERP(a[2],b[2],t);
}

void euler_wrap(vec3 e, vec3 dst){
	for (int i = 0; i < 3; i++){
		if (e[i] > 4.0f*M_PI){
			dst[i] = e[i] - 4.0f*M_PI;
		} else if (e[i] < -4.0f*M_PI){
			dst[i] = e[i] + 4.0f*M_PI;
		}
	}
}

void vec4_copy(vec4 dst, vec4 src){
	memcpy(dst,src,sizeof(dst));
}

void vec4_add(vec4 a, vec4 b, vec4 dst){
	dst[0] = a[0] + b[0];
	dst[1] = a[1] + b[1];
	dst[2] = a[2] + b[2];
	dst[3] = a[3] + b[3];
}

void vec4_sub(vec4 a, vec4 b, vec4 dst){
	dst[0] = a[0] - b[0];
	dst[1] = a[1] - b[1];
	dst[2] = a[2] - b[2];
	dst[3] = a[3] - b[3];
}

void vec4_negate(vec4 v, vec4 dst){
	dst[0] = -v[0];
	dst[1] = -v[1];
	dst[2] = -v[2];
	dst[3] = -v[3];
}

void vec4_scale(vec4 v, float s, vec4 dst){
	dst[0] = v[0] * s;
	dst[1] = v[1] * s;
	dst[2] = v[2] * s;
	dst[3] = v[3] * s;
}

void vec4_mul(vec4 a, vec4 b, vec4 dst){
	dst[0] = a[0] * b[0];
	dst[1] = a[1] * b[1];
	dst[2] = a[2] * b[2];
	dst[3] = a[3] * b[3];
}

void vec4_div(vec4 a, vec4 b, vec4 dst){
	dst[0] = a[0] / b[0];
	dst[1] = a[1] / b[1];
	dst[2] = a[2] / b[2];
	dst[3] = a[3] / b[3];
}

float vec4_dot(vec4 a, vec4 b){
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
}

float vec4_length(vec4 v){
	return sqrtf(vec4_dot(v,v));
}

void quat_identity(vec4 q){
	q[0] = 0;
	q[1] = 0;
	q[2] = 0;
	q[3] = 1;
}

void quat_slerp(vec4 a, vec4 b, float t, vec4 dst){
	float cosOmega = vec4_dot(a,b);
	bool flip = false;
	if (cosOmega < 0){
		flip = true;
		cosOmega = -cosOmega;
	}
	float s1,s2;
	if (cosOmega > (1 - 1e-6f)){
		s1 = 1 - t;
		s2 = flip ? -t : t;
	} else {
		float omega = acosf(cosOmega);
		float invSinOmega = 1 / sinf(omega);
		s1 = sinf((1-t)*omega)*invSinOmega;
		float sm = sinf(t*omega)*invSinOmega;
		s2 = flip ? -sm : sm;
	}
	for (int i = 0; i < 4; i++){
		dst[i] = s1*a[i] + s2*b[i];
	}
}

void quat_to_mat4(vec4 q, mat4 dst){
	float w, x, y, z,
		xx, yy, zz,
		xy, yz, xz,
		wx, wy, wz, norm, s;

	norm = vec4_length(q);
	s    = norm > 0.0f ? 2.0f / norm : 0.0f;

	x = q[0];
	y = q[1];
	z = q[2];
	w = q[3];

	xx = s * x * x;   xy = s * x * y;   wx = s * w * x;
	yy = s * y * y;   yz = s * y * z;   wy = s * w * y;
	zz = s * z * z;   xz = s * x * z;   wz = s * w * z;

	dst[0][0] = 1.0f - yy - zz;
	dst[1][1] = 1.0f - xx - zz;
	dst[2][2] = 1.0f - xx - yy;

	dst[0][1] = xy + wz;
	dst[1][2] = yz + wx;
	dst[2][0] = xz + wy;

	dst[1][0] = xy - wz;
	dst[2][1] = yz - wx;
	dst[0][2] = xz - wy;

	dst[0][3] = 0.0f;
	dst[1][3] = 0.0f;
	dst[2][3] = 0.0f;
	dst[3][0] = 0.0f;
	dst[3][1] = 0.0f;
	dst[3][2] = 0.0f;
	dst[3][3] = 1.0f;
}

void mat4_copy(mat4 dst, mat4 src){
	memcpy(dst,src,sizeof(dst));
}

void mat4_mul(mat4 a, mat4 b, mat4 dst){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			dst[i][j] = a[0][j]*b[i][0] + a[1][j]*b[i][1] + a[2][j]*b[i][2] + a[3][j]*b[i][3];
		}
	}
}

void mat4_mul_vec4(mat4 m, vec4 v, vec4 dst){
	for (int i = 0; i < 4; i++){
		dst[i] = m[0][i]*v[0] + m[1][i]*v[1] + m[2][i]*v[2] + m[3][i]*v[3];
	}
}

void mat4_identity(mat4 m){
	m[0][0] = 1;
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = 0;

	m[1][0] = 0;
	m[1][1] = 1;
	m[1][2] = 0;
	m[1][3] = 0;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 1;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

void mat4_scale_3(mat4 m, vec3 s){
	m[0][0] = s[0];
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = 0;

	m[1][0] = 0;
	m[1][1] = s[1];
	m[1][2] = 0;
	m[1][3] = 0;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = s[2];
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

void mat4_translate(mat4 m, vec3 t){
	m[0][0] = 1;
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = 0;

	m[1][0] = 0;
	m[1][1] = 1;
	m[1][2] = 0;
	m[1][3] = 0;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 1;
	m[2][3] = 0;

	m[3][0] = t[0];
	m[3][1] = t[1];
	m[3][2] = t[2];
	m[3][3] = 1;
}

void mat4_rotate(mat4 m, vec3 a, float r){
	vec3 axisn, v, vs;
	float c;

	c = cosf(r);

	vec3_normalize(a, axisn);
	vec3_scale(axisn, 1.0f - c, v);
	vec3_scale(axisn, sinf(r), vs);

	vec3_scale(axisn, v[0], m[0]);
	vec3_scale(axisn, v[1], m[1]);
	vec3_scale(axisn, v[2], m[2]);

	m[0][0] += c;       m[1][0] -= vs[2];   m[2][0] += vs[1];
	m[0][1] += vs[2];   m[1][1] += c;       m[2][1] -= vs[0];
	m[0][2] -= vs[1];   m[1][2] += vs[0];   m[2][2] += c;

	m[0][3] = m[1][3] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

void mat4_euler_yxz(mat4 m, vec3 e){
	float cx, cy, cz,
		sx, sy, sz, cycz, sysz, czsy, cysz;

	sx   = sinf(e[0]); cx = cosf(e[0]);
	sy   = sinf(e[1]); cy = cosf(e[1]);
	sz   = sinf(e[2]); cz = cosf(e[2]);

	cycz = cy * cz;
	sysz = sy * sz;
	czsy = cz * sy;
	cysz = cy * sz;

	m[0][0] =  cycz + sx * sysz;
	m[0][1] =  cx * sz;
	m[0][2] = -czsy + cysz * sx;
	m[1][0] = -cysz + czsy * sx;
	m[1][1] =  cx * cz;
	m[1][2] =  cycz * sx + sysz;
	m[2][0] =  cx * sy;
	m[2][1] = -sx;
	m[2][2] =  cx * cy;
	m[0][3] =  0.0f;
	m[1][3] =  0.0f;
	m[2][3] =  0.0f;
	m[3][0] =  0.0f;
	m[3][1] =  0.0f;
	m[3][2] =  0.0f;
	m[3][3] =  1.0f;
}

void mat4_rotate_x(mat4 m, float r){
	float c = cosf(r);
	float s = sinf(r);

	m[0][0] = 1;
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = 0;

	m[1][0] = 0;
	m[1][1] = c;
	m[1][2] = s;
	m[1][3] = 0;

	m[2][0] = 0;
	m[2][1] = -s;
	m[2][2] = c;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

void mat4_rotate_y(mat4 m, float r){
	float c = cosf(r);
	float s = sinf(r);

	m[0][0] = c;
	m[0][1] = 0;
	m[0][2] = -s;
	m[0][3] = 0;

	m[1][0] = 0;
	m[1][1] = 1;
	m[1][2] = 0;
	m[1][3] = 0;

	m[2][0] = s;
	m[2][1] = 0;
	m[2][2] = c;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

void mat4_rotate_z(mat4 m, float r){
	float c = cosf(r);
	float s = sinf(r);

	m[0][0] = c;
	m[0][1] = s;
	m[0][2] = 0;
	m[0][3] = 0;

	m[1][0] = -s;
	m[1][1] = c;
	m[1][2] = 0;
	m[1][3] = 0;

	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 1;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

void mat4_ortho_rh_no(mat4 m, float left, float right, float bottom, float top, float nearZ, float farZ){
	float rl, tb, fn;

	rl = 1.0f / (right - left);
	tb = 1.0f / (top - bottom);
	fn =-1.0f / (farZ - nearZ);

	m[0][0] = 2.0f * rl;
	m[1][1] = 2.0f * tb;
	m[2][2] = 2.0f * fn;
	m[3][0] = -(right + left) * rl;
	m[3][1] = -(top + bottom) * tb;
	m[3][2] = (farZ + nearZ) * fn;
	m[3][3] = 1.0f;

	m[0][1] = m[0][2] = m[0][3] = 
	m[1][0] = m[1][2] = m[1][3] = 
	m[2][0] = m[2][1] = m[2][3] = 0;
}

void mat4_ortho_lh_zo(mat4 m, float left, float right, float bottom, float top, float nearZ, float farZ){
	float rl, tb, fn;

	rl = 1.0f / (right - left);
	tb = 1.0f / (top - bottom);
	fn =-1.0f / (farZ - nearZ);

	m[0][0] = 2.0f * rl;
	m[1][1] = 2.0f * tb;
	m[2][2] = -fn;
	m[3][0] = -(right + left) * rl;
	m[3][1] = -(top + bottom) * tb;
	m[3][2] = nearZ * fn;
	m[3][3] = 1.0f;

	m[0][1] = m[0][2] = m[0][3] = 
	m[1][0] = m[1][2] = m[1][3] = 
	m[2][0] = m[2][1] = m[2][3] = 0;
}

void mat4_persp_rh_no(mat4 m, float fovy, float aspect, float nearZ, float farZ){
	float f, fn;

	f  = 1.0f / tanf(fovy * 0.5f);
	fn = 1.0f / (nearZ - farZ);

	m[0][0] = f / aspect;
	m[1][1] = f;
	m[2][2] = (nearZ + farZ) * fn;
	m[2][3] =-1.0f;
	m[3][2] = 2.0f * nearZ * farZ * fn;

	m[0][1] = m[0][2] = m[0][3] =
	m[1][0] = m[1][2] = m[1][3] =
	m[2][0] = m[2][1] =
	m[3][0] = m[3][1] = m[3][3] = 0;
}

void mat4_persp_lh_zo(mat4 m, float fovy, float aspect, float nearZ, float farZ){
	float f, fn;

	f  = 1.0f / tanf(fovy * 0.5f);
	fn = 1.0f / (nearZ - farZ);

	m[0][0] = f / aspect;
	m[1][1] = f;
	m[2][2] =-farZ * fn;
	m[2][3] = 1.0f;
	m[3][2] = nearZ * farZ * fn;

	m[0][1] = m[0][2] = m[0][3] =
	m[1][0] = m[1][2] = m[1][3] =
	m[2][0] = m[2][1] =
	m[3][0] = m[3][1] = m[3][3] = 0;
}