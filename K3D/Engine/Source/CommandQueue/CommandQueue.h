#pragma once
#include "Engine/Source/Async/Fence.h"
#include <string>
namespace K3D {

	class D3D12Device;
	class CommandList;

	class CommandQueue
	{


	public:
	private:

		//!�L���[
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>	_commandQueue;
		
		//!�t�F���X
		K3D::Fence	_fence;
				
		//!�L���[�̖��O
		std::string _name;

	public:

		CommandQueue();

		~CommandQueue();

		/**
		* @fn
		* @brief �L���[�쐬
		* @param[in] desc �L���[�̃f�X�N���v�V����
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT Create(D3D12_COMMAND_QUEUE_DESC& desc);
		
		/**
		* @fn
		* @brief �L���[�쐬
		* @param[in] device �f�o�C�X
		* @param[in] desc �L���[�̃f�X�N���v�V����
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT Create(std::weak_ptr<D3D12Device> device,D3D12_COMMAND_QUEUE_DESC& desc);

		/**
		* @fn
		* @brief �L���[�̃t�F�b�`
		* @return �L���[�ւ̎Q��
		*/
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetQueue();

		/**
		* @fn
		* @brief �ҋ@
		* @param[in] fence�@�t�F���X�@NullPtr�ŕێ��t�F���X�őҋ@
		*/
		void Wait(Fence* fence = nullptr);

		/**
		* @fn
		* @brief �^�C���X�^���v�̎擾
		* @return �^�C���X�^���v
		*/
		UINT64 GetTimestampFrequency();

		/**
		* @fn
		* @brief �L���[�̃f�X�N���v�V�����̎擾
		* @return �f�X�N���v�V����
		*/
		D3D12_COMMAND_QUEUE_DESC& GetDesc();

		/**
		* @fn
		* @brief �L���[�̃f�X�N���v�V�����̎擾
		* @param[in] lists�@�R�}���h���X�g�x�N�^�ւ̎Q��
		* @return �f�X�N���v�V����
		*/
		void ExecuteCommandLists(std::vector<std::shared_ptr<CommandList>>& lists);

		/**
		* @fn
		* @brief �L���[�̖��O�̐ݒ�
		* @param[in] name ���O
		*/
		void SetName(std::string name);

		/**
		* @fn
		* @brief �j��
		*/
		void Discard();

	private:
	};
}

