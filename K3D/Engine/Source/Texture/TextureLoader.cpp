#include "TextureLoader.h"
#include "TextureManager.h"
#include "WICTextureLoader12.h"
#include "DDSTextureLoader12.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Resource/ShaderResource.h"

K3D::TextureLoader::TextureLoader()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
}


K3D::TextureLoader::~TextureLoader()
{
	CoUninitialize();
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
			LoadTexture(commandList, commandQueue, resource, path);

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
			LoadTexture(commandList, commandQueue, resource, path);

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
				LoadTexture(commandList, commandQueue, resource, paths[i]);
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

HRESULT K3D::TextureLoader::LoadUpdateSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path)
{
	//list->ResetCommandList();
	D3D12_SUBRESOURCE_DATA& subresource = subResource;

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
	auto result = DirectX::LoadWICTextureFromFile(Framework::GetDevice().GetDevice().Get(), TextureTempPath.c_str(), resource.lock()->GetResource().GetAddressOf(), decodedData, subresource);
	if (FAILED(result))
	{
		return E_FAIL;
	}
	//resource.lock()->SetName(path + "_TempResource");
	D3D12_RESOURCE_DESC uploadDesc = {
		D3D12_RESOURCE_DIMENSION_BUFFER,
		0,
		GetRequiredIntermediateSize(resource.lock()->GetResource().Get(), 0, 1),
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
	UpdateSubresources(list->GetCommandList().Get(), resource.lock()->GetResource().Get(), uploadHeap.GetResource().Get(), static_cast<UINT>(0), static_cast<UINT>(0), static_cast<UINT>(1), &subresource);

	list->SetResourceBarrie(resource.lock()->GetResource().Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
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

HRESULT K3D::TextureLoader::LoadWriteToSubResource(std::shared_ptr<CommandList> list, CommandQueue * commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResoruce)
{
	D3D12_RESOURCE_DESC desc = *resource.lock()->GetResourceDesc();
	Resource destRes;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	int count = resource.lock()->GetResource().Reset();
	destRes.Create(heapProp, D3D12_HEAP_FLAG_NONE, desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);


	D3D12_BOX box = {};
	box.left = 0;
	box.right = static_cast<UINT>(desc.Width);
	box.top = 0;
	box.bottom = desc.Height;
	box.front = 0;
	box.back = 1;

	destRes.GetResource()->WriteToSubresource(0, &box, subResoruce.pData, box.right * 4, box.bottom * 4);
	destRes.ResourceTransition(list, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	resource.lock()->Discard();
	resource.lock()->GetResource = destRes;
	list->CloseCommandList();
	ID3D12CommandList* command_lists[] = { list->GetCommandList().Get() };

	// closeしてから呼ぼうね
	commandQueue->GetQueue()->ExecuteCommandLists(1, command_lists); // 実行するコマンドリストの配列をコマンドキューへ送信

	commandQueue->Wait();
	list->ResetAllocator();
	list->ResetCommandList();
}

HRESULT K3D::TextureLoader::LoadWICFile(DirectX::TexMetadata & metaData, DirectX::ScratchImage & scratchImage, std::string & path)
{
	return DirectX::LoadFromWICFile(Util::StringToWString(path).data(), 0, &metaData, scratchImage);
}

HRESULT K3D::TextureLoader::LoadDDSFile(DirectX::TexMetadata & metaData, DirectX::ScratchImage & scratchImage, std::string & path)
{
	return DirectX::LoadFromDDSFile(Util::StringToWString(path).data(), 0, &metaData, scratchImage);
}

HRESULT K3D::TextureLoader::LoadTGAFile(DirectX::TexMetadata & metaData, DirectX::ScratchImage & scratchImage, std::string & path)
{
	return DirectX::LoadFromTGAFile(Util::StringToWString(path).data(), &metaData, scratchImage);
}

bool K3D::TextureLoader::IsUseGamma(DXGI_FORMAT format)
{
	return(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB == format || DXGI_FORMAT_BC1_UNORM_SRGB == format ||
		DXGI_FORMAT_BC2_UNORM_SRGB == format || DXGI_FORMAT_BC3_UNORM_SRGB == format ||
		DXGI_FORMAT_B8G8R8A8_UNORM_SRGB == format || DXGI_FORMAT_B8G8R8X8_UNORM_SRGB == format ||
		DXGI_FORMAT_BC7_UNORM_SRGB == format);
}

HRESULT K3D::TextureLoader::LoadTexture(std::shared_ptr<CommandList> commandList, CommandQueue * commandQueue, std::weak_ptr<ShaderResource> resource, std::string path)
{

	auto& device = Framework::GetDevice().GetDevice();

	DirectX::TexMetadata metaData = {};
	DirectX::ScratchImage scratchImage = {};
	D3D12_SUBRESOURCE_DATA subResource = {};
	auto ext = Util::ExtensionExtruction(path);
	HRESULT hr = {};
	if (ext == "dds")
	{
		hr = this->LoadDDSFile(metaData, scratchImage, path);

	}
	else if (ext == "tga")
	{
		hr = this->LoadTGAFile(metaData, scratchImage, path);

	}
	else
	{
		hr = this->LoadWICFile(metaData, scratchImage, path);
	}


	if (hr != S_OK)
	{
		//Asign nullTex

	}
	else
	{

		subResource.pData = scratchImage.GetPixels();
		subResource.RowPitch = scratchImage.GetImages()->rowPitch;
		subResource.SlicePitch = scratchImage.GetImages()->slicePitch;

	}

	LoadUpdateSubResource(Framework::GetCommandList(), &Framework::GetCommandQueue(), resource, subResource, path);


	return E_NOTIMPL;
}
