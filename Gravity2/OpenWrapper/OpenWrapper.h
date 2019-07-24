#pragma once


#ifndef GRAVITY_OPENGL_WRAPPER
#define GRAVITY_OPENGL_WRAPPER


/**
* If GLAD is not included in the project's pre-compiled header, comment the macro and specify the proper directory to GLAD in cpp file.
*/


using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;

using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;


enum BufferTarget : uint32 {
	EMPTY_BUFFER_OBJECT		= 0,
	VERTEX_BUFFER_OBJECT	= 0x8892,
	INDEX_BUFFER_OBJECT		= 0x8893,
	UNIFORM_BUFFER_OBJECT	= 0x8A11
};


struct BufferObject {
	uint32			Id;
	BufferTarget	Target;

	BufferObject();
	~BufferObject();

	BufferObject(const BufferObject &Rhs)				= delete;
	BufferObject& operator= (const BufferObject &Rhs)	= delete;

	BufferObject(BufferObject &&Rhs);
	BufferObject& operator= (BufferObject &&Rhs);

	operator uint32();

	bool Alloc();
	bool Release();
	void Bind();
	void UnBind();
};


struct VtxArrObject {
	uint32 Id;
	uint32 ChildBuffers;

	VtxArrObject();
	~VtxArrObject();

	VtxArrObject(const VtxArrObject &Rhs)				= delete;
	VtxArrObject& operator= (const VtxArrObject &Rhs)	= delete;

	VtxArrObject(VtxArrObject &&Rhs);
	VtxArrObject& operator= (VtxArrObject &&Rhs);

	operator uint32();

	void AddChild(const BufferObject &Buffer);
};


/**
* Mesh Vertex Attribute Pointers.
*
* Freedom is given to assign the index to any data in the mesh.
* However, for consistency and the perk of using the same layout in various shaders, it is recommended to specify the following index for the following data:
*
* Index: 0 - Mesh vertex positions.
* Index: 1 - Mesh normals.
* Index: 2 - Mesh UV coordinates.
* Index: 3 - Mesh tangents.
* Index: 4 - Mesh bitangents.
*
* Size represent the number of components per generic vertex attribute.
* Stride needs to be manually provided. Automating this would reduce flexibility to generate Meshes for different type of objects. Eg: Models & Terrains.
* Pointer to data needs to be manually provided.
*/
struct VertexPointer {
	uint32	Index;
	int32	Size;
	int32	Stride;
	uint32	Pointer;
};


struct MeshConfig {
	Array<float>			Data;
	Array<VertexPointer>	VertexPointer;
	Mesh					*Mesh;
};







#endif // !GRAVITY_OPENGL_WRAPPER