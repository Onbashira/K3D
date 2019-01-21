#pragma once
#include"Engine/Source/Utility/D3D12Common.h"


namespace K3D {
	
	class D3D12Device;

	class Factory
	{

	public:

	private:
		//!�t�@�N�g���[
		Microsoft::WRL::ComPtr<IDXGIFactory5>	_factory;

	public:

		Factory();

		~Factory();

		/**
		* @fn
		* @brief �q�[�v�̃t�F�b�`
		* @param[in] createFlags �I�v�V�����t���O
		* @return S_OK�Ő���
		*/
		HRESULT	Create(UINT createFlags = 0);

		/**
		* @fn
		* @brief �t�@�N�g���̃t�F�b�`
		* @return �t�@�N�g���̎Q��
		*/
		Microsoft::WRL::ComPtr<IDXGIFactory5>& GetFactory();

		/**
		* @fn
		* @brief �t�@�N�g���̔j��
		*/
		void Discard();

	private:

	};

}