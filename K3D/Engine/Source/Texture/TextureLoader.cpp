#include "TextureLoader.h"
#include "TextureManager.h"
#include "WICTextureLoader12.h"
#include "DDSTextureLoader12.h"
#include "Engine/Source/CoreSystem/Framework.h"
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

std::shared_ptr<K3D::TextureObject> K3D::TextureLoader::LoadTextureResource(std::string filePath)
{
	DirectX::TexMetadata metaData = {};
	DirectX::ScratchImage scratchImage = {};
	auto hr = LoadFile(metaData, scratchImage, filePath);

	if (hr != S_OK) 
	{
		std::shared_ptr<K3D::TextureObject>();
	}
	D3D12_SUBRESOURCE_DATA subResource = {};

	subResource.pData = scratchImage.GetPixels();
	subResource.RowPitch = scratchImage.GetImages()->rowPitch;
	subResource.SlicePitch = scratchImage.GetImages()->slicePitch;
	
	TextureObjectDesc desc = {};

	desc.subResource = std::move(subResource);
	desc.metaData = std::move(metaData);
	desc.fileName = std::move(filePath);
	
	if (IsUseGamma(scratchImage.GetImages()->format)) {
		//汎用ガンマ
		desc.gamma = 2.2f;
	}
	std::shared_ptr<K3D::TextureObject> object = std::make_shared< K3D::TextureObject>();

	UpdateSubResource(Framework::GetInstance().GetCommandList(), &Framework::GetInstance().GetCommandQueue(), object->GetShaderResource(),desc.subResource,desc.fileName);

	return object;
}

std::shared_ptr<K3D::TextureObject> K3D::TextureLoader::LoadTextureResource(std::shared_ptr<CommandList>& commandList, CommandQueue * queue, std::string filePath)
{
	DirectX::TexMetadata metaData = {};
	DirectX::ScratchImage scratchImage = {};
	auto hr = LoadFile(metaData, scratchImage, filePath);

	if (hr != S_OK)
	{
		std::shared_ptr<K3D::TextureObject>();
	}
	D3D12_SUBRESOURCE_DATA subResource = {};

	subResource.pData = scratchImage.GetPixels();
	subResource.RowPitch = scratchImage.GetImages()->rowPitch;
	subResource.SlicePitch = scratchImage.GetImages()->slicePitch;

	TextureObjectDesc desc = {};

	desc.subResource = std::move(subResource);
	desc.metaData = std::move(metaData);
	desc.fileName = std::move(filePath);

	if (IsUseGamma(scratchImage.GetImages()->format)) {
		//汎用ガンマ
		desc.gamma = 2.2f;
	}
	std::shared_ptr<K3D::TextureObject> object = std::make_shared< K3D::TextureObject>();

	UpdateSubResource(commandList, queue, object->GetShaderResource(), desc.subResource, desc.fileName);

	return object;
}

bool K3D::TextureLoader::IsUseGamma(DXGI_FORMAT format)
 {
	 return(
		 DXGI_FORMAT_R8G8B8A8_UNORM_SRGB == format || DXGI_FORMAT_BC1_UNORM_SRGB == format ||
		 DXGI_FORMAT_BC2_UNORM_SRGB == format || DXGI_FORMAT_BC3_UNORM_SRGB == format ||
		 DXGI_FORMAT_B8G8R8A8_UNORM_SRGB == format || DXGI_FORMAT_B8G8R8X8_UNORM_SRGB == format ||
		 DXGI_FORMAT_BC7_UNORM_SRGB == format);
 }

HRESULT K3D::TextureLoader::UpdateSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path)
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

HRESULT K3D::TextureLoader::UpdateSubResource(std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA & subResource, std::string path)
{
	D3D12_SUBRESOURCE_DATA& subresource = subResource;

	auto list = Framework::GetInstance().GetCommandList();
	auto commandQueue = &Framework::GetInstance().GetCommandQueue();

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

HRESULT K3D::TextureLoader::WriteToSubResource(std::shared_ptr<CommandList> list, CommandQueue * commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path)
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

	auto ret = destRes.GetResource()->WriteToSubresource(0, &box, subResource.pData, box.right * 4, box.bottom * 4);
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

HRESULT K3D::TextureLoader::WriteToSubResource(std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA & subResource, std::string path)
{
	D3D12_RESOURCE_DESC desc = *resource.lock()->GetResourceDesc();
	auto list = Framework::GetInstance().GetCommandList();
	auto commandQueue = &Framework::GetInstance().GetCommandQueue();

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

	auto ret = destRes.GetResource()->WriteToSubresource(0, &box, subResource.pData, box.right * 4, box.bottom * 4);
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

HRESULT K3D::TextureLoader::LoadFile(DirectX::TexMetadata & metaData, DirectX::ScratchImage & scratchImage, std::string & path)
{

	auto ext = Util::ExtensionExtruction(path);
	HRESULT hr = {};
	if (ext == "dds")
	{
		hr = LoadDDSFile(metaData, scratchImage, path);

	}
	else if (ext == "tga")
	{
		hr = LoadTGAFile(metaData, scratchImage, path);

	}
	else
	{
		hr = LoadWICFile(metaData, scratchImage, path);
	}

	if (hr != S_OK) {
#ifdef _DEBUG
		std::wstring text = Util::StringToWString(path) + L"が見つかりません";
		MessageBox(Framework::GetInstance().GetWindow().GetWindowHandle(), text.c_str(), L"テクスチャ読み込みエラー", MB_OK);
#endif
	}
	return hr;
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


