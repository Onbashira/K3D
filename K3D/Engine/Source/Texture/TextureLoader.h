#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include <vector>
#include <string>

namespace K3D {

	class ShaderResource;
	class CommandList;
	class CommandQueue;

	class TextureLoader
	{
	private:

		HRESULT LoadUpdateSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, std::string path);

		TextureLoader();

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

	};
}