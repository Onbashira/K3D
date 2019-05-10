#pragma once
namespace K3D {
	class Resource;
	class Emitter;
	class D3D12Device;
	struct EmitterHeader;
	class EmitterBinary
	{

	public:

	private:

		std::unique_ptr<K3D::Resource> _emitterBin;

		std::unique_ptr<K3D::Resource> _emitterHeadersBin;

		std::unique_ptr<K3D::Resource> _emitterTableBin;

		std::map<int, unsigned int> _binaryEmitterSizeMap;

	public:

		EmitterBinary();

		~EmitterBinary();

		HRESULT Initialize(std::shared_ptr<D3D12Device>& device , unsigned int binarySize);

		//Emitterが保持する各アイテムの情報をバイナリ領域に書き込み
		void Write(const EmitterHeader* header,std::shared_ptr<Emitter>& emitter);

		void DeleteEmitter(int emitterIdx);

		void Discard();
	private:

		HRESULT EmitterBinInit(std::shared_ptr<D3D12Device>& device, unsigned int binarySize);
		HRESULT EmitterHeaderesBinInit(std::shared_ptr<D3D12Device>& device, unsigned int binarySize);
		HRESULT EmitterTableBinInit(std::shared_ptr<D3D12Device>& device, unsigned int binarySize);

		void WriteEmitterHeader(const EmitterHeader* header);


	};

}