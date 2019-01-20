#pragma once
#include "Engine/Source/Math/Math.h"

namespace K3D {

	//!�I�u�W�F�N�g�̈ړ��A��]����ێ��A�v�Z����N���X
	class Transform
	{
	public:

	protected:
		//!���[�g�I�u�W�F�N�g�̈ʒu
		Vector3 _pos;

		//!���[�g�I�u�W�F�N�g�̔C�ӎ��Ɖ�]��
		Quaternion _rotation;

		//!�ێ��I�C���[�p
		Vector3 _eulerAngles;

	private:

	public:

		Transform();

		virtual ~Transform();

		void SetPos(const Vector3 pos);

		void SetRotation(const Quaternion rotation);

		void SetEulerAngles(const Vector3 euler);

		Vector3 GetPos();

		Quaternion GetRotation();

		Vector3 GetEulerAngles();

		///@fn				�I�u�W�F�N�g�̈ړ��i���[�J������j
		///@param			(velocity) �ω���	
		void				Translate(const Vector3& velocity);


		///@fn				�I�u�W�F�N�g�̈ړ��i���[���h����j
		///@param			(velocity) �ω���	
		void				Move(const Vector3& velocity);


		///@fn				���_���S��]
		///@param			(rot) �l����	
		void				Rotation(const Quaternion& rot);

		///@fn				���[�J�����S��]
		///@param			(rot) �l����	
		void				RotationLocalWorld(const Quaternion& rot);

		///@fn�I�C���[�p�Ō��_���S��]
		///@param			(eulerAngles) �ω���	
		void				RotationLocalWorld(const Vector3& eulerAngles);

		///@fn�I�C���[�p�Ō��_���S��]
		///@param			(eulerAngles) ��Η�	
		void				RotationEulerAngles(const Vector3& eulerAngles);

		///@fn���Ɖ�]�ʂŉ�]
		///@param			(axis)	��
		///@param			(rad)	�ʓx
		void				RotationAxisAngles(const Vector3& axis, float rad);

		///@fn�|�C���g����Ɏ��Ɖ�]�ʂŉ�]
		///@param			(point)	���E���W
		///@param			(rot)	��]��
		void				RotateAround(const Vector3& point, const Quaternion& rot);
		///@fn�|�C���g����Ɏ��Ɖ�]�ʂŉ�]
		///@param			(point)	���E���W
		///@param			(axis)	��
		///@param			(rad)	�ʓx
		void				RotateAround(const Vector3& point, const Vector3& axis, float rad);

		///@fn�I�u�W�F�N�g�̃��[�J��Forward�����|�C���g������悤�ɉ�]
		void				LookAt(const Vector3& point, const Vector3& up);

		///@fn���[���h���W�Ō������[�J�����̎擾
		OrthonormalBasis	GetLocalAxis();

		///@fn���̃I�u�W�F�N�g�̃r���[�s����擾
		Matrix				GetView();

	private:

	};
}