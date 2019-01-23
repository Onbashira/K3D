#include "TextureLoader.h"
#include "TextureManager.h"
#include "WICTextureLoader12.h"
#include "DDSTextureLoader12.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Resource/ShaderResource.h"
#include "Engine/Source/Texture/TextureObject.h"



K3D::TextureLoader::TextureLoader()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
}


K3D::TextureLoader::~TextureLoader()
{
	CoUninitialize();
}

void K3D::TextureLoader::LoadModelTexture(std::shared_ptr<CommandList> commandList, CommandQueue * commandQueue, std::string madelName, std::vector<std::string>& paths)
{

	if (paths.size() <= 0)
	{
		return;
	}

	auto& managerRef = TextureManager::GetInstance();

	std::shared_ptr<ShaderResource> resource;


	for (UINT i = 0; i < paths.size(); i++) {
		if (managerRef._textureResourceMap.GetMap().find(paths[i]) == managerRef._textureResourceMap.GetMap().end()) {

			//中間リソースの生成とか
			FILE* fp = nullptr;

			//ファイルが存在するかどうかの確認のため
			if (paths[i].c_str() != '\0') {
				fopen_s(&fp, paths[i].c_str(), "rb");
			}

			if (fp == NULL)
			{
				std::wstring text = Util::StringToWString(paths[i]) + L"にあるファイルが見つかりません";
				MessageBox(Framework::GetWindow().GetWindowHandle(), text.c_str(), L"テクスチャ読み込みエラー", MB_OK);

			}
			else if (fp != NULL)
			{
				resource = std::make_shared<ShaderResource>();
				LoadTexture(commandList, commandQueue, resource, paths[i]);
				//file Close
				fclose(fp);
				resource->SetName(paths[i]);
				managerRef._textureResourceMap.GetMap()[paths[i]] = resource;
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
	std::shared_ptr<ShaderResource> resource;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	for (unsigned int i = 0; i < paths.size(); i++) {
		if (managerRef._textureResourceMap.GetMap().find(paths[i]) == managerRef._textureResourceMap.GetMap().end()) {
			//もしテクスチャリソースが読み込まれてなければ
			//ロードしてからビュー作成
						//中間リソースの生成とか
			FILE* fp = nullptr;

			//ファイルが存在するかどうかの確認のため
			if (paths[i].c_str() != '\0') {
				fopen_s(&fp, paths[i].c_str(), "rb");
			}

			if (fp == NULL)
			{
				std::wstring text = Util::StringToWString(paths[i]) + L"にあるファイルが見つかりません";
				MessageBox(Framework::GetWindow().GetWindowHandle(), text.c_str(), L"テクスチャ読み込みエラー", MB_OK);

			}
			else if (fp != NULL)
			{
				resource = std::make_shared<ShaderResource>();
				LoadTexture(commandList, commandQueue, resource, paths[i]);
				//file Close
				fclose(fp);
				resource->SetName(paths[i]);
				managerRef._textureResourceMap.GetMap()[paths[i]] = resource;
			}

			srvDesc.Format = managerRef._textureResourceMap.GetMap()[paths[i]]->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			auto cpuHandle = heap.GetCPUHandle(heapStartIndex + i);
			managerRef._textureResourceMap.GetMap()[paths[i]]->CreateView(srvDesc, cpuHandle);

		}
		else {
			//もしテクスチャリソースがあったなら

			srvDesc.Format = managerRef._textureResourceMap.GetMap()[paths[i]]->GetResourceDesc()->Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.PlaneSlice = 0;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0F;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			auto cpuHandle = heap.GetCPUHandle(heapStartIndex + i);
			managerRef._textureResourceMap.GetMap()[paths[i]]->CreateView(srvDesc, cpuHandle);
		}

		resource.reset();
	}
}

std::shared_ptr<K3D::TextureObject> K3D::TextureLoader::LoadTexture(std::shared_ptr<CommandList> commandList, CommandQueue * commandQueue, std::string texturePath)
{
	return std::shared_ptr<TextureObject>();
}

std::shared_ptr<K3D::TextureObject> K3D::TextureLoader::LoadTexture(std::shared_ptr<D3D12Device>& device, std::string texturePath)
{
	return std::shared_ptr<TextureObject>();
}

std::shared_ptr<K3D::TextureObject> K3D::TextureLoader::LoadTexture(std::string texturePath)
{
	return std::shared_ptr<TextureObject>();
}

HRESULT K3D::TextureLoader::LoadUpdateSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path)
{
	D3D12_SUBRESOURCE_DATA& subresource = subResource;

	//テクスチャのロード処理
	Resource uploadHeap;

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
	auto result = uploadHeap.Create(props, D3D12_HEAP_FLAG_NONE, uploadDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);


	if (FAILED(result))
	{
		return E_FAIL;
	}
	uploadHeap.SetName(path + "_UploadHeap");

	UpdateSubresources(list->GetCommandList().Get(), resource.lock()->GetResource().Get(), uploadHeap.GetResource().Get(), static_cast<UINT>(0), static_cast<UINT>(0), static_cast<UINT>(1), &subresource);

	resource.lock()->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | 
										      D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	resource.lock()->SetName(path);
	list->CloseCommandList();
	ID3D12CommandList* command_lists[] = { list->GetCommandList().Get() };
	commandQueue->GetQueue()->ExecuteCommandLists(1, command_lists);
	commandQueue->Wait();

	list->ResetAllocator();
	list->ResetCommandList();
	uploadHeap.Discard();
	return S_OK;
}

HRESULT K3D::TextureLoader::LoadWriteToSubResource(std::shared_ptr<CommandList> list, CommandQueue * commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResoruce, std::string path)
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

	auto ret = destRes.GetResource()->WriteToSubresource(0, &box, subResoruce.pData, box.right * 4, box.bottom * 4);
	CHECK_RESULT(ret);
	destRes.ResourceTransition(list, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	resource.lock()->Discard();
	resource.lock()->GetResource() = destRes.GetResource();
	resource.lock()->SetName(path);
	list->CloseCommandList();
	ID3D12CommandList* command_lists[] = { list->GetCommandList().Get() };

	// closeしてから呼ぼうね
	commandQueue->GetQueue()->ExecuteCommandLists(1, command_lists); // 実行するコマンドリストの配列をコマンドキューへ送信

	commandQueue->Wait();
	list->ResetAllocator();
	list->ResetCommandList();
	return ret;
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

	auto& device = Framework::GetDevice()->GetDevice();

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
		if (IsUseGamma(metaData.format)) {

		}
	}

	hr = LoadUpdateSubResource(Framework::GetCommandList(), &Framework::GetCommandQueue(), resource, subResource, path);
	
	//セット

	return hr;
}
