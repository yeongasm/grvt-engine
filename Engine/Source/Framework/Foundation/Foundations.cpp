#include "GrvtPch.h"
#include "Framework/Foundation/Foundations.h"


namespace Grvt
{

	ObjHandle::ObjHandle() : Id(0), Target(0) {}


	ObjHandle::~ObjHandle() {}


	ObjHandle::ObjHandle(ObjHandle&& Rhs)
	{
		*this = Gfl::Move(Rhs);
	}


	ObjHandle& ObjHandle::operator= (ObjHandle&& Rhs)
	{
		_ASSERTE(this != &Rhs);

		if (this != &Rhs)
		{
			Id = Rhs.Id;
			Target = Rhs.Target;

			new (&Rhs) ObjHandle();
		}

		return *this;
	}


	namespace BaseAPI
	{

		void GrCreateVertexArray(ObjHandle& Handle)
		{
			glGenVertexArrays(1, &Handle.Id);
		}


		bool GrBindVertexArray(ObjHandle& Handle)
		{
			if (!Handle.Id)
				return false;

			glBindVertexArray(Handle.Id);

			return true;
		}


		void GrUnbindVertexArray()
		{
			glBindVertexArray(0);
		}


		void GrDeleteVertexArray(ObjHandle& Handle)
		{
			glDeleteVertexArrays(1, &Handle.Id);
		}


		void GrCreateBufferObject(ObjHandle& Handle, uint32 Target)
		{
			Handle.Target = Target;
			glGenBuffers(1, &Handle.Id);
		}


		bool GrBindBufferObject(ObjHandle& Handle)
		{
			if (!Handle.Id || !Handle.Target)
				return false;

			glBindBuffer(Handle.Target, Handle.Id);

			return true;
		}


		void GrUnbindBufferObject(ObjHandle& Handle)
		{
			glBindBuffer(Handle.Target, 0);
		}


		void GrDeleteBufferObject(ObjHandle& Handle)
		{
			glDeleteBuffers(1, &Handle.Id);
		}


		void GrCreateShader(ObjHandle& Handle, uint32 Type)
		{
			Handle.Target = Type;
			Handle.Id = glCreateShader(Handle.Target);
		}


		void GrDeleteShader(ObjHandle& Handle)
		{
			glDeleteShader(Handle.Id);
		}


		void GrCreateShaderProgram(ObjHandle& Handle)
		{
			Handle.Id = glCreateProgram();
		}


		void GrBindShaderProgram(ObjHandle& Handle)
		{
			glUseProgram(Handle.Id);
		}


		void GrUnbindShaderProgram(ObjHandle& Handle)
		{
			glUseProgram(0);
		}


		void GrDeleteShaderProgram(ObjHandle& Handle)
		{
			glDeleteProgram(Handle.Id);
		}



		void GrCreateTexture(ObjHandle& Handle, uint32 Target)
		{
			Handle.Target = Target;
			glGenTextures(1, &Handle.Id);
		}


		bool GrBindTexture(ObjHandle& Handle)
		{
			if (!Handle.Id || !Handle.Target)
				return false;

			glBindTexture(Handle.Target, Handle.Id);

			return true;
		}


		void GrUnbindTexture(ObjHandle& Handle)
		{
			glBindTexture(Handle.Target, 0);
		}


		void GrDeleteTexture(ObjHandle& Handle)
		{
			glDeleteTextures(1, &Handle.Id);
		}


		void GrCreateFramebuffer(ObjHandle& Handle)
		{
			Handle.Target = GL_FRAMEBUFFER;
			glGenFramebuffers(1, &Handle.Id);
		}


		bool GrBindFramebuffer(ObjHandle& Handle)
		{
			if (!Handle.Id || !Handle.Target)
				return false;

			glBindFramebuffer(Handle.Target, Handle.Id);

			return true;
		}


		void GrUnbindFramebuffer(ObjHandle& Handle)
		{
			glBindFramebuffer(Handle.Target, Handle.Id);
		}


		void GrDeleteFramebuffer(ObjHandle& Handle)
		{
			glDeleteFramebuffers(1, &Handle.Id);
		}


		void GrCreateRenderbuffer(ObjHandle& Handle)
		{
			Handle.Target = GL_RENDERBUFFER;
			glGenRenderbuffers(1, &Handle.Id);
		}


		bool GrBindRenderbuffer(ObjHandle& Handle)
		{
			if (!Handle.Id || !Handle.Target)
				return false;

			glBindRenderbuffer(Handle.Target, Handle.Id);

			return true;
		}


		void GrUnbindRenderbuffer(ObjHandle& Handle)
		{
			glBindRenderbuffer(Handle.Target, 0);
		}


		void GrDeleteRenderbuffer(ObjHandle& Handle)
		{
			glDeleteRenderbuffers(1, &Handle.Id);
		}

	}

}