#pragma once
namespace K3D {

	class CommandAllocator
	{
	public:

	private:

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _allocator;

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
		HRESULT Create(unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		/**
		* @fn
		* @brief コマンドリストの作成
		* @param[in] device 作成に使うデバイス
		* @param[in] nodeMask ノードマスク
		* @param[in] listType リストのタイプ
		* @return リザルト　S_OKで成功
		*/
		HRESULT Create(D3D12Device* device,unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& GetAllocator();

		void ResetAllocator();

		void Discard();

	private:

	};

}