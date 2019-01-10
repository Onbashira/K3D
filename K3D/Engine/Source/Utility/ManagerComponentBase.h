#pragma once
#include <memory>
#include <map>
#include <unordered_map>
#include <string>


namespace K3D {
	//�V���v���ȃ��\�[�X�L�����A�[�̃N���X
	template <class T>
	class ManagerComponentBase
	{
	private:

		std::map<std::string, std::shared_ptr<T>> _resourceMap;

	protected:
	
	public:
	
	private:
	
	public:

		ManagerComponentBase() {};

		virtual ~ManagerComponentBase() { DiscardMap(); };
		//�v�f�̒ǉ��B��O��������True�ŋ����㏑��
		void Set(std::string name, std::shared_ptr<T> resource, bool forceOverride = false);
		//�v�f�̒ǉ��B��O��������True�ŋ����㏑��
		void Set(std::string name, const T& resource, bool forceOverride = false);
		//�v�f�̎擾
		std::weak_ptr<T>	 Get(std::string name);
		//�v�f�̍폜
		void Erase(std::string name);
		//�}�b�v�̖����I�N���A
		void DiscardMap();
		//���\�[�X�����[�h�ς݂��ۂ�
		bool IsLooded(std::string name);
		//�}�b�v���̂̎Q�Ƃ��擾����
		std::map<std::string, std::shared_ptr<T>>& GetMap();


	};


	template <class T>
	class NonRaiiManagerComponentBase
	{
	private:

	protected:

		std::map<std::string, T> _resourceMap;

	public:

	private:

	public:

		NonRaiiManagerComponentBase() {};

		virtual ~NonRaiiManagerComponentBase() { DiscardMap(); };
		//�v�f�̒ǉ��B��O��������True�ŋ����㏑��
		void Set(std::string name,const T& resource, bool forceOverride = false);
		//�v�f�̎擾
		T*	 Get(std::string name);
		//�v�f�̍폜
		void Erase(std::string name);
		//�}�b�v�̖����I�N���A
		void DiscardMap();
		//���\�[�X�����[�h�ς݂��ۂ�
		bool IsLooded(std::string name);
		//�}�b�v���̂̎Q�Ƃ��擾����
		std::map<std::string, T>& GetMap();


	};

	template <class T>
	class UnorderedManagerComponentBase
	{
	private:

	protected:

		std::unordered_map<std::string, std::shared_ptr<T>> _resourceMap;

	public:

	private:

	public:	
		
		UnorderedManagerComponentBase() {};

		virtual ~UnorderedManagerComponentBase() { DiscardMap(); };
		//�v�f�̒ǉ��B��O��������True�ŋ����㏑��
		void Set(std::string name, std::shared_ptr<T> resource, bool forceOverride = false);
		//�v�f�̒ǉ��B��O��������True�ŋ����㏑��
		void Set(std::string name, const T & resource, bool forceOverride = false);
		//�v�f�̎擾
		std::weak_ptr<T>  Get(std::string name);
		//�v�f�̍폜
		void Erase(std::string name);
		//�}�b�v�̖����I�N���A
		void DiscardMap();
		//���\�[�X�����[�h�ς݂��ۂ�
		bool IsLooded(std::string name);
		//�}�b�v���̂̎Q�Ƃ��擾����
		std::unordered_map<std::string, std::shared_ptr<T>>& GetMap();
	};
}

template<class T>
inline void K3D::ManagerComponentBase<T>::Set(std::string name, std::shared_ptr<T> resource, bool forceOverride)
{

	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = resource;
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = resource;

	}
}

template<class T>
inline void K3D::ManagerComponentBase<T>::Set(std::string name, const T & resource, bool forceOverride)
{
	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = std::make_shared<T>(resource);
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = std::make_shared<T>(resource);

	}
}

template<class T>
inline std::weak_ptr<T> K3D::ManagerComponentBase<T>::Get(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return _resourceMap[name];
	}
	return std::weak_ptr<T>();
}

template<class T>
inline void K3D::ManagerComponentBase<T>::Erase(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		_resourceMap.erase(name);
	}
}

template<class T>
inline void K3D::ManagerComponentBase<T>::DiscardMap()
{
	_resourceMap.clear();
}

template<class T>
inline bool K3D::ManagerComponentBase<T>::IsLooded(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return true;
	}
	return false;
}

template<class T>
inline std::map<std::string, std::shared_ptr<T>>& K3D::ManagerComponentBase<T>::GetMap()
{

	return _resourceMap;
}


//NoRaii


template<class T>
inline void K3D::NonRaiiManagerComponentBase<T>::Set(std::string name, const T & resource, bool forceOverride)
{
	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = resource;
	}
	else if (forceOverride) {
		//_resourceMap[name];
		_resourceMap[name] = resource;

	}
}

template<class T>
inline T* K3D::NonRaiiManagerComponentBase<T>::Get(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return &_resourceMap[name];
	}
	return nullptr;
}

template<class T>
inline void K3D::NonRaiiManagerComponentBase<T>::Erase(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		_resourceMap.erase(name);
	}
}

template<class T>
inline void K3D::NonRaiiManagerComponentBase<T>::DiscardMap()
{
	_resourceMap.clear();

}

template<class T>
inline bool K3D::NonRaiiManagerComponentBase<T>::IsLooded(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return true;
	}
	return false;
}

template<class T>
inline std::map<std::string, T>& K3D::NonRaiiManagerComponentBase<T>::GetMap()
{
	return _resourceMap;
}

//UnorderedMap

template<class T>
inline void K3D::UnorderedManagerComponentBase<T>::Set(std::string name, std::shared_ptr<T> resource, bool forceOverride)
{

	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = resource;
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = resource;

	}
}

template<class T>
inline void K3D::UnorderedManagerComponentBase<T>::Set(std::string name, const T & resource, bool forceOverride)
{
	if (_resourceMap.find(name) == _resourceMap.end()) {
		_resourceMap[name] = std::make_shared<T>(resource);
	}
	else if (forceOverride) {
		_resourceMap[name].reset();
		_resourceMap[name] = std::make_shared<T>(resource);

	}
}

template<class T>
inline std::weak_ptr<T> K3D::UnorderedManagerComponentBase<T>::Get(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return _resourceMap[name];
	}
	return std::weak_ptr<T>();
}

template<class T>
inline void K3D::UnorderedManagerComponentBase<T>::Erase(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		_resourceMap.erase(name);
	}
}

template<class T>
inline void K3D::UnorderedManagerComponentBase<T>::DiscardMap()
{
	_resourceMap.clear();
}  

template<class T>
inline bool K3D::UnorderedManagerComponentBase<T>::IsLooded(std::string name)
{
	if (_resourceMap.find(name) != _resourceMap.end()) {
		return true;
	}
	return false;
}

template<class T>
inline std::unordered_map<std::string, std::shared_ptr<T>>& K3D::UnorderedManagerComponentBase<T>::GetMap()
{
	return _resourceMap;
}


