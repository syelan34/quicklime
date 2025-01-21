# Shaders

Ok writing down my thoughts on how to do this

Remove shader subclasses
Instead, set unif with script
Shader file becomes a binary file which is read by shader main class, includes all the settings in shader file
make shader metadata file from shader file

struct for shader would look something like this

```
{
tevsetting[5]
alpha test
alpha test func
...
}
```

Have both default (to access engine state) uniforms and user-defined ones

Examples of default uniforms:
0-3		QL_MATRIX_MVP	Current model * view * projection matrix.
4-7 	QL_MATRIX_MV	Current model * view matrix.
8-11	QL_MATRIX_V	Current view matrix.
12-15	QL_MATRIX_P	Current projection matrix.
16-19	QL_MATRIX_VP	Current view * projection matrix.
20-23	ql_ObjectToWorld	Current model matrix.
24-27	ql_WorldToObject    Inverse of current model matrix
28 \_Time	float4	Time since level load (t, sin(t), cos(t), dt), use to animate things inside the shaders.
29 \_ScreenParams	float4	x is the width of the camera’s target texture in pixels
, y is the height of the camera’s target texture in pixels, z is 1.0 + 1.0/width and w is 1.0 + 1.0/height.

set by camera:
matrices
\_ScreenParams

set by skeleton renderer:


set in main loop:
\_Time

Object drawing order priority
1. shader
2. queue
3. depth

.crsh file:
Contains GPU commands for the shader file

commands:
- texenvs (fully constant)
- alpha test (placeholder value)
- stencil op (placeholder value)
- face culling (placeholder value)
- depth test (placeholder value)


.mat file:
- textures