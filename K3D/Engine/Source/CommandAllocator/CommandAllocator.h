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
		* @brief �R�}���h���X�g�̍쐬
		* @param[in] nodeMask �m�[�h�}�X�N
		* @param[in] listType ���X�g�̃^�C�v
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT Create(unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		/**
		* @fn
		* @brief �R�}���h���X�g�̍쐬
		* @param[in] device �쐬�Ɏg���f�o�C�X
		* @param[in] nodeMask �m�[�h�}�X�N
		* @param[in] listType ���X�g�̃^�C�v
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT Create(D3D12Device* device,unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& GetAllocator();

		void ResetAllocator();

		void Discard();

	private:

	};

}