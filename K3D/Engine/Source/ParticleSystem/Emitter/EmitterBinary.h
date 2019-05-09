#pragma once
namespace K3D {
	class Resource;
	class Emitter;
	class D3D12Device;

	class EmitterBinary
	{

	public:

	private:

		std::unique_ptr<K3D::Resource> _emitterBin;

		std::unique_ptr<K3D::Resource> _emitterHeadersBin;

		std::unique_ptr<K3D::Resource> _emitterTableBin;

		std::map<int, size_t> _binaryEmitterSizeMap;

	public:

		EmitterBinary();

		~EmitterBinary();

		HRESULT Initialize(std::shared_ptr<D3D12Device>& device ,size_t binarySize);

		//Headerおよび各アイテムの情報をバイナリ領域に書き込み
		void Write(std::shared_ptr<Emitter>& emitter);

		void DeleteEmitter(int emitterIdx);

		void Discard();
	private:

		HRESULT EmitterBinInit(std::shared_ptr<D3D12Device>& device, size_t binarySize);
		HRESULT EmitterHeaderesBinInit(std::shared_ptr<D3D12Device>& device, size_t binarySize);
		HRESULT EmitterTableBinInit(std::shared_ptr<D3D12Device>& device, size_t binarySize);

	};

}