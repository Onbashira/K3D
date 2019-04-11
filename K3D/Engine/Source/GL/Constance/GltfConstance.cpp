#include "GltfConstance.h"
#include <d3d12.h>
#include "DirectXManagers/Device/D3D12DeviceManager.h"
#include "DirectXManagers/constance/ConstantManager.h"
#include "Ziometori.h"


GltfConstance::GltfConstance() {

}
GltfConstance::~GltfConstance() {

}
void GltfConstance::CbuffCreate(std::shared_ptr<D3D12DeviceManager>& device,const int maxmeshnum) {
	//定数用ヒープの量計算
	int matcountnum = 1;
	//for (int meshidx = 0; meshidx < maxmeshnum; meshidx++) {
	//	//matcountnum += FBXLoader::GetMaterialTable()[meshidx].size();
	//}

	std::shared_ptr<ConstantManager> cm(new ConstantManager());
	cm->CreateConstantBufferAndView(device, sizeof(DR_MAT), matcountnum);
	_mataddress = nullptr;
	cm->ConstantbufferMap(&_mataddress);
	_cbm = cm;
}

void GltfConstance::CbuffCreate_Bone(std::shared_ptr<D3D12DeviceManager>& device, const int maxmeshnum) {
	//定数用ヒープの量計算
	int matcountnum = 1;
	std::shared_ptr<ConstantManager> cm(new ConstantManager());
	cm->CreateConstantBufferAndView(device, sizeof(TESTBONE), matcountnum);
	_mataddress_bone = nullptr;
	cm->ConstantbufferMap(&_mataddress_bone);
	_cbm = cm;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& GltfConstance::GetDescHeap() {
	return _cbm->GetDescHeap();
}
DR_MAT*& GltfConstance::GetAddress() {
	return _mataddress;
}
TESTBONE*& GltfConstance::GetAddress_Bone() {
	return _mataddress_bone;
}