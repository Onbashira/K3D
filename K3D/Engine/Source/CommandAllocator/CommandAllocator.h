#pragma once
namespace K3D {

	class D3D12Device;

	class CommandAllocator
	{
	public:

	private:

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

		void SetName(String name);
		
		String& GetName();

		void ResetAllocator();

		void Discard();

	private:

	};

}