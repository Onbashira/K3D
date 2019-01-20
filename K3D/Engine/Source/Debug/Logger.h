#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <comdef.h>
#include "Engine/Source/Utility/NonCopyable.h"

namespace K3D {
	//!ログレベル
	enum class LogLevel {
		Details,
		Info,
		Debug,
		Warning,
		Error
	};

	class  ILogger {
	private:

	public:

	private:

	public:

		virtual ~ILogger() {};

		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		virtual void		Log(LogLevel level, std::string format) = 0;

		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		virtual void		LogW(LogLevel level, std::wstring format) = 0;

		/**
		* @fn
		* @brief フィルター以下のレベルのロギングを抑制するためフィルターの設定
		* @param[in] level 
		*/
		virtual void		SetFilter(LogLevel level) = 0;

		/**
		* @fn
		* @brief フィルターのフェッチ
		* @return フィルタ
		*/
		virtual LogLevel	GetFilter() = 0;

	};

	class  SystemLogger : public ILogger, private NonCopyable {
	private:

		//!フィルタ
		LogLevel _LogFilter;
		
		//！デバッグファイル
		std::ofstream _debugFile;

	public:

	private:

		SystemLogger();
		
		void CreateDebugFile();

	public:

		~SystemLogger();

		/**
		* @fn
		* @brief ファイルへマッピング
		* @param[in] fileName ファイル名
		*/
		void		Map(std::string fileName);
		
		/**
		* @fn
		* @brief マップ解除
		*/
		void		UnMap();
		
		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] hr リザルト
		*/
		void		Log(LogLevel level, HRESULT hr);
		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		void		Log(LogLevel level, std::string format);
		
		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		void		Log(LogLevel level, const char* format, ...);
		
		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		void		LogW(LogLevel level, std::wstring format);
		
		/**
		* @fn
		* @brief ロギング
		* @param[in] level ログレベル
		* @param[in] format ログ文
		*/
		void		LogW(LogLevel level, const wchar_t* format, ...);
		
		/**
		* @fn
		* @brief フィルター以下のレベルのロギングを抑制するためフィルターの設定
		* @param[in] level
		*/
		void		SetFilter(LogLevel level);
		
		/**
		* @fn
		* @brief フィルターのフェッチ
		* @return フィルタ
		*/
		LogLevel	GetFilter();
		
		/**
		* @fn
		* @brief 出力パスの設定
		* @param[in] filePath
		*/
		void		SetDebugFilePath(std::string filePath);
		
		/**
		* @fn
		* @brief インスタンスの取得
		* @return インスタンス
		*/
		static SystemLogger& GetInstance();
	};

#define HRESULT_LOG(hr)		K3D::SystemLogger::GetInstance().Log(K3D::LogLevel::Details,	"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, reinterpret_cast<const char*>(_com_error(hr).ErrorMessage()))
#define	DEBUG_LOG(str)		K3D::SystemLogger::GetInstance().Log(K3D::LogLevel::Debug,		"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define INFO_LOG(str)		K3D::SystemLogger::GetInstance().Log(K3D::LogLevel::Info ,		"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define DETAILS_LOG(str)	K3D::SystemLogger::GetInstance().Log(K3D::LogLevel::Details ,	"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define WARNING_LOG(str)	K3D::SystemLogger::GetInstance().Log(K3D::LogLevel::Warning ,	"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define ERROR_LOG(str)		K3D::SystemLogger::GetInstance().Log(K3D::LogLevel::Error ,		"[ FILE : %s, LINE : %d] { %s }\n", __FILE__, __LINE__, str.c_str())


#if defined(UNICODE) || defined(_UNICODE)
#else
#endif

}