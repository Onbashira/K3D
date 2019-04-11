#include "GltfLoader.h"
#include <boost/version.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "DirectXManagers\Device\D3D12DeviceManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>
#include <GLTFSDK/Deserialize.h>
#include <GLTFSDK/Serialize.h>
#include <GLTFSDK/GLTFResourceWriter.h>
#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/RapidJsonUtils.h>
#include "DirectXManagers/vertex/VertexBufferManager.h"
#include "DirectXManagers/index/IndexBufferManager.h"
#include "constance.h"
#include "Library/DirectX12Create/String.h"
#include "DirectXManagers/Texture/TextureManager.h"
#include "DirectXManagers/comand/ComandManager.h"
#include "DirectXManagers/Fence/FenceManager.h"
#include <algorithm>

using namespace boost::property_tree;
using namespace Microsoft::glTF;
using namespace std;
using namespace DirectX;


/*
まずtdataをメッシュに適用できるようにすること
*/


std::vector<SceneNodeData> sceneNodeObjTRS;//オブジェクト空間上に存在するmeshの情報
std::vector<std::vector<DirectX::XMMATRIX>> matss;//BindMatrix
std::vector<AnimationData> animdata;//全体アニメーション 配列の数はアニメーションの数
std::vector<SkinData> skindata;
//SkinData　TODO:
std::vector<int> join;//nodeに対応するindex　らしい
std::vector<DirectX::XMMATRIX> bindmatrix;


GltfLoader::GltfLoader() {
	std::cout << std::endl;
	std::cout << "boostバージョン:" << BOOST_VERSION << std::endl;
	std::cout << "boostライブラリバージョン:" << BOOST_LIB_VERSION << std::endl;
}
GltfLoader::~GltfLoader() {

}

/// <summary>
/// 任意のパスを相対パスにする
/// </summary>
/// <param name="filepath">ファイル名(そのファイルまでのパスではない)</param>
/// <param name="modelfilepath">モデルに使用したファイルパスまたはそこまでのパス</param>
/// <returns>相対パスを返す</returns>
const std::string relativeTexturePath(const std::string& filepath,const std::string& modelfilepath) {
	std::string workpath = filepath;
	auto lastpath = filepath.substr(0, filepath.find_last_of('\\') + 1);//最後に現れる指定文字までの場所
	workpath.erase(0, lastpath.size());//中身を編集

	//パス化する
	auto lastpathtex = modelfilepath.substr(0, modelfilepath.find_last_of('/') + 1);
	if (lastpathtex.size() < 1) {
		printf("指定ファイルパスが無効\n");
		throw(1);
	}
	workpath = lastpathtex + workpath;
	printf("LoadTexture: %s\n", workpath.c_str());
	return workpath;
}


void DeserializeIndex(int meshidx, const std::string& accessID, const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader, std::vector<std::vector<int>>& indexdatas);
std::vector<GltfVertex> DeserializeVertex(const Microsoft::glTF::MeshPrimitive& meshprim, const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader);
std::vector<float> LoadWheightidx(const Microsoft::glTF::MeshPrimitive& meshprim, const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader);
std::vector<unsigned short> LoadJointidx(const Microsoft::glTF::MeshPrimitive& meshprim, const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader);
void CreateNode(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader);
void LoadSceneNode(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader,const Node& sceneNode, int tailIdx, int parentIndex);
TransformData LoadTransform(const Node& mNode);
void CreateSkin(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader);
void CreateAnimation(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader);
void LoadAnimation(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader);
class StreamReader : public IStreamReader{
public:
	StreamReader(std::experimental::filesystem::path pathBase) : m_pathBase(std::move(pathBase)){}
	std::shared_ptr<std::istream> GetInputStream(const std::string& filename) const override
	{
		std::wstring p = m_pathBase.c_str();
		std::wstring n = _T("\\");
		p.append(n);
		p.append(StringtoWString(filename));
		auto stream = std::make_shared<std::ifstream>(p, std::ios_base::binary);
		if (!stream || !(*stream)){throw std::runtime_error("Unable to create a valid input stream for uri: " + filename);}
		return stream;
	}
private:
	std::experimental::filesystem::path m_pathBase;
};
//バイナリデータの抽出
void PrintResourceInfo(const Document& document, const GLBResourceReader& resourceReader)
{
	for (const auto& mesh : document.meshes.Elements())
	{
		std::cout << "Mesh: " << mesh.id << "\n";
		for (const auto& meshPrimitive : mesh.primitives)
		{
			std::string accessorId;
			if (meshPrimitive.TryGetAttributeAccessorId(ACCESSOR_POSITION, accessorId))//ここでIDというものを使ってデータを呼べる
			{
				const Accessor& accessor = document.accessors.Get(accessorId);
				const auto data = resourceReader.ReadBinaryData<float>(document, accessor);
				const auto dataByteLength = data.size() * sizeof(float);
				std::cout << "MeshPrimitive: " << dataByteLength << " bytes of position data\n";
			}
		}
		std::cout << "\n";
	}
}
//モデル情報の取得例
void PrintDocumentInfo(const Document& document)
{
	// Asset Info
	std::cout << "Asset Version:    " << document.asset.version << "\n";
	std::cout << "Asset MinVersion: " << document.asset.minVersion << "\n";
	std::cout << "Asset Generator:  " << document.asset.generator << "\n";
	std::cout << "Asset Copyright:  " << document.asset.copyright << "\n\n";

	// Scene Info
	std::cout << "Scene Count: " << document.scenes.Size() << "\n";

	if (document.scenes.Size() > 0U)
	{
		std::cout << "Default Scene Index: " << document.GetDefaultScene().id << "\n\n";
	}
	else
	{
		std::cout << "\n";
	}

	// Entity Info
	std::cout << "Node Count:     " << document.nodes.Size() << "\n";
	std::cout << "Camera Count:   " << document.cameras.Size() << "\n";
	std::cout << "Material Count: " << document.materials.Size() << "\n\n";

	// Mesh Info
	std::cout << "Mesh Count: " << document.meshes.Size() << "\n";
	std::cout << "Skin Count: " << document.skins.Size() << "\n\n";

	// Texture Info
	std::cout << "Image Count:   " << document.images.Size() << "\n";
	std::cout << "Texture Count: " << document.textures.Size() << "\n";
	std::cout << "Sampler Count: " << document.samplers.Size() << "\n\n";

	// Buffer Info
	std::cout << "Buffer Count:     " << document.buffers.Size() << "\n";
	std::cout << "BufferView Count: " << document.bufferViews.Size() << "\n";
	std::cout << "Accessor Count:   " << document.accessors.Size() << "\n\n";

	// Animation Info
	std::cout << "Animation Count: " << document.animations.Size() << "\n\n";

	for (const auto& extension : document.extensionsUsed)
	{
		std::cout << "Extension Used: " << extension << "\n";
	}

	if (!document.extensionsUsed.empty())
	{
		std::cout << "\n";
	}

	for (const auto& extension : document.extensionsRequired)
	{
		std::cout << "Extension Required: " << extension << "\n";
	}

	if (!document.extensionsRequired.empty())
	{
		std::cout << "\n";
	}
}


