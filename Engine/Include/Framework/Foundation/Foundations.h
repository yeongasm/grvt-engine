#pragma once

#ifndef GRAVITY_GL_FOUNDATIONS
#define GRAVITY_GL_FOUNDATIONS

#include "Minimal.h"

namespace Grvt
{

	/**
	* [BASEAPI]
	* ObjHandle data structure.
	*
	* Low level API functionality.
	*
	* Represents all types of graphics object that can exist in the GPU.
	* Allocation and Deallocation should be done through helper functions.
	*/
	struct ObjHandle
	{
		uint32 Id;
		uint32 Target;

		ObjHandle();
		~ObjHandle();

		ObjHandle(const ObjHandle& Rhs)				= delete;
		ObjHandle& operator= (const ObjHandle& Rhs) = delete;

		ObjHandle(ObjHandle&& Rhs);
		ObjHandle& operator= (ObjHandle&& Rhs);
	};


	namespace BaseAPI
	{


		/**
		* [BASEAPI]
		* Creates a new Vertex Array ObjHandle.
		*/
		void GrCreateVertexArray(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Binds the specified ObjHandle into OpenGL's context for a Vertex Array object.
		* To unbind a Vertex Array object, call GrUnbindVertexArray().
		*/
		bool GrBindVertexArray(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Unbinds the previously specified ObjHandle from OpenGL's context for a Vertex Array object.
		* To bind a Vertex Array object, call GrBindVertexArray();
		*/
		void GrUnbindVertexArray();


		/**
		* [BASEAPI]
		* Deletes the specified Vertex Array ObjHandle from OpenGL.
		*/
		void GrDeleteVertexArray(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Creates a new Buffer ObjHandle.
		*/
		void GrCreateBufferObject(ObjHandle& Handle, uint32 Target);


		/**
		* [BASEAPI]
		* Binds the specified ObjHandle into OpenGL's context for an OpenGL Buffer object.
		* To unbind an OpenGL Buffer object, call GrUnbindBufferObject();
		*/
		bool GrBindBufferObject(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Unbinds the specified ObjHandle from OpenGL's context for an OpenGL Buffer object.
		* To bind a OpenGL Buffer object, call GrBindBufferObject();
		*/
		void GrUnbindBufferObject(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Deletes the specified Buffer ObjHandle from OpenGL.
		*/
		void GrDeleteBufferObject(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Creates a new Shader ObjHandle.
		*/
		void GrCreateShader(ObjHandle& Handle, uint32 Type);


		/**
		* [BASEAPI]
		* Deletes the specified Shader ObjHandle from OpenGL.
		*/
		void GrDeleteShader(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Creates a new Shader Program ObjHandle.
		*/
		void GrCreateShaderProgram(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Binds the specified ObjHandle into OpenGL's context for an OpenGL Shader object.
		* To unbind an OpenGL Shader object, call GrUnbindShader();
		*/
		void GrBindShaderProgram(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Unbinds the specified ObjHandle from OpenGL's context for an OpenGL Shader object.
		* To bind an OpenGL Shader object, call GrBindShader();
		*/
		void GrUnbindShaderProgram(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Deletes the specified Shader Program ObjHandle from OpenGL.
		*/
		void GrDeleteShaderProgram(ObjHandle& Handle);

		/**
		* [BASEAPI]
		* Creates a new Texture ObjHandle.
		*/
		void GrCreateTexture(ObjHandle& Handle, uint32 Target);


		/**
		* [BASEAPI]
		* Binds the specified ObjHandle into OpenGL's context for a Texture object.
		* To unbind the Texture object, call GrUnbindTexture();
		*/
		bool GrBindTexture(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Unbinds the specified ObjHandle from OpenGL's context for a Texture object.
		* To bind a Texture object, call GrBindTexture();
		*/
		void GrUnbindTexture(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Deletes the specified Texture ObjHandle from OpenGL.
		*/
		void GrDeleteTexture(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Creates a new Framebuffer ObjHandle.
		*/
		void GrCreateFramebuffer(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Binds the specified ObjHandle into OpenGL's context for a Framebuffer object.
		* To unbind a Framebuffer object, call GrUnbindFramebuffer();
		*/
		bool GrBindFramebuffer(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Unbinds the specified ObjHandle from OpenGL's context for a Framebuffer object.
		* To bind a Framebuffer object, call GrBindFramebuffer();
		*/
		void GrUnbindFramebuffer(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Deletes the specified Framebuffer ObjHandle from OpenGL.
		*/
		void GrDeleteFramebuffer(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Creates a new Renderbuffer ObjHandle.
		*/
		void GrCreateRenderbuffer(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Binds the specified ObjHandle into OpenGL's context for a Renderbuffer object.
		* To unbind a Renderbuffer object, call GrUnbindRenderbuffer();
		*/
		bool GrBindRenderbuffer(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Unbinds the specified ObjHandle from OpenGL's context for a Renderbuffer object.
		* To bind a Renderbuffer object, call GrBindRenderbuffer();
		*/
		void GrUnbindRenderbuffer(ObjHandle& Handle);


		/**
		* [BASEAPI]
		* Deletes the specified Renderbuffer ObjHandle from OpenGL.
		*/
		void GrDeleteRenderbuffer(ObjHandle& Handle);


		namespace Shader
		{
			/**
			* [BASEAPI][Shader]
			* Sends a boolean uniform to the shader.
			*/
			void GrShaderSetBool(uint32 Location, bool Value);

			/**
			* [BASEAPI][Shader]
			* Sends a integer uniform to the shader.
			*/
			void GrShaderSetInt(uint32 Location, int32 Value);
			
			/**
			* [BASEAPI][Shader]
			* Sends a float uniform to the shader.
			*/
			void GrShaderSetFloat(uint32 Location, float32 Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 2D float vector uniform to the shader.
			*/
			void GrShaderSetVec2Float(uint32 Location, float32* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 2D double vector uniform to the shader.
			*/
			void GrShaderSetVec2Double(uint32 Location, float64* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 3D float vector uniform to the shader.
			*/
			void GrShaderSetVec3Float(uint32 Location, float32* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 3D double vector uniform to the shader.
			*/
			void GrShaderSetVec3Double(uint32 Location, float64* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 4D float vector uniform to the shader.
			*/
			void GrShaderSetVec4Float(uint32 Location, float32* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 4D double vector uniform to the shader.
			*/
			void GrShaderSetVec4Double(uint32 Location, float64* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 2x2 float matrix uniform to the shader.
			*/
			void GrShaderSetMat2Float(uint32 Location, float32* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 2x2 double matrix uniform to the shader.
			*/
			void GrShaderSetMat2Double(uint32 Location, float64* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 3x3 float matrix uniform to the shader.
			*/
			void GrShaderSetMat3Float(uint32 Location, float32* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 3x3 double matrix uniform to the shader.
			*/
			void GrShaderSetMat3Double(uint32 Location, float64* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 4x4 float matrix uniform to the shader by name.
			*/
			void GrShaderSetMat4FloatN(uint32 Id, const char* Uniform, float32* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 4x4 float matrix uniform to the shader.
			*/
			void GrShaderSetMat4Float(uint32 Location, float32* Value);

			/**
			* [BASEAPI][Shader]
			* Sends a 4x4 double matrix uniform to the shader.
			*/
			void GrShaderSetMat4Double(uint32 Location, float64* Value);

			/**
			* [BASEAPI][Shader]
			* Gets a location of a uniform from a shader.
			*/
			uint32 GrGetUniformLocation(ObjHandle& Handle, const char* Uniform);

		}

	}

}


#endif // ! GRAVITY_GL_FOUNDATIONS