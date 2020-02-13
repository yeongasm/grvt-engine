#pragma once

#ifndef GRAVITY_API_GRAPHICS_INTERFACE
#define GRAVITY_API_GRAPHICS_INTERFACE

#include "Minimal.h"
#include "API/Graphics/GraphicsDriver.h"

namespace Grvt
{

	class GrvtMesh;
	class GrvtTexture;
	class GrvtShader;

	namespace Interface
	{
		template <class BuildDataType>
		struct ResourcePacket
		{
			GfxHandle*		Handle = nullptr;
			BuildDataType	BuildData;
		};

		struct DeletePacket
		{
		private:

			DeletePacket(const DeletePacket& Rhs) = delete;
			DeletePacket& operator= (const DeletePacket& Rhs) = delete;

		public:

			GfxHandle	Handle;
			HandleType	Type = HandleType::Handle_None;

			DeletePacket() {}
			~DeletePacket() {}
		};

		struct MeshPacket : public ResourcePacket<Driver::MeshBuildData>
		{
			// Temporary placement for interleaved vertex data.
			Gfl::Array<float32> Interleaved;
		};

		struct BufferPacket : public ResourcePacket<Driver::BufferBuildData>
		{
			// Temporary placement for interleaved / non-interleaved buffer data.
			Gfl::Array<float32> TempStore;
		};

		using TexturePacket = ResourcePacket<Driver::TextureBuildData>;
		using ShaderPacket	= ResourcePacket<Driver::ShaderBuildData>;
		using FramePacket	= ResourcePacket<Driver::FramebufferBuildData>;
	}

	class GraphicsInterface
	{
	private:

		GraphicsDriver* Driver;

		std::deque<Interface::MeshPacket>		MeshQueue;
		std::deque<Interface::TexturePacket>	TextureQueue;
		std::deque<Interface::ShaderPacket>		ShaderQueue;
		std::deque<Interface::FramePacket>		FrameQueue;
		std::deque<Interface::BufferPacket>		BufferQueue;
		std::deque<Interface::DeletePacket>		DeleteQueue;

		bool HasResource;

		GraphicsInterface(const GraphicsInterface& Rhs)				= delete;
		GraphicsInterface& operator= (const GraphicsInterface& Rhs) = delete;

		GraphicsInterface(GraphicsInterface&& Rhs)				= delete;
		GraphicsInterface& operator= (GraphicsInterface&& Rhs)	= delete;

	public:

		GraphicsInterface();
		~GraphicsInterface();

		void Init(GraphicsDriver* Driver);
		void Shutdown();

		void QueueMeshForBuild			(GfxHandle& Handle, const Driver::MeshBuildData& Data);
		void QueueTextureForBuild		(GfxHandle& Handle, const Driver::TextureBuildData& Data);
		void QueueShaderForBuild		(GfxHandle& Handle, const Driver::ShaderBuildData& Data);
		void QueueFramebufferForBuild	(GfxHandle& Handle, const Driver::FramebufferBuildData& Data);
		void QueueBufferForBuild		(GfxHandle& Handle, const Driver::BufferBuildData& Data);

		void QueueMeshPacketForBuild	(const Interface::MeshPacket& Packet);
		void QueueTexturePacketForBuild	(const Interface::TexturePacket& Packet);
		void QueueShaderPacketForBuild	(const Interface::ShaderPacket& Packet);
		void QueueFramePacketForBuild	(const Interface::FramePacket& Packet);
		void QueueBufferPacketForBuild	(const Interface::BufferPacket& Packet);
		void QueueHandleForDelete		(GfxHandle&& Handle, HandleType Type);

		void Tick();

		void PackageMeshForBuild		(GrvtMesh& MeshSrc);
		void PackageTextureForBuild		(GrvtTexture& TextureSrc);
		void PackageShaderForBuild		(GrvtShader& ShaderSrc);
	};

}

#endif // !GRAVITY_API_GRAPHICS_INTERFACE