void GltfCreateStreamReaderAndDoc(std::unique_ptr<GLBResourceReader>& gltfrr, Document& doc, std::string& filepath) {
	//ファイルパスからリソース名を抽出
	std::string workpath = filepath;
	auto modelPath = filepath.substr(0, filepath.find_last_of('/') + 1);//パスの最後の'/'まで
	workpath.erase(0, modelPath.size());//文字削除
	//ストリームリーダーの作成
	std::experimental::filesystem::path path = workpath;
	std::experimental::filesystem::path path_s_2 = filepath;
	std::string manifest;
	std::experimental::filesystem::path pathFile = path;
	auto streamReader = std::make_unique<StreamReader>(path_s_2.parent_path());
	auto glbStream = streamReader->GetInputStream(pathFile.u8string()); // Pass a UTF-8 encoded filename to GetInputString
	auto glbResourceReader = std::make_unique<GLBResourceReader>(std::move(streamReader), std::move(glbStream));
	manifest = glbResourceReader->GetJson(); // Get the manifest from the JSON chunk
	gltfrr = std::move(glbResourceReader);
	if (!gltfrr)
	{
		throw std::runtime_error("Command line argument path filename extension must be .gltf or .glb");
	}
	//ドキュメントの作成
	try
	{
		doc = Deserialize(manifest);
	}
	catch (const GLTFException& ex)
	{
		std::stringstream ss;
		ss << "Microsoft::glTF::Deserialize failed: ";
		ss << ex.what();
		throw std::runtime_error(ss.str());
	}
	std::cout << "### glTF Info - " << pathFile << " ###\n\n";
	//モデル情報開示
	PrintDocumentInfo(doc);

}


