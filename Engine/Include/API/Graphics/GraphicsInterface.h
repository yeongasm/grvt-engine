#pragma once

#ifndef GRAVITY_API_GRAPHICS_INTERFACE
#define GRAVITY_API_GRAPHICS_INTERFACE

#include "Minimal.h"
#include "API/Graphics/GraphicsDriver.h"

#include "Framework/Model.h"
#include "Framework/Shader.h"
#include "Framework/Texture.h"
#include "Framework/Material.h"

namespace Grvt
{

	class GrvtMesh;
	class GrvtTexture;
	class GrvtShader;

	namespace Interface
	{
		template <class ResourceType, class BuildDataType>
		struct ResourcePacket
		{
			ResourceType*	ResourcePtr = nullptr;
			BuildDataType	BuildData;
		};

		struct DeletePacket
		{
			GfxHandle	Handle;
			HandleType	Type = HandleType::Handle_None;

			DeletePacket() {}
			~DeletePacket() {}

			DeletePacket(const DeletePacket& Rhs)				= delete;
			DeletePacket& operator= (const DeletePacket& Rhs)	= delete;

			DeletePacket(DeletePacket&& Rhs);
			DeletePacket& operator= (DeletePacket&& Rhs);
		};

		struct MeshPacket : public ResourcePacket<GrvtMesh, Driver::MeshBuildData>
		{
			// Temporary placement for interleaved vertex data.
			Gfl::Array<float32> Interleaved;
		};

		struct BufferPacket : public ResourcePacket<GfxHandle, Driver::BufferBuildData>
		{
			// Temporary placement for interleaved / non-interleaved buffer data.
			Gfl::Array<float32> TempStore;
		};

		// TODO(Afiq):
		// Implement Material Packets. We won't have the uninitialise uniforms when materials are pushed into the interface.
		struct MaterialPacket
		{
			GrvtMaterial*	ResourcePtr = nullptr;
			GrvtShader*		ShaderPtr	= nullptr;
		};

		using TexturePacket = ResourcePacket<GrvtTexture, Driver::TextureBuildData>;
		using ShaderPacket	= ResourcePacket<GrvtShader, Driver::ShaderBuildData>;
		using FramePacket	= ResourcePacket<GfxHandle, Driver::FramebufferBuildData>;
	}

	class GraphicsInterface
	{
	private:

		GraphicsDriver* Driver;

		std::deque<Interface::MeshPacket>		MeshQueue;
		std::deque<Interface::TexturePacket>	TextureQueue;
		std::deque<Interface::ShaderPacket>		ShaderQueue;
		std::deque<Interface::MaterialPacket>	MaterialQueue;
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

		void QueueMeshForBuild			(GrvtMesh& Resource, const Driver::MeshBuildData& Data);
		void QueueTextureForBuild		(GrvtTexture& Resource, const Driver::TextureBuildData& Data);
		void QueueShaderForBuild		(GrvtShader& Resource, const Driver::ShaderBuildData& Data);
		void QueueFramebufferForBuild	(GfxHandle& Resource, const Driver::FramebufferBuildData& Data);
		void QueueBufferForBuild		(GfxHandle& Resource, const Driver::BufferBuildData& Data);

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
		void PackageMaterialForBuild	(GrvtMaterial& MaterialSrc);
	};

}

#endif // !GRAVITY_API_GRAPHICS_INTERFACE