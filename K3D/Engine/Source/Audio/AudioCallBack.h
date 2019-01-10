#pragma once
#include <xaudio2.h>
#include <functional>
#include <map>

namespace K3D {
	class AudioCallBack : public IXAudio2VoiceCallback
	{	
	public:
		static inline unsigned int AUDIO_BUFFER_QUEUE_MAX = 2;
	private:

	protected:
				
		std::function<void(UINT32)> _onVoiceProcessingPassStart;

		std::function<void()> _onVoiceProcessingPassEnd;

		std::function<void()> _onStreamEnd;

		std::function<void(void*)> _onBufferStart;

		std::function<void(void*)> _onBufferEnd;

		std::function<void(void*)> _onLoopEnd;

		std::function<void(void*, HRESULT)> _onVoiceError;



	public:

		AudioCallBack();

		virtual ~AudioCallBack();

		/**
		 * @fn
		 * @brief �J�n���ɃR�[�������֐�
		 */
		virtual void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired)override;

		/**
		 * @fn
		 * @brief �I�����ɃR�[�������
		 */
		virtual void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void)override;
		
		/**
		 * @fn
		 * @brief �X�g���[���G���h�ɃR�[��
		 */
		virtual void STDMETHODCALLTYPE OnStreamEnd(void)override;
		
		/**
		 * @fn
		 * @brief �o�b�t�@�X�^�[�g�̎��ɌĂ΂��
		 */
		virtual void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext)override;

		/**
		 * @fn
		 * @brief �o�b�t�@�̏I�[�ŌĂ΂��
		 */
		virtual void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext)override;

		/**
		 * @fn
		 * @brief ���[�v�G���h���ɌĂ΂��
		 */
		virtual void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext)override;

		/**
		 * @fn
		 * @brief �G���[���N�������Ƃ��ɌĂ΂��
		 */
		virtual void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error)override;

		/**
		 * @fn
		 * @brief �J�n���ɃR�[�������
		 */
		void  SetOnVoiceProcessingPassStart(std::function<void(UINT32)> func);

		/**
		 * @fn
		 * @brief �I�����ɃR�[�������
		 */
		void  SetOnVoiceProcessingPassEnd(std::function<void()> func);

		/**
		 * @fn
		 * @brief �X�g���[���G���h�ɃR�[��
		 */
		void  SetOnStreamEnd(std::function<void()> func);

		/**
		�@* @fn
		�@* @brief �o�b�t�@�X�^�[�g�̎��ɌĂ΂��
		�@*/
		void  SetOnBufferStart(std::function<void(void*)> func);

		/**
		 * @fn
		 * @brief �o�b�t�@�̏I�[�ŌĂ΂��
		 */
		void  SetOnBufferEnd(std::function<void(void*)> func);

		/**
		 * @fn
		 * @brief ���[�v�G���h���ɌĂ΂��
		 */
		void  SetOnLoopEnd(std::function<void(void*)> func);

		/**
		 * @fn
		 * @brief �G���[���N�������Ƃ��ɌĂ΂��
		 */
		void  SetOnVoiceError(std::function<void(void*,HRESULT)> func);

	private:

	protected:

	};
}
