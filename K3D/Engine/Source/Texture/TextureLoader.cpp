#include "TextureLoader.h"
#include "TextureManager.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Utility/WICTextureLoader12.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Resource/ShaderResource.h"

K3D::TextureLoader::TextureLoader()
{

}


K3D::TextureLoader::~TextureLoader()
{

}


HRESULT K3D::TextureLoader::LoadUpdateSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, std::string path)
{
	//list->ResetCommandList();
	D3D12_SUBRESOURCE_DATA subresource;

	std::wstring TextureTempPath = {};
	FILE* fp = nullptr;

	if (path.c_str() != '\0') {
		TextureTempPath = Util::StringToWString(path);
		TextureTempPath.pop_back();
	}
	else {
		return E_FAIL;
	}

	std::unique_ptr<uint8_t[]> decodedData{};
	//テクスチャのロード処理
	//Microsoft::WRL::ComPtr<ID3D12Resource> textureUploadHeap;
	ShaderResource uploadHeap;
	auto result = DirectX::LoadWICTextureFromFile(Framework::GetDevice().GetDevice().Get(), TextureTempPath.c_str(), resource.lock()->GetAddressOf(), decodedData, subresource);
	if (FAILED(result))
	{
		return E_FAIL;
	}
	//resource.lock()->SetName(path + "_TempResource");
	D3D12_RESOURCE_DESC uploadDesc = {
		D3D12_RESOURCE_DIMENSION_BUFFER,
		0,
		GetRequiredIntermediateSize(resource.lock()->GetResource(), 0, 1),
		1,
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
	{ 1, 0 },
	D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
	D3D12_RESOURCE_FLAG_NONE
	};

	D3D12_HEAP_PROPERTIES props = {
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		1,
		1
	};
	result = uploadHeap.Create(props, D3D12_HEAP_FLAG_NONE, uploadDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);


	if (FAILED(result))
	{
		return E_FAIL;
	}
	uploadHeap.SetName(path + "_UploadHeap");
	UpdateSubresources(list->GetCommandList().Get(), resource.lock()->GetResource(), uploadHeap.GetResource(), static_cast<UINT>(0), static_cast<UINT>(0), static_cast<UINT>(1), &subresource);

	list->SetResourceBarrie(resource.lock()->GetResource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	resource.lock()->SetResourceState(D3D12_RESOURCE_STATE_GENERIC_READ);
	resource.lock()->SetName(path);
	list->CloseCommandList();
	ID3D12CommandList* command_lists[] = { list->GetCommandList().Get() };

	// closeしてから呼ぼうね
	commandQueue->GetQueue()->ExecuteCommandLists(1, command_lists); // 実行するコマンドリストの配列をコマンドキューへ送信します
																						   // Managerと違って1をイベント値として扱う
	commandQueue->Wait();

	list->ResetAllocator();
	list->ResetCommandList();
	uploadHeap.Discard();
	return S_OK;
}

void K3D::TextureLoader::LoadSpriteTexture(std::shared_ptr<CommandList> commandList, CommandQueue * commandQueue, std::string path)
{
	auto& managerRef = TextureManager::GetInstance();

	if (managerRef._modelTextureResourceMap.find(path) == managerRef._modelTextureResourceMap.end()) {

		std::shared_ptr<ShaderResource> resource = std::make_shared<ShaderResource>();
		std::shared_ptr<DescriptorHeap> descriptorHeap = std::make_shared<DescriptorHeap>();
		D3D12_DESCRIPTOR_HEAP_DESC desc;
		desc.NumDescriptors = 1;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NodeMask = 0;

		descriptorHeap->Create(&desc);
		//中間リソースの生成とか
		FILE* fp = nullptr;
		if (path.c_str() != '\0') {

			fopen_s(&fp, path.c_str(), "rb");
		}

		if (fp == NULL)
		{
			return;
		}
		else if (fp != NULL)
		{
			LoadUpdateSubResource(commandList, commandQueue, resource, path);

			//file Close
			fclose(fp);

			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = resource->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			// テクスチャとデスクリプターヒープとを紐付ける
			auto cpuHandle = descriptorHeap->GetCPUHandle(0);
			resource->CreateView(srvDesc, cpuHandle);

			//managerRef._DescriptorHeapMap[path] = descriptorHeap;
			descriptorHeap.reset();

			managerRef._modelTextureResourceMap[path] = resource;
		}
		resource.reset();
	}
	else {
		//もしすでに画像がロード済みならば無視

	}
	return;
}

void K3D::TextureLoader::LoadSpriteTexture(std::shared_ptr<CommandList> commandList, CommandQueue * commandQueue, std::string path, D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
	auto& managerRef = TextureManager::GetInstance();

	if (managerRef._modelTextureResourceMap.find(path) == managerRef._modelTextureResourceMap.end()) {

		std::shared_ptr<ShaderResource> resource = std::make_shared<ShaderResource>();

		//中間リソースの生成とか
		FILE* fp = nullptr;
		if (path.c_str() != '\0') {

			fopen_s(&fp, path.c_str(), "rb");
		}

		if (fp == NULL)
		{
			return;
		}
		else if (fp != NULL)
		{
			LoadUpdateSubResource(commandList, commandQueue, resource, path);

			//file Close
			fclose(fp);

			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = resource->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			// テクスチャとデスクリプターヒープとを紐付ける
			resource->CreateView(srvDesc, handle);
			managerRef._modelTextureResourceMap[path] = resource;
		}
		resource.reset();
	}
	else {
		//もしすでに画像がロード済みならば

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = managerRef._modelTextureResourceMap[path]->GetResourceDesc()->Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.PlaneSlice = 0;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		// テクスチャとデスクリプターヒープとを紐付ける
		managerRef._modelTextureResourceMap[path]->CreateView(srvDesc, handle);
	}
	return;
}

void K3D::TextureLoader::LoadModelTexture(std::shared_ptr<CommandList> commandList, CommandQueue * commandQueue, std::string madelName, std::vector<std::string>& paths)
{

	//仕様変更に伴い、この関数ではデスクリプタの生成を行わない

	if (paths.size() <= 0)
	{
		return;
	}

	auto& managerRef = TextureManager::GetInstance();

	std::shared_ptr<ShaderResource> resource;


	for (UINT i = 0; i < paths.size(); i++) {
		if (managerRef._modelTextureResourceMap.find(paths[i]) == managerRef._modelTextureResourceMap.end()) {

			//中間リソースの生成とか
			FILE* fp = nullptr;

			if (paths[i].c_str() != '\0') {
				fopen_s(&fp, paths[i].c_str(), "rb");
			}

			if (fp == NULL)
			{
				D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
				srvDesc.Format = managerRef._modelTextureResourceMap[managerRef._nullTextureName]->GetResourceDesc()->Format;
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels = 1;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.PlaneSlice = 0;
				srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
				srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

			}
			else if (fp != NULL)
			{
				resource = std::make_shared<ShaderResource>();
				LoadUpdateSubResource(commandList, commandQueue, resource, paths[i]);
				//file Close
				fclose(fp);

				D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
				srvDesc.Format = resource->GetResourceDesc()->Format;
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels = 1;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.PlaneSlice = 0;
				srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
				srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

				resource->SetName(paths[i]);

				managerRef._modelTextureResourceMap[paths[i]] = resource;
			}
		}
		else {
		}

		resource.reset();//所有権の放棄

	}

}

void K3D::TextureLoader::LoadModelTexture(std::shared_ptr<CommandList> commandList, CommandQueue * commandQueue, DescriptorHeap & heap, unsigned int heapStartIndex, std::string modelName, std::vector<std::string>& paths)
{
	if (paths.size() <= 0)
	{
		return;
	}

	auto& managerRef = TextureManager::GetInstance();

	for (unsigned int i = 0; i < paths.size(); i++) {
		if (managerRef._modelTextureResourceMap.find(paths[i]) == managerRef._modelTextureResourceMap.end()) {
			assert(true);
		}
		else {
			//もしテクスチャリソースがあったなら

			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = managerRef._modelTextureResourceMap[paths[i]]->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			auto cpuHandle = heap.GetCPUHandle(heapStartIndex + i);
			managerRef._modelTextureResourceMap[paths[i]]->CreateView(srvDesc, cpuHandle);
		}
	}
}

