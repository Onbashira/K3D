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
	DirectX::XMFLOAT4 bone_weightidx;//�E�F�C�g�C���f�b�N�X(idx����wheight�l�ɂȂ�H)
	DirectX::XMFLOAT4 bone_jointidx;//�W���C���g�C���f�b�N�X
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

//TRS�s��@�����炭�I�u�W�F�N�g��ԏ�̏�񂩂�
//�Ȃ̂�Scene��̏������W�ɂ�����
class TransformData{
public:
	TransformData()
	{
		rotation[0] = rotation[1] = rotation[2] = rotation[3] = 0.0;
		translation[0] = translation[1] = translation[2] = 0.0;
		scale[0] = scale[1] = scale[2] = 1.0;
	}

	double rotation[4];
	double translation[3];//bone_head_pos�@�Ƃ��Ďg���邩�H
	double scale[3];
	double matrix[16];

	bool hasMatrix;
};

struct SceneNodeData {
	int tailIdx;//�q��index
	int parentIndex;//�e��index 
	std::vector<int> tailidxs;
	std::string Name;//object�̖��O
	DirectX::XMMATRIX inverseBindMatrices;//�t�o�C���h�s�� �e�W���C���g���Ƃɂ��̍s�񂪂���
	TransformData tdata;//���[���h�̃p�����[�^
};
struct SkinData {
	std::string name;
	int inverseBindMatricesAccessorId;
	int skeletonId;
	std::vector<int> jointIds;
};
struct Bone {
	DirectX::XMFLOAT3 headpos;//�{�[���̍�����(��)
	DirectX::XMFLOAT3 tailpos;//�{�[���̐�[(������)
	DirectX::XMMATRIX inverseBindMatrices;
};

struct BoneNode {
	std::vector<int> child;//�����̎q�ǂ�����
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
//�A�j���[�V����
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
	std::map<std::string, Animation_t_gltf> _animation;//�L�[�t���[���A�j���[�V����
};