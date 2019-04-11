#pragma once


#include <memory>
#include <string>
#include <DirectXMath.h>
#include <vector>

class GltfLoader;
class GltfLoader2;
class VertexBufferManager;
class D3D12DeviceManager;
class ComandManager;
class RootSignatureManager;
class WhiteTextureManager;
enum TEXTURE_REGISTER;
class GltfConstance;
struct DR_MAT;
class FenceManager;

struct GLTF_DRAW {
	std::shared_ptr<D3D12DeviceManager> device;
	std::shared_ptr<ComandManager> comand;
	std::shared_ptr<RootSignatureManager> rootsignature;

	GLTF_DRAW() {}
	GLTF_DRAW(
		std::shared_ptr<D3D12DeviceManager>& d,
		std::shared_ptr<ComandManager>& c,
		std::shared_ptr<RootSignatureManager>& r
	) :
		device(d),
		comand(c),
		rootsignature(r)
	{}
};


class GltfManager {
private:
	void DeferredRender(GLTF_DRAW& fbxdraw, TEXTURE_REGISTER texture_register, std::shared_ptr<WhiteTextureManager>& whitetexM);
	void UploadConstant(std::shared_ptr<ComandManager>& comand, DR_MAT* address);
	void CreateBoneInit(std::shared_ptr<ComandManager>& comand);
public:
	GltfManager();
	virtual ~GltfManager();



	
	/// <summary>
	/// モデルをロードする
	/// </summary>
	/// <param name="device"></param>
	/// <param name="filepath">モデルファイルまでの絶対パス</param>
	void Load(
		std::shared_ptr< D3D12DeviceManager>& device,
		std::shared_ptr<ComandManager>& comand,
		std::shared_ptr<FenceManager>& fence,
		const std::string& filepath
	);
	/// <summary>
	/// ディファードで描画
	/// </summary>
	/// <param name="gltfdraw"></param>
	/// <param name="texture_register"></param>
	/// <param name="whitetexM"></param>
	void DrawModelDR(GLTF_DRAW& gltfdraw, TEXTURE_REGISTER texture_register, std::shared_ptr<WhiteTextureManager>& whitetexM);

	void Scale(std::shared_ptr<ComandManager>& comand, const float x, const float y, const float z);
	void RotateX(float radian_x);
	void RotateY(float radian_y);
	void RotateZ(float radian_z);
	
private:
	std::shared_ptr<GltfLoader> _gltf;
	std::shared_ptr<VertexBufferManager> _vbm;
	std::shared_ptr< GltfConstance> _gltfcm;
	std::shared_ptr< GltfConstance> _gltfbone;


	DirectX::XMMATRIX _scale_backup;//拡縮値入れ
	//回転行列用
	DirectX::XMMATRIX _roll;
	DirectX::XMMATRIX _pitch;
	DirectX::XMMATRIX _heading;
};