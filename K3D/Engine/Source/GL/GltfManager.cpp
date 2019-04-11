#include "GltfManager.h"
#include "GltfLoader.h"
#include "DirectXManagers/Device/D3D12DeviceManager.h"
#include "DirectXManagers/vertex/VertexBufferManager.h"
#include "DirectXManagers/comand/ComandManager.h"
#include "DirectXManagers/index/IndexBufferManager.h"
#include "GltfLoader/Constance/GltfConstance.h"
#include "WhiteTexture/WhiteTextureManager.h"
#include "DirectXManagers/Fence/FenceManager.h"
#include "Ziometori.h"
#include <d3d12.h>
#include "constance.h"
#include "DirectXManagers/Texture/TextureManager.h"
GltfManager::GltfManager() {

}
GltfManager::~GltfManager() {

}

void GltfManager::Load(std::shared_ptr< D3D12DeviceManager>& device,std::shared_ptr<ComandManager>& comand,std::shared_ptr<FenceManager>& fence,const std::string& filepath) {
	//ロード
	std::shared_ptr<GltfLoader> gltf(new GltfLoader());
	_gltf = gltf;
	_gltf->Load(device, comand, fence, filepath);


	//定数の作成
	std::shared_ptr<GltfConstance> gltfc(new GltfConstance());
	_gltfcm = gltfc;
	_gltfcm->CbuffCreate(device,1);


	std::shared_ptr<GltfConstance> gltfbone(new GltfConstance());
	_gltfbone = gltfbone;
	_gltfbone->CbuffCreate_Bone(device, 1);

	//ボーンの初期化
	CreateBoneInit(comand);
}


void GltfManager::DrawModelDR(GLTF_DRAW& gltfdraw, TEXTURE_REGISTER texture_register, std::shared_ptr<WhiteTextureManager>& whitetexM){
	DeferredRender(gltfdraw, texture_register, whitetexM);
}
void GltfManager::DeferredRender(GLTF_DRAW& gltfdraw, TEXTURE_REGISTER texture_register, std::shared_ptr<WhiteTextureManager>& whitetexM) {
	CreateBoneInit(gltfdraw.comand);
	//行列パレットの作成
	auto cbffmat_bone = _gltfbone->GetAddress_Bone();
	for (int i = 0; i < _gltf->GetBoneMat().size(); i++) {
		cbffmat_bone->jointMatrix[i] = _gltf->GetBoneMat()[i];
	}
	cbffmat_bone = (TESTBONE*)(((char*)cbffmat_bone) + ((sizeof(TESTBONE) + 0xff) & ~0xff));
	auto handle_cbuff_bone = _gltfbone->GetDescHeap()->GetGPUDescriptorHandleForHeapStart();
	auto heap = _gltfbone->GetDescHeap().Get();
	gltfdraw.comand->GetGraphicsCommandList()->SetDescriptorHeaps(1, &heap);
	gltfdraw.comand->GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(ROOT_PARAM_CONSTANT_TESTBONE, handle_cbuff_bone);



	auto cbffmat = _gltfcm->GetAddress();
	auto handle_cbuff = _gltfcm->GetDescHeap()->GetGPUDescriptorHandleForHeapStart();
	unsigned int indexoffset = 0;
	for (int meshidx = 0; meshidx < _gltf->GetVertexData().size(); meshidx++) {

		gltfdraw.comand->GetGraphicsCommandList()->IASetVertexBuffers(0, 1, _gltf->GetMeshData()[meshidx]._vm->GetVertexBufferView());

		gltfdraw.comand->GetGraphicsCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//インデックスバッファーをセット
		gltfdraw.comand->GetGraphicsCommandList()->IASetIndexBuffer(_gltf->GetMeshData()[meshidx]._idxm->GetIndexBufferView());



		UploadConstant(gltfdraw.comand,cbffmat);
		cbffmat = (DR_MAT*)(((char*)cbffmat) + ((sizeof(DR_MAT) + 0xff) & ~0xff));
		auto heap = _gltfcm->GetDescHeap().Get();
		gltfdraw.comand->GetGraphicsCommandList()->SetDescriptorHeaps(1, &heap);
		gltfdraw.comand->GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(ROOT_PARAM_CONSTANT_DR, handle_cbuff);
		
		if (_gltf->GetTexture()[0] != nullptr) {//TODO: one tex
			_gltf->GetTexture()[0]->DrawImage(gltfdraw.comand,1, texture_register);
		}
		else {
			whitetexM->TexregisterDraw(gltfdraw.comand, texture_register);//白テクスチャへ
		}
		


		//Draw
		gltfdraw.comand->GetGraphicsCommandList()->DrawIndexedInstanced(_gltf->GetMeshData()[meshidx].fecenum, 1, indexoffset, 0, 0);
		indexoffset += _gltf->GetMeshData()[meshidx].fecenum;

	}

	


}


void GltfManager::CreateBoneInit(std::shared_ptr<ComandManager>& comand) {
	_gltf->BoneFrameUpdata(0);//ボーン0フレで初期化
	
}

void GltfManager::UploadConstant(std::shared_ptr<ComandManager>& comand, DR_MAT* address) {

	address->dr_diffuse_color = DirectX::XMFLOAT4(1,1,1,1);
	address->dr_ambient_color = DirectX::XMFLOAT4(1, 1, 1, 1);
	address->dr_specular_color = DirectX::XMFLOAT4(1, 1, 1, 1);
	address->dr_world = DirectX::XMMatrixIdentity();
	address->dr_scale = _scale_backup;
	address->dr_rotate = _roll * _pitch*_heading;



}



void GltfManager::Scale(std::shared_ptr<ComandManager>& comand, const float x, const float y, const float z) {
	_scale_backup = DirectX::XMMatrixScaling(x, y, z);
}
void GltfManager::RotateX(float radian_x) {
	_roll = DirectX::XMMatrixRotationX(radian_x);
}
void GltfManager::RotateY(float radian_y) {
	_pitch = DirectX::XMMatrixRotationY(radian_y);
}
void GltfManager::RotateZ(float radian_z) {
	_heading = DirectX::XMMatrixRotationZ(radian_z);
}