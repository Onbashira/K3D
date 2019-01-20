#pragma once
#include "Engine/Source/Utility/D3D12Common.h"

namespace K3D {
	class GeometryState
	{
	public:

	private:

		//!���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW _vertexBuffer;
		//!�C���f�b�N�X�o�b�t�@�r���[
		D3D12_INDEX_BUFFER_VIEW  _indexBuffer;

	public:

		GeometryState();

		~GeometryState();

		/**
		* @fn
		* @brief ���_�o�b�t�@�̎擾
		* @return  ���_�o�b�t�@�ւ̎Q��
		*/
		D3D12_VERTEX_BUFFER_VIEW& GetVertexBuffer();

		/**
		* @fn
		* @brief �C���f�b�N�X�o�b�t�@�̎擾
		* @return  �C���f�b�N�X�o�b�t�@�ւ̎Q��
		*/
		D3D12_INDEX_BUFFER_VIEW& GetIndexBuffef();

		/**
		* @fn
		* @brief ���_�o�b�t�@�̃Z�b�g
		* @oaram[in] vertexBuffer�@VBO
		*/
		void SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW& vertexBuffer);

		/**
		* @fn
		* @brief �C���f�b�N�X�o�b�t�@�̃Z�b�g
		* @oaram[in] vertexBuffer�@VBO
		*/
		void SetIndexBuffef(D3D12_INDEX_BUFFER_VIEW& indexBuffer);

	private:

	};
}