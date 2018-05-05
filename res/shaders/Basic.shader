#shader vertex
#version 330 core 

layout(location = 0) in vec4 position;	// location is 0 because our attrib is at index 0

void main()
{
	gl_Position = position;
}

#shader fragment
#version 330 core 

layout(location = 0) out vec4 color;	// location is 0 because our attrib is at index 0

void main()
{
	color = vec4(0.2, 0.3, 0.8, 1.0);
}
