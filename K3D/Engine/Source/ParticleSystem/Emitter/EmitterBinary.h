#pragma once
namespace K3D {
	class Resource;
	class Emitter;
	class D3D12Device;
	struct CPUEmitterHeader;
	class EmitterBinary
	{

	public:

	private:

		std::unique_ptr<K3D::Resource> _emitterBin;

		std::unique_ptr<K3D::Resource> _emitterHeadersBin;

		std::unique_ptr<K3D::Resource> _emitterTableBin;

		std::vector<unsigned int> _binaryEmitterSizeMap;

		std::vector<CPUEmitterHeader> _cpuEmitterHeader;

		std::vector<unsigned int > _emtTable;


	public:

		EmitterBinary();

		~EmitterBinary();

		HRESULT Initialize(std::shared_ptr<D3D12Device>& device , unsigned int binarySize);

		//Emitterが保持する各アイテムの情報をバイナリ領域に書き込み
		void Write(const CPUEmitterHeader* header,std::shared_ptr<Emitter>& emitter);

		void WriteEmitterHeader(const CPUEmitterHeader* header, unsigned int index);

		void DeleteEmitter(int emitterIdx);

		void Discard();
	private:

		HRESULT EmitterBinInit(std::shared_ptr<D3D12Device>& device, unsigned int binarySize);
		
		HRESULT EmitterHeaderesBinInit(std::shared_ptr<D3D12Device>& device, unsigned int binarySize);
		
		HRESULT EmitterTableBinInit(std::shared_ptr<D3D12Device>& device, unsigned int binarySize);



	};

}