//メッシュ情報を読み込む
void Deserialize(
	const Microsoft::glTF::Document& document,
	const Microsoft::glTF::GLBResourceReader& resourceReader,
	Data& d,
	std::vector<std::shared_ptr<TextureManager>>& texms,
	const std::string& modelfilepath,
	std::shared_ptr<D3D12DeviceManager>& device,
	std::shared_ptr<ComandManager>& comand,
	std::shared_ptr<FenceManager>& fence
    ) {
	std::vector<std::vector<int>> i_indexDatas;


	int meshindex = 0;
	int accessorID = 0;
	for (const auto& mesh : document.meshes.Elements())//Elementsを使ったメッシュの総数を数える方法はあってる
	{
		//メッシュ情報
		std::cout << "Mesh: " << mesh.id << "\n";
		std::cout << "Meshname: " << mesh.name << "\n";

		//頂点
		for (const auto& meshPrimitive : mesh.primitives)
		{	
			d._vdata.push_back(DeserializeVertex(meshPrimitive, document, resourceReader));
		}

		std::cout << "\n";
	}









	//index
	int meshmaxsize = d._vdata.size();//変なメッシュも入ってる可能性があるため全て読み込むため、多重にループをかける
	i_indexDatas.resize(meshmaxsize);
	for (const auto& mesh : document.meshes.Elements())
	{
		for (const auto& meshPrimitive : mesh.primitives)
		{
			DeserializeIndex(meshindex, meshPrimitive.indicesAccessorId, document, resourceReader, i_indexDatas);
			meshindex++;
		}
	}
	d._idxdata = i_indexDatas;

	//material TODO: テクスチャもここでするっぽい
	/*
	data.baseColourFactor[0] = mNode.metallicRoughness.baseColorFactor.r;
	data.baseColourFactor[1] = mNode.metallicRoughness.baseColorFactor.g;
	data.baseColourFactor[2] = mNode.metallicRoughness.baseColorFactor.b;
	data.baseColourFactor[3] = mNode.metallicRoughness.baseColorFactor.a;　で色はとれそう・・（DXViewr）

	*/
	//https://kotaeta.com/55023731 これを使えば行けるかもしれない
	//TODO:　jpegを読み込む必要がある。時間がかかるため今回は飛ばす
	std::vector< std::vector<uint8_t>> texdata;//画像バイナリデータ
	for (const auto& mat : document.materials.Elements())
	{
		//メッシュ情報
		std::cout << "materialid: " << mat.id << "\n";
		std::cout << "materialname(TexPath): " << mat.name << "\n";

		//テクスチャをロード
		std::shared_ptr<TextureManager> texm(new TextureManager());
		 auto texfilepath = relativeTexturePath(mat.name, modelfilepath);
		 texm->WICLoadTexture(device, comand, fence, texfilepath);
		 texms.push_back(texm);

		// TODO: glbに圧縮されている画像情報の読み込み 現時点でmaterialから外部読み込みにする(png jpegのローダーを作成する必要あり時間かかる)
		 //処理する方法としてメタデータはInitializeFromMemory を使用する(glTF-DXViewer-masterのImgUtils.hより)
		if (mat.metallicRoughness.baseColorTexture.textureId != "") {
			const int textureID = std::stoi(mat.metallicRoughness.baseColorTexture.textureId);
			std::vector<uint8_t> imgdata;
			const Texture& texture = document.textures[textureID];
			if (texture.imageId != "") {
				auto baseColorImageIndex = std::stoi(texture.imageId);
				auto img = document.images[baseColorImageIndex];
				imgdata = resourceReader.ReadBinaryData(document, img);
				texdata.push_back(imgdata);//pngそのままバイナリデータとして出力された・・・
				
			}
		}
	}



	//Node階層の作成
	CreateNode(document, resourceReader);
	//skinの作成
	CreateSkin(document, resourceReader);
	
	int stoped = 0;
	//Gltfまとめ
	//Vertex情報を取得 wheight　と Joint関係がよくわからんがNodeベースではないため、普通に入れるだけでいいのかな
	//ノード階層を作成する BoneNodeがその作業にあたる
	//skinでどのnodeがボーンになるかindexを取得する
	//Animationでinputのフレームによって遷移されるoutput情報をノード階層にぶち込んでメッシュを動かす用の行列を作成する
	//シェーダ側で用意した行列達とwheightとjointを足し合わせてメッシュが動く
	
}


void CreateSkin(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader) {
	
	for (const auto& skin : document.skins.Elements()) {
		
		SkinData skn;
		skn.name = skin.name;
		skn.inverseBindMatricesAccessorId = std::stoi(skin.inverseBindMatricesAccessorId.c_str());
		skn.skeletonId = std::stoi(skin.skeletonId.c_str());
		for (auto& inx : skin.jointIds) {
			skn.jointIds.push_back(std::stoi(inx));
		}
		skindata.push_back(skn);
	}	
	
}
void CreateAnimation(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader) {
	/*
	Acsesser全体の中にAnimationDataがある->Bynaryに続く
	Animationのなかに行列パレットが入っている？->Bindと関係している
	AcsesserIDが必要になるはずAnimation用AcsesserIDはどうやって取得する？
	Sampler input,output-> アクセサオブジェクトを参照
	Sampler Channel は とあるnodeのtdataを指すー＞ということは初期座標としているアレが「パレット行列か？」
	*/
	AnimationData animd = {};
	const Animation& anim = document.animations[0];//1つのアニメーションのみで
	animd.name = anim.name;
	const size_t channelsCou = anim.channels.Size();
	const size_t samplersCou = anim.samplers.Size();

	

	//AnimationSampler
	if (samplersCou > 0) {
		for (size_t i = 0; i < samplersCou; ++i) {
			const AnimationSampler& sampler = anim.samplers[i];
			std::vector<float> input;
			std::vector<float> output;
			if (sampler.inputAccessorId != "") {
				animd.sampler.inputAccessorId = std::stoi(sampler.inputAccessorId);
				const int accessorID = std::stoi(sampler.inputAccessorId);
				const Accessor& acce = document.accessors[accessorID];

				const int bufferViewID = std::stoi(acce.bufferViewId);
				input = resourceReader.ReadBinaryData<float>(document, document.bufferViews[bufferViewID]);
			}
			if (sampler.outputAccessorId != "") {
				animd.sampler.inputAccessorId = std::stoi(sampler.outputAccessorId);
				const int accessorID = std::stoi(sampler.outputAccessorId);
				const Accessor& acce = document.accessors[accessorID];
				const int bufferViewID = std::stoi(acce.bufferViewId);
				output = resourceReader.ReadBinaryData<float>(document, document.bufferViews[bufferViewID]);
			}
			animd.sampler.inputdatas.push_back(input);
			animd.sampler.outputdatas.push_back(output);
		}
	}

	std::vector<int> nodeids;//
	//Channels
	for (int i = 0; i < channelsCou; i++) {
		auto channels = anim.channels[i];
		if (channels.samplerId != "") {
			nodeids.push_back(std::stoi(channels.target.nodeId));
			animd.channel.samplerId = std::stoi(channels.samplerId);
		}	
	}


	animdata.push_back(animd);//n分のアニメーションの量

	

}


//Node階層の作成
void CreateNode(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader) {
	auto scene = document.GetDefaultScene();
    const int PARENTINDEX = -1;//子がいない場合-1
	for (auto node : scene.nodes)
    {
	
	auto sceneNode = document.nodes[node];




	string name = sceneNode.name.length() > 0 ? sceneNode.name : "node-" + sceneNode.id;
	cout << "Loading " << name.c_str() << "\n";//表示されるのはルートたち
	if (name == "Lamp" || name == "Camera") {
		//continue;//channelのtarget.nodeindexを使用しnodeを利用するかもしれないためindexの不揃いを考慮する
	}









	//シーンの階層を下って再帰し、シーンのnodeを作成する
	LoadSceneNode(document, resourceReader, sceneNode, stoi(sceneNode.id.c_str()), PARENTINDEX);//もとの

	//TODO:　おそらくツリー構造間違ってる
	//この構成だとどんどん子供を持っていることになる
	//とはいえchildを複数持つnodeはどう対処すべきか

	//LoadSceneNode(document, resourceReader, sceneNode, stoi(sceneNode.children[0].c_str()), PARENTINDEX);
    }	
	
}
void LoadSceneNode(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader, const Node& sceneNode, int tailIdx, int parentIndex) {
	SceneNodeData data = {};
	data.tailIdx = tailIdx;//子
	data.parentIndex = parentIndex;//親


	DirectX::XMMATRIX invbindmat = DirectX::XMMatrixIdentity();
	//skin情報からなんとかしてボーンを構築する
	if (sceneNode.skinId.length() > 0) {
		//joint配列の取得->対応するノード番号　らしい
		for (auto idx : document.skins[stoi(sceneNode.skinId.c_str())].jointIds) {
			join.push_back(stoi(idx.c_str()));
		}
		//逆バインド行列の取得 ノードごとにこの行列が存在する
		auto invbmat = document.skins[stoi(sceneNode.skinId.c_str())].inverseBindMatricesAccessorId;
		if (invbmat != "") {
			const int accessorID = std::stoi(invbmat);
			const Accessor& acce = document.accessors[accessorID];
			const int bufferViewID = std::stoi(acce.bufferViewId);
			std::vector<float> fData;
			fData = resourceReader.ReadBinaryData<float>(document, document.bufferViews[bufferViewID]);


			for (int i = 0; i < fData.size(); i += 16) {
				invbindmat.r[0].m128_f32[0] = fData[i + 0];
				invbindmat.r[0].m128_f32[1] = fData[i + 1];
				invbindmat.r[0].m128_f32[2] = fData[i + 2];
				invbindmat.r[0].m128_f32[3] = fData[i + 3];



				invbindmat.r[1].m128_f32[0] = fData[i + 4];
				invbindmat.r[1].m128_f32[1] = fData[i + 5];
				invbindmat.r[1].m128_f32[2] = fData[i + 6];
				invbindmat.r[1].m128_f32[3] = fData[i + 7];

				invbindmat.r[2].m128_f32[0] = fData[i + 8];
				invbindmat.r[2].m128_f32[1] = fData[i + 9];
				invbindmat.r[2].m128_f32[2] = fData[i + 10];
				invbindmat.r[2].m128_f32[3] = fData[i + 11];

				invbindmat.r[3].m128_f32[0] = fData[i + 12];
				invbindmat.r[3].m128_f32[1] = fData[i + 13];
				invbindmat.r[3].m128_f32[2] = fData[i + 14];
				invbindmat.r[3].m128_f32[3] = fData[i + 15];

			}

		}

	}




	//scene name
	string name = sceneNode.name.length() > 0 ? sceneNode.name : "node-" + sceneNode.id;
	data.Name = name.c_str();
	cout << "name: " << data.Name << "\n";

	data.inverseBindMatrices = invbindmat;//inversbindMatrix　Jointごとに取得する


	data.tdata = LoadTransform(sceneNode);//mesh以外TRS行列の取得
	sceneNodeObjTRS.push_back(data);
	for (auto child : sceneNode.children)//子供いるなら再帰
	{
		auto childNode = document.nodes[child];
		LoadSceneNode(document, resourceReader, childNode, stoi(childNode.id.c_str()), tailIdx);
	}

}


TransformData LoadTransform(const Node& mNode){
	TransformData tdata;

	//TRS行列の取得
	TransformationType type = mNode.GetTransformationType();
	tdata.hasMatrix = type == TRANSFORMATION_MATRIX;
	tdata.translation[0] = 1;
	tdata.translation[1] = 1;
	tdata.translation[2] = 1;


		if (tdata.hasMatrix)//生のままか
		{
			//tdata.matrix[0] = mNode.matrix.values[0];
			//tdata.matrix[1] = mNode.matrix.values[1];
			//tdata.matrix[2] = mNode.matrix.values[2];
			//tdata.matrix[3] = mNode.matrix.values[3];

			//tdata.matrix[4] = mNode.matrix.values[4];
			//tdata.matrix[5] = mNode.matrix.values[5];
			//tdata.matrix[6] = mNode.matrix.values[6];
			//tdata.matrix[7] = mNode.matrix.values[7];

			//tdata.matrix[8] = mNode.matrix.values[8];
			//tdata.matrix[9] = mNode.matrix.values[9];
			//tdata.matrix[10] = mNode.matrix.values[10];
			//tdata.matrix[11] = mNode.matrix.values[11];

			//tdata.matrix[12] = mNode.matrix.values[12];
			//tdata.matrix[13] = mNode.matrix.values[13];
			//tdata.matrix[14] = mNode.matrix.values[14];
			//tdata.matrix[15] = mNode.matrix.values[15];
		}
		else
		{
			tdata.rotation[0] = mNode.rotation.x;
			tdata.rotation[1] = mNode.rotation.y;
			tdata.rotation[2] = mNode.rotation.z;
			tdata.rotation[3] = mNode.rotation.w;

			tdata.translation[0] = mNode.translation.x;
			tdata.translation[1] = mNode.translation.y;
			tdata.translation[2] = mNode.translation.z;

			tdata.scale[0] = mNode.scale.x;
			tdata.scale[1] = mNode.scale.y;
			tdata.scale[2] = mNode.scale.z;
		}
	
	return tdata;
	
}
//vertex
std::vector<GltfVertex> DeserializeVertex(const Microsoft::glTF::MeshPrimitive& meshprim, const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader) {
	std::string accessorId;
	std::vector< DirectX::XMFLOAT3> pdata;
	std::vector< DirectX::XMFLOAT3> ndata;
	std::vector< DirectX::XMFLOAT2> uvdata;
	std::vector< DirectX::XMFLOAT2> uvdata1;
	std::vector< DirectX::XMFLOAT4> weightidx;
	std::vector< DirectX::XMFLOAT4> jointidx;
	//Postion
	if (meshprim.TryGetAttributeAccessorId(ACCESSOR_POSITION, accessorId))//ここでIDというものを使ってデータを呼べる
	{
		const Accessor& accessor = document.accessors.Get(accessorId);
		const auto data = resourceReader.ReadBinaryData<float>(document, accessor);
		const auto dataByteLength = data.size() * sizeof(float);
		for (int i = 0; i < data.size() - 2; i += 3) {
			DirectX::XMFLOAT3 pos = {};
			pos.x = data[i];
			pos.y = data[i + 1];
			pos.z = data[i + 2];
			//pos.w = data[i + 3];
			pdata.push_back(pos);
		}
		std::cout << "MeshPrimitive: " << dataByteLength << " bytes of position data\n";
	}
	//Normal
	if (meshprim.TryGetAttributeAccessorId(ACCESSOR_NORMAL, accessorId))//ここでIDというものを使ってデータを呼べる
	{
		const Accessor& accessor = document.accessors.Get(accessorId);
		const auto data = resourceReader.ReadBinaryData<float>(document, accessor);
		const auto dataByteLength = data.size() * sizeof(float);
		for (int i = 0; i < data.size() - 2; i += 3) {
			DirectX::XMFLOAT3 n = {};
			n.x = data[i];
			n.y = data[i + 1];
			n.z = data[i + 2];
			ndata.push_back(n);
		}

	}
	//uv0の取得:　よくわからないのでテスト的に引用
	if (meshprim.TryGetAttributeAccessorId(ACCESSOR_TEXCOORD_0, accessorId))//ここでIDというものを使ってデータを呼べる
	{
		const Accessor& accessor = document.accessors.Get(accessorId);
		const auto data = resourceReader.ReadBinaryData<float>(document, accessor);
		const int bufferViewID = std::stoi(accessor.bufferViewId);
		const BufferView& bufferView = document.bufferViews[bufferViewID];
		const size_t byteStride = bufferView.byteStride;
		const size_t floatStride = (byteStride == 0) ? 2 : (byteStride / sizeof(float));
		std::vector<float> uvs;
		uvs = resourceReader.ReadBinaryData<float>(document, document.bufferViews[bufferViewID]);
		const size_t offsetI = accessor.byteOffset / (sizeof(float));

		if (uvs.size() > 0 && accessor.count > 0) {
			uvdata.resize(accessor.count);
			for (size_t j = 0, iPos = offsetI; j < accessor.count; ++j, iPos += floatStride) {
				uvdata[j].x = uvs[iPos + 0];
				uvdata[j].y = uvs[iPos + 1];//DirectXは左上原点->なんかなってる
			}
		}
	}
	else {
		printf("caution!: モデルにUVがありません ないものとして処理します。\n");
	}
	//uv1の取得
	if (meshprim.TryGetAttributeAccessorId(ACCESSOR_TEXCOORD_1, accessorId))//ここでIDというものを使ってデータを呼べる
	{
		const Accessor& accessor = document.accessors.Get(accessorId);
		const auto data = resourceReader.ReadBinaryData<float>(document, accessor);
		for (int i = 0; i < data.size() - 1; i += 2) {
			DirectX::XMFLOAT2 uv1 = {};
			uv1.x = data[i];
			uv1.y = data[i + 1];
			uvdata1.push_back(uv1);
		}

	}

	//wheightidx
	auto weights = LoadWheightidx(meshprim, document, resourceReader);
	if (!(weights.empty())) {
		for (int i = 0; i < weights.size() - 3; i += 4) {
			DirectX::XMFLOAT4 w = {};
			w.x = weights[i];
			w.y = weights[i + 1];
			w.z = weights[i + 2];
			w.w = weights[i + 3];
			weightidx.push_back(w);
		}
	}
	//jointidx
	auto jointidxs = LoadJointidx(meshprim, document, resourceReader);
	if (!(jointidxs.empty())) {
		for (int i = 0; i < jointidxs.size() - 3; i += 4) {
			DirectX::XMFLOAT4 idx = {};
			idx.x = jointidxs[i];
			idx.y = jointidxs[i + 1];
			idx.z = jointidxs[i + 2];
			idx.w = jointidxs[i + 3];
			jointidx.push_back(idx);
		}
	}




	//変換処理
	std::vector<GltfVertex> data;
	data.resize(pdata.size());
	int count1 = 0;
	for (auto& d : pdata) {//頂点座標
		data[count1].pos.x = -d.x;//
		data[count1].pos.y = d.y;
		data[count1].pos.z = d.z;
		count1++;
	}
	count1 = 0;
	for (auto& n : ndata) {//法線
		data[count1].normal.x = -n.x;
		data[count1].normal.y = n.y;
		data[count1].normal.z = n.z;
		count1++;
	}
	count1 = 0;
	for (auto& uv : uvdata) {//uv
		data[count1].uv.x = uv.x;
		data[count1].uv.y = uv.y;//gltfはデフォルトで（0、0）
		count1++;
	}
	count1 = 0;
	for (auto& w : weightidx) {//wheightidx
		data[count1].bone_weightidx.x = w.x;
		data[count1].bone_weightidx.y = w.y;
		data[count1].bone_weightidx.z = w.z;
		data[count1].bone_weightidx.w = w.w;
		count1++;
	}
	count1 = 0;
	for (auto& joi : jointidx) {//jointidx
		data[count1].bone_jointidx.x = joi.x;
		data[count1].bone_jointidx.y = joi.y;
		data[count1].bone_jointidx.z = joi.z;
		data[count1].bone_jointidx.w = joi.w;
		count1++;
	}


	return data;
}
//index
void DeserializeIndex(int meshidx, const std::string& accessID, const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader, std::vector<std::vector<int>>& indexdatas) {


	const int indicesID = std::stoi(accessID);
	const Accessor& accessor = document.accessors[indicesID];
	const int bufferViewID = std::stoi(accessor.bufferViewId);
	ComponentType compType = accessor.componentType;
	if (compType == COMPONENT_UNSIGNED_BYTE) {
		std::vector<unsigned char> indices;
		indices = resourceReader.ReadBinaryData<unsigned char>(document, document.bufferViews[bufferViewID]);
		const int offsetI = accessor.byteOffset / sizeof(unsigned char);
		indexdatas[meshidx].resize(accessor.count);
		for (size_t j = 0; j < accessor.count; ++j) {
			indexdatas[meshidx][j] = (int)indices[j + offsetI];
		}
	}
	else if (compType == COMPONENT_UNSIGNED_SHORT) {
		std::vector<unsigned short> indices;
		indices = resourceReader.ReadBinaryData<unsigned short>(document, document.bufferViews[bufferViewID]);
		const int offsetI = accessor.byteOffset / sizeof(unsigned short);
		indexdatas[meshidx].resize(accessor.count);
		for (size_t j = 0; j < accessor.count; ++j) {
			indexdatas[meshidx][j] = (int)indices[j + offsetI];
		}
	}
	else {
		std::vector<int> indices;
		indices = resourceReader.ReadBinaryData<int>(document, document.bufferViews[bufferViewID]);
		const int offsetI = accessor.byteOffset / sizeof(int);
		indexdatas[meshidx].resize(accessor.count);
		for (size_t j = 0; j < accessor.count; ++j) {
			indexdatas[meshidx][j] = indices[j + offsetI];
		}
	}
}
//ウェイトindex
std::vector<float> LoadWheightidx(const Microsoft::glTF::MeshPrimitive& meshprim, const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader) {
	std::string accessorId;
	std::vector<float> weights;
	//ウェイトの取得
	if (meshprim.TryGetAttributeAccessorId(ACCESSOR_WEIGHTS_0, accessorId))
	{
		const int weightsID = std::stoi(accessorId);
		const Accessor& acce = document.accessors[weightsID];
		const int bufferViewID = std::stoi(acce.bufferViewId);
		const BufferView& bufferView = document.bufferViews[bufferViewID];
		const size_t byteStride = bufferView.byteStride;
		if (acce.componentType == COMPONENT_FLOAT) {//一般的な型
			weights = resourceReader.ReadBinaryData<float>(document, document.bufferViews[bufferViewID]);
		}
	}
	return weights;
}
//ジョイントindex
std::vector<unsigned short> LoadJointidx(const Microsoft::glTF::MeshPrimitive& meshprim, const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader) {
	std::string accessorId;
	std::vector<unsigned short> joints;
	if (meshprim.TryGetAttributeAccessorId(ACCESSOR_JOINTS_0, accessorId))
	{
		const int jointID = std::stoi(accessorId);
		const Accessor& acce = document.accessors[jointID];
		const int bufferViewID = std::stoi(acce.bufferViewId);
		const BufferView& bufferView = document.bufferViews[bufferViewID];
		const size_t byteStride = bufferView.byteStride;
		/*
		セマンティクスに指定できる型分ジョイントを格納できるらしい　https://qiita.com/mebiusbox2/items/c6b1c1f401a8df8df552
		*/
		if (acce.componentType == COMPONENT_UNSIGNED_SHORT) {//一般的な型
			std::vector<unsigned short> joints2;
			joints2 = resourceReader.ReadBinaryData<unsigned short>(document, document.bufferViews[bufferViewID]);
			joints.resize(joints2.size());
			for (int i = 0; i < joints2.size(); ++i) {
				joints[i] = (int)joints2[i];
			}
		}
	}
	return joints;
}

void GltfLoader::InitVData(std::shared_ptr< D3D12DeviceManager>& device) {
	int meshmaxsize = data._vdata.size();
	_mesh.resize(meshmaxsize);
	for (int meshidx = 0; meshidx < meshmaxsize; meshidx++) {
		GltfMesh m = {};
		std::shared_ptr<VertexBufferManager> vbm(new VertexBufferManager());
		m._vm = vbm;
		m._vm->CreateVertexBuffer(device, data._vdata[meshidx].size(), sizeof(GltfVertex));
		m._vm->ResoceMap(&m._vaddress);
		DISABLE_C4996
			std::copy(
				data._vdata[meshidx].begin(),
				data._vdata[meshidx].end(),
				m._vaddress
			);
		ENABLE_C4996
			m._vm->ResoceUnmap();
		_mesh[meshidx] =m;
	}

}


void GltfLoader::InitVIndexData(std::shared_ptr< D3D12DeviceManager>& device) {
	std::shared_ptr< IndexBufferManager> idxm(new IndexBufferManager());
	 int meshmaxsize = data._idxdata.size();
	for (int meshidx = 0; meshidx < meshmaxsize; meshidx++) {
		//インデックスバッファ作成
		idxm->CreateIndexBuffer(device, (const int)data._idxdata[meshidx].size(), sizeof(int));
		idxm->CreateBufferViewR32((const int)data._idxdata[meshidx].size(), sizeof(int));
		idxm->ResoceMap(&_mesh[meshidx]._indexAddress);
		DISABLE_C4996
			std::copy(
				data._idxdata[meshidx].begin(),
				data._idxdata[meshidx].end(),
				_mesh[meshidx]._indexAddress
			);
		ENABLE_C4996
			idxm->ResoceUnmap();
		_mesh[meshidx].fecenum = (const int)data._idxdata[meshidx].size();
		_mesh[meshidx]._idxm = idxm;
	}
}



void GltfLoader::Load(
	std::shared_ptr<D3D12DeviceManager>& device,
	std::shared_ptr<ComandManager>& comand,
	std::shared_ptr<FenceManager>& fence,
	const std::string& filepath
    ) {
	


	//絶対パスじゃないとダメ
	std::string path = filepath;
	std::unique_ptr<Microsoft::glTF::GLBResourceReader> glbrr;//Glbとgltf相互変換が可能
	Microsoft::glTF::Document doc;
	GltfCreateStreamReaderAndDoc(glbrr, doc, path);

	


	
	Deserialize(doc, *glbrr, data, _texm, filepath,device,comand, fence);



	InitVData(device);
	InitVIndexData(device);
	CreateBoneMatrix();
	LoadAnimation(doc,*glbrr);
	
}

void GltfLoader::CreateBoneMatrix() {
	bonedraw_data.resize(sceneNodeObjTRS.size());
	_bonenodes.resize(sceneNodeObjTRS.size());
	bonemat.resize(sceneNodeObjTRS.size());




	for (auto& joinidx : join) {
		bonemat[joinidx] = DirectX::XMMatrixIdentity();
		if (sceneNodeObjTRS[joinidx].tailIdx == 0) {
			continue;//ボーン番号0（Name = "Armature"）を飛ばす
		}
		_boneList[sceneNodeObjTRS[joinidx].Name] = joinidx;//親にindexを入れる
		bonedraw_data[joinidx].headpos = DirectX::XMFLOAT3(
			sceneNodeObjTRS[joinidx].tdata.translation[0],
			sceneNodeObjTRS[joinidx].tdata.translation[1],
			sceneNodeObjTRS[joinidx].tdata.translation[2]
		);
		if (sceneNodeObjTRS[joinidx].parentIndex != -1) {//子供に親のindexを入れる
			_bonenodes[sceneNodeObjTRS[joinidx].parentIndex].child.push_back(joinidx);
		}

		bonedraw_data[joinidx].tailpos = DirectX::XMFLOAT3(
			sceneNodeObjTRS[sceneNodeObjTRS[joinidx].tailIdx].tdata.translation[0],
			sceneNodeObjTRS[sceneNodeObjTRS[joinidx].tailIdx].tdata.translation[1],
			sceneNodeObjTRS[sceneNodeObjTRS[joinidx].tailIdx].tdata.translation[2]
		);
		bonedraw_data[joinidx].inverseBindMatrices = sceneNodeObjTRS[joinidx].inverseBindMatrices;
	}

	int stoped = 0;

}

int counts = 0;//ｵｰﾊﾞｰﾌﾛｰ用
void GltfLoader::Bone_Rotation(unsigned int bonemapindex) {
	//std::vector<Bone>& bones = bonedraw_data;


	auto idx = _boneList["Armature_Shoulder_R"];


	auto headpos = DirectX::XMFLOAT3(
		sceneNodeObjTRS[idx].tdata.translation[0],
		sceneNodeObjTRS[idx].tdata.translation[1],
		sceneNodeObjTRS[idx].tdata.translation[2]
	);
	auto tailposs = DirectX::XMFLOAT3(
		sceneNodeObjTRS[sceneNodeObjTRS[idx].tailIdx].tdata.translation[0],
		sceneNodeObjTRS[sceneNodeObjTRS[idx].tailIdx].tdata.translation[1],
		sceneNodeObjTRS[sceneNodeObjTRS[idx].tailIdx].tdata.translation[2]
	);
	DirectX::XMVECTOR offsetVec = DirectX::XMLoadFloat3(&headpos);
	DirectX::XMMATRIX bone = DirectX::XMMatrixTranslationFromVector(-offsetVec);
	DirectX::XMVECTOR tailpos = DirectX::XMLoadFloat3(&tailposs);


	//回転:
	bone *= DirectX::XMMatrixRotationZ(XM_PIDIV4);


	bone *= DirectX::XMMatrixTranslationFromVector(offsetVec);
	bone *= sceneNodeObjTRS[idx].inverseBindMatrices;


	bonemat[idx] *= bone;



}
//再帰関数 旧
void GltfLoader::Recursive_Bone(std::vector<BoneNode>& bonenodes, std::vector<Bone>& bonedraw, unsigned short boneidx, DirectX::XMMATRIX& bonem) {
	
	//循環ツリー
	//boneidx 6 -> 1child
	//boneidx 4 -> 6child
	for (auto& idx : bonenodes[boneidx].child) {
		//ヘッダの回転
		DirectX::XMVECTOR headposvec = XMLoadFloat3(&bonedraw[idx].headpos);
		headposvec = XMVector3Transform(headposvec, bonem);
		XMStoreFloat3(&bonedraw[idx].headpos, headposvec);
		 //テールの回転
		DirectX::XMVECTOR tallposvec = XMLoadFloat3(&bonedraw[idx].tailpos);
		tallposvec = XMVector3Transform(tallposvec, bonem);
		XMStoreFloat3(&bonedraw[idx].tailpos, tallposvec);
		bonemat[idx] *= bonem;
		counts += 1;//6000千回もループしている->StackOverFlowになる
		Recursive_Bone(bonenodes, bonedraw, idx, bonem);
	}
}
void LoadAnimation(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLBResourceReader& resourceReader) {








	////キーフレームをアニメーションデータとして登録
	//for (auto& pose : _vmdanimationdata->GetMotionData()) {
	//	_animation[pose.BoneName].second.push_back(Pose(pose.FrameNo, XMLoadFloat4(&pose.Rotatation)));
	//}

	
}
void GltfLoader::BoneFrameUpdata(unsigned int frameno) {


	//行列初期化
	std::fill(bonemat.begin(), bonemat.end(), DirectX::XMMatrixIdentity());
	Bone_Rotation(_boneList["Armature_Neck"]);
	//int bc = 0;
	//for (auto& anim : _animation) {
	//	Motion_t it = _animation[anim.first].second;
	//	auto frameIt = std::find_if(it.rbegin(), it.rend(), [frameno](const Pose& p) {return p.frameNo <= frameno; });//Nフレームの最大のキーフレームのイテレータを返す

	//	if (frameIt == it.rend()) {
	//		continue;
	//	}
	//	auto nextIt = frameIt.base();

	//	if (nextIt == it.end()) {
	//		Bone_Rotation(_boneList[anim.first], DirectX::XMMatrixRotationQuaternion(frameIt->quaternion));//次のキーフレーム
	//	}
	//	else {
	//		float a = frameIt->frameNo;
	//		float b = nextIt->frameNo;
	//		float t = (static_cast<float>(frameno) - a) / (b - a);
	//		Bone_Rotation2(
	//			_boneList[anim.first],
	//			frameIt->quaternion,
	//			nextIt->quaternion,
	//			t
	//		);//補間
	//	}
	//}

}


std::vector<std::vector<GltfVertex>>& GltfLoader::GetVertexData() {
	return data._vdata;
}
std::vector<GltfMesh>& GltfLoader::GetMeshData() {
	return _mesh;
}
std::vector<std::shared_ptr<TextureManager>>& GltfLoader::GetTexture() {
	return _texm;
}
std::vector<DirectX::XMMATRIX>& GltfLoader::GetBoneMat() {
	return bonemat;
}