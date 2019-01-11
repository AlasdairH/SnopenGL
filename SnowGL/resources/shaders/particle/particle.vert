#version 430 core

    in vec3 in_Value;
    out vec3 out_Value;

    void main()
    {
        out_Value = in_Value * 2;
    }