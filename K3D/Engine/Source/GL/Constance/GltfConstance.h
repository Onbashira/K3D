#pragma once

#include <memory>
#include <DirectXMath.h>
#include <wrl.h>


class D3D12DeviceManager;
class ConstantManager;
class ComandManager;
class RootSignatureManager;
struct ID3D12DescriptorHeap;
struct DR_MAT;
struct TESTBONE;



class GltfConstance {
public:

	GltfConstance();
	virtual ~GltfConstance();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="device"></param>
	/// <param name="maxmeshnum">ƒƒbƒVƒ…Å‘å”</param>
	void CbuffCreate(std::shared_ptr<D3D12DeviceManager>& device, const int maxmeshnum);
	void CbuffCreate_Bone(std::shared_ptr<D3D12DeviceManager>& device, const int maxmeshnum);

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& GetDescHeap();
	DR_MAT*& GetAddress();
	TESTBONE*& GetAddress_Bone();
private:
	DR_MAT* _mataddress;
	TESTBONE* _mataddress_bone;
	std::shared_ptr<ConstantManager> _cbm;
};