#version 440 core

uniform mat4 P;
uniform float point_size;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout(location = 0) in float height[];

out VertexData {
  vec4 colour;
  vec2 tex_coord;
};


void main() {
  vec4 position = gl_in[0].gl_Position;

  // fire temperature
  float temp = clamp(2.0 / (2.0 / (height[0])), 0.0, 1.0);
  // scale between orange and black
  vec4 fire_colour = mix(vec4(0.88, .35, 0., 1.),vec4(0), temp);
  // and then between the previous mix and more black
  fire_colour = mix(fire_colour, vec4(0), height[0] - 1.0);
  // fade smoke out near top
  fire_colour.a = clamp((2.0 - height[0]) , 0.0, 1.0);

  //Billboarding
  // Expand point to 4 verts
  //point VA (-0.5, -0.5)
  vec2 va = position.xy + vec2(-0.5f, -0.5f) * point_size;
  gl_Position = P * vec4(va, position.zw);
  tex_coord = vec2(0.0f, 0.0f);
  colour = fire_colour;
  EmitVertex();

  // *********************************
  //point VB (0.5, -0.5), Tex (1,0)
  vec2 vb = position.xy + vec2(0.5f, -0.5f) * point_size;
  gl_Position = P * vec4(vb, position.zw);
  tex_coord = vec2(1.0f,0.0f);
  colour = fire_colour;
  EmitVertex();

  // point VC ((0.5, 0.5), Tex (1,1)
  vec2 vc = position.xy + vec2(0.5f, 0.5f) * point_size;
  gl_Position = P * vec4(vc, position.zw);
  tex_coord = vec2(1.0f,1.0f);
  colour = fire_colour;
  EmitVertex();

    // point VD (-0.5, 0.5), Tex (0,1)
  vec2 vd = position.xy + vec2(-0.5f, 0.5f) * point_size;
  gl_Position = P * vec4(vd, position.zw);
  tex_coord = vec2(0.0f,1.0f);
  colour = fire_colour;
  EmitVertex();
  // *********************************

  EndPrimitive();
}