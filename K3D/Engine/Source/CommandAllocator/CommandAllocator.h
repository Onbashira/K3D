#pragma once
namespace K3D {

	class D3D12Device;

	class CommandAllocator
	{
	public:

	private:

		bool _isDuringExecuting;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _allocator;

		String _name;


	public:

		CommandAllocator();

		~CommandAllocator();

		/**
		* @fn
		* @brief コマンドリストの作成
		* @param[in] nodeMask ノードマスク
		* @param[in] listType リストのタイプ
		* @return リザルト　S_OKで成功
		*/
		HRESULT Initialize(unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		/**
		* @fn
		* @brief コマンドリストの作成
		* @param[in] device 作成に使うデバイス
		* @param[in] nodeMask ノードマスク
		* @param[in] listType リストのタイプ
		* @return リザルト　S_OKで成功
		*/
		HRESULT Initialize(std::shared_ptr<D3D12Device>& device,unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& GetAllocator();

		void SetName(String name);
		
		String& GetName();

		void ResetAllocator();
		void ExecutedAllocator() {
			_isDuringExecuting = true;
		};

		void Discard();

	private:

	};

}