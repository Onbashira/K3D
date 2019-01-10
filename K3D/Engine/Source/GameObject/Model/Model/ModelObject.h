#pragma once
#include <vector>

namespace K3D {
	//�x�[�X�̃��f���I�u�W�F�N�g�@��{���b�V�����A�`�悷�邽�߂̃q�[�v�A�`�悷�邽�߂̃p�C�v���C�����A�Q�[���I�u�W�F�N�g����������
	class ModelObject : public GameObject ,public ModelMesh
	{
	private:

	public:

	protected:
		std::vector<Vertex3D> _vertexes;

		std::vector<unsigned int> _indexList;
	private:

	protected:
		//�o���h�����X�g�Ɍp������L�̃h���[�R�[����ςލ�Ƃ�����
		virtual void RegisterToBundle()override = 0;
	public:
		//���f���̃A�b�v�f�[�g
		virtual void Update()override = 0;
		//�`�掞�Ăяo���֐��̍쐬������
		virtual void Draw()override = 0;

		void DiscardModelObject();

		ModelObject();
		virtual ~ModelObject();
	};
}

