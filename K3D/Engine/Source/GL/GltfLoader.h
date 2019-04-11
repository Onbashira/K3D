#pragma once


#include <vector>
#include <string>
#include <DirectXMath.h>
#include <memory>
#include <map>


class D3D12DeviceManager;
struct Vertex;
class VertexBufferManager;
class IndexBufferManager;
struct TextureInfo;
class TextureManager;
class ComandManager;
class FenceManager;

struct GltfVertex {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT4 bone_weightidx;//ウェイトインデックス(idxだがwheight値になる？)
	DirectX::XMFLOAT4 bone_jointidx;//ジョイントインデックス
};

struct GltfMateial {
	DirectX::XMFLOAT4 baseColorFactor;
	float metallicFactor;
	float roughnessFactor;
};
struct Data {
	std::vector<std::vector<GltfVertex>> _vdata;
	std::vector<std::vector<int>> _idxdata;
};
struct GltfMesh {
	std::shared_ptr<VertexBufferManager> _vm;
	std::shared_ptr< IndexBufferManager> _idxm;
	GltfVertex* _vaddress;
	int* _indexAddress;
	int fecenum;
};

//TRS行列　おそらくオブジェクト空間上の情報かと
//なのでScene上の初期座標につかえる
class TransformData{
public:
	TransformData()
	{
		rotation[0] = rotation[1] = rotation[2] = rotation[3] = 0.0;
		translation[0] = translation[1] = translation[2] = 0.0;
		scale[0] = scale[1] = scale[2] = 1.0;
	}

	double rotation[4];
	double translation[3];//bone_head_pos　として使えるか？
	double scale[3];
	double matrix[16];

	bool hasMatrix;
};

struct SceneNodeData {
	int tailIdx;//子のindex
	int parentIndex;//親のindex 
	std::vector<int> tailidxs;
	std::string Name;//objectの名前
	DirectX::XMMATRIX inverseBindMatrices;//逆バインド行列 各ジョイントごとにこの行列がある
	TransformData tdata;//ワールドのパラメータ
};
struct SkinData {
	std::string name;
	int inverseBindMatricesAccessorId;
	int skeletonId;
	std::vector<int> jointIds;
};
struct Bone {
	DirectX::XMFLOAT3 headpos;//ボーンの根っこ(頭)
	DirectX::XMFLOAT3 tailpos;//ボーンの先端(しっぽ)
	DirectX::XMMATRIX inverseBindMatrices;
};

struct BoneNode {
	std::vector<int> child;//自分の子どもたち
};
struct AnimationChannelData {
	int samplerId;
	int target;
};
struct AnimationSamplerData {
	int inputAccessorId;
	int outputAccessorId;
	std::vector<std::vector<float>> inputdatas;
	std::vector <std::vector<float>> outputdatas;
};

struct AnimationData {
	std::string name;
	AnimationChannelData channel;
	AnimationSamplerData sampler;
};
//アニメーション
struct Pose_GLTF {
	Pose_GLTF(unsigned int f, const DirectX::XMVECTOR& q) :frameNo(f), quaternion(q) {}
	unsigned int frameNo;
	DirectX::XMVECTOR quaternion;
};
typedef std::vector<Pose_GLTF> Motion_t_gltf;
typedef std::pair<std::string, Motion_t_gltf> Animation_t_gltf;


class GltfLoader {
private:
	void InitVData(std::shared_ptr< D3D12DeviceManager>& device);
	void InitVIndexData(std::shared_ptr< D3D12DeviceManager>& device);
	void CreateBoneMatrix();
public:
	GltfLoader();
	virtual ~GltfLoader();


	void Load(
		std::shared_ptr<D3D12DeviceManager>& device,
		std::shared_ptr<ComandManager>& comand,
		std::shared_ptr<FenceManager>& fence,
		const std::string& filepath
	);

	void BoneFrameUpdata(unsigned int frameno);
	void Bone_Rotation(unsigned int bonemapindex);
	void Recursive_Bone(std::vector<BoneNode>& bonenodes, std::vector<Bone>& bonedraw, unsigned short boneidx, DirectX::XMMATRIX& bonem);
	
	std::vector<std::vector<GltfVertex>>& GetVertexData();
	std::vector<GltfMesh>& GetMeshData();
	std::vector<std::shared_ptr<TextureManager>>& GetTexture();
	std::vector<DirectX::XMMATRIX>& GetBoneMat();
private:
	std::vector<GltfMesh> _mesh;
	std::vector<std::shared_ptr<TextureManager>> _texm;
	Data data;







	std::vector<DirectX::XMMATRIX> bonemat;
	std::map<std::string, int> _boneList;
	std::vector<Bone> bonedraw_data;
	std::vector<BoneNode> _bonenodes;
	std::map<std::string, Animation_t_gltf> _animation;//キーフレームアニメーション
};