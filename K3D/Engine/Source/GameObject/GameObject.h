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

		//@fn				オブジェクトの移動（ワールド軸基準）
		//@param[in] velocity 変化量	
		void				Translate(const Vector3& velocity);

		//@fn				オブジェクトの移動（ローカル軸基準）
		//@param[in] velocity 変化量	
		void				Move(const Vector3& velocity);

		//@fn				原点中心回転(四元数の積で計算)
		//@param[in] rot 変化量	
		void				Rotation(const Quaternion& rot);

		//@fnオイラー角で原点中心回転
		//@param[in] eulerAngles 絶対量	
		void				RotationEulerAngles(const Vector3& eulerAngles);

		//@fn軸と回転量で回転
		//@param[in] axis	軸
		//@param[in] rad	弧度
		void				RotationAxisAngles(const Vector3& axis, float rad);

		//@fnポイント周りに軸と回転量で回転
		//@param[in] point	座標
		//@param[in] rot	変化量
		void				RotateAround(const Vector3& point, const  Quaternion& rot);

		//@fnポイント周りに軸と回転量で回転
		//@param[in] point	座標
		//@param[in] axis	軸
		//@param[in] rad	弧度
		void				RotateAround(const Vector3& point, const Vector3& axis, float rad);

		//@fnオブジェクトのローカルForward軸がポイントを見るように回転
		//@param[in] point	座標
		//@param[in] up	　　Y軸
		void				LookAt(const Vector3& point, const Vector3& up);

		//@fnワールド座標で見たローカル軸の取得
		//@return ローカル軸
		OrthonormalBasis	GetLocalAxis();

		//@fnこのオブジェクトのビュー行列を取得
		//@return ビュー行列
		Matrix				GetView();

		//@fnこのオブジェクトのSRT行列を取得
		//@return SRT行列
		Matrix				GetSRTMatrix();

	protected:

		virtual void UpdateTransformBuffer();
		//バッファリソースにSize分の1次元バッファとして領域を確保する；
		virtual void InitalizeTransformBuffer(size_t size);
		//シェーダーにバインディング
		virtual void SetTransform(int rootParamaterIndex, std::shared_ptr<CommandList> list);

	private:

	};

}
