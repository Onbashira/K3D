#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include <vector>
#include <string>
#include <DirectXTex.h>

namespace K3D {

	class ShaderResource;
	class CommandList;
	class CommandQueue;

	class TextureLoader
	{
	private:



	public:

		~TextureLoader();

		static TextureLoader & GetInstance() {
			static TextureLoader loader;
			return loader;

		}

		void LoadSpriteTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, std::string path);

		void LoadSpriteTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, std::string path, D3D12_CPU_DESCRIPTOR_HANDLE handle);

		void LoadModelTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, std::string modelName, std::vector<std::string>& paths);

		void LoadModelTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, DescriptorHeap& heap, unsigned int heapStartIndex, std::string modelName, std::vector<std::string>& paths);

	private:

		TextureLoader();

		HRESULT LoadUpdateSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource,D3D12_SUBRESOURCE_DATA& subResource, std::string path);

		HRESULT LoadWriteToSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource);

		HRESULT LoadWICFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		HRESULT LoadDDSFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		HRESULT LoadTGAFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		bool IsUseGamma(DXGI_FORMAT format);

		HRESULT LoadTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, std::string path);
		
	};
}