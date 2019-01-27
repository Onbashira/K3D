#pragma once
#include "Transform.h"
#include "Engine/Source/Math/Math.h"
#include "Engine/Source/Resource/ConstantBuffer.h"
#include <memory>


namespace K3D {

	class CommandList;

	class GameObject
	{
	public:

	protected:

		Transform		_transform;

		Vector3			_scale;

		unsigned int	_initializeSize;

		bool			_isEnable;

		ConstantBuffer	_transformBuffer;

	private:

	public:

		GameObject();

		GameObject(Transform transform);

		virtual ~GameObject();

		virtual void Update() = 0;

		void SetPos(const Vector3 pos);

		void SetRotation(const Quaternion rotation);

		void SetEulerAngles(const Vector3 euler);

		void SetScale(const Vector3 scale);

		Vector3 GetPos();

		Quaternion GetRotation();

		Vector3 GetEulerAngles();

		Vector3 GetScale();

		void Enable();

		void Disable();

		bool IsEnable();

		//@fn				�I�u�W�F�N�g�̈ړ��i���[���h����j
		//@param[in] velocity �ω���	
		void				Translate(const Vector3& velocity);

		//@fn				�I�u�W�F�N�g�̈ړ��i���[�J������j
		//@param[in] velocity �ω���	
		void				Move(const Vector3& velocity);

		//@fn				���_���S��](�l�����̐ςŌv�Z)
		//@param[in] rot �ω���	
		void				Rotation(const Quaternion& rot);

		//@fn�I�C���[�p�Ō��_���S��]
		//@param[in] eulerAngles ��Η�	
		void				RotationEulerAngles(const Vector3& eulerAngles);

		//@fn���Ɖ�]�ʂŉ�]
		//@param[in] axis	��
		//@param[in] rad	�ʓx
		void				RotationAxisAngles(const Vector3& axis, float rad);

		//@fn�|�C���g����Ɏ��Ɖ�]�ʂŉ�]
		//@param[in] point	���W
		//@param[in] rot	�ω���
		void				RotateAround(const Vector3& point, const  Quaternion& rot);

		//@fn�|�C���g����Ɏ��Ɖ�]�ʂŉ�]
		//@param[in] point	���W
		//@param[in] axis	��
		//@param[in] rad	�ʓx
		void				RotateAround(const Vector3& point, const Vector3& axis, float rad);

		//@fn�I�u�W�F�N�g�̃��[�J��Forward�����|�C���g������悤�ɉ�]
		//@param[in] point	���W
		//@param[in] up	�@�@Y��
		void				LookAt(const Vector3& point, const Vector3& up);

		//@fn���[���h���W�Ō������[�J�����̎擾
		//@return ���[�J����
		OrthonormalBasis	GetLocalAxis();

		//@fn���̃I�u�W�F�N�g�̃r���[�s����擾
		//@return �r���[�s��
		Matrix				GetView();

		//@fn���̃I�u�W�F�N�g��SRT�s����擾
		//@return SRT�s��
		Matrix				GetSRTMatrix();

	protected:

		virtual void UpdateTransformBuffer();
		//�o�b�t�@���\�[�X��Size����1�����o�b�t�@�Ƃ��ė̈���m�ۂ���G
		virtual void InitalizeTransformBuffer(size_t size);
		//�V�F�[�_�[�Ƀo�C���f�B���O
		virtual void SetTransform(int rootParamaterIndex, std::shared_ptr<CommandList> list);

	private:

	};

}
