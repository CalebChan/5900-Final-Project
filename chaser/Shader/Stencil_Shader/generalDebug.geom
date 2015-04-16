#version 330

layout (triangles_adjacency) in;
layout (line_strip, max_vertices = 6) out;

in vec4 PosL[];
uniform vec3 gLightPos;

void EmitLine(int StartIndex, int EndIndex)
{
    gl_Position = PosL[StartIndex];
    EmitVertex();

    gl_Position = PosL[EndIndex];
    EmitVertex();

    EndPrimitive();
}



void main()
{
    vec3 e1 = PosL[2].xyz - PosL[0].xyz;
    vec3 e2 = PosL[4].xyz - PosL[0].xyz;
    vec3 e3 = PosL[1].xyz - PosL[0].xyz;
    vec3 e4 = PosL[3].xyz - PosL[2].xyz;
    vec3 e5 = PosL[4].xyz - PosL[2].xyz;
    vec3 e6 = PosL[5].xyz - PosL[0].xyz;

    vec3 Normal = cross(e1,e2);
    vec3 LightDir = gLightPos - PosL[0].xyz;

    if (dot(Normal, LightDir) > 0.00001) {

        Normal = cross(e3,e1);

        if (dot(Normal, LightDir) <= 0) {
            EmitLine(0, 2);
        }

        Normal = cross(e4,e5);
        LightDir = gLightPos - PosL[2].xyz;

        if (dot(Normal, LightDir) <=0) {
            EmitLine(2, 4);
        }

        Normal = cross(e2,e6);
        LightDir = gLightPos - PosL[4].xyz;

        if (dot(Normal, LightDir) <= 0) {
            EmitLine(4, 0);
        }
    }
}