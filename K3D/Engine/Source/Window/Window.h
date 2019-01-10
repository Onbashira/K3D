#pragma once
#include <string>
#include <functional>

#include <Windows.h>
#include <d3d12.h>


namespace K3D {
	class Window
	{
	private:
		
		HINSTANCE					_hInstance;
		
		HWND						_hWindow;
		
		D3D12_VIEWPORT				_viewPort;
		
		D3D12_RECT					_scissorRect;
		
		float						_aspectRatio;
		
		std::wstring				_appClassName;
		
		static LRESULT __stdcall	WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
		LRESULT(_stdcall*_windowProc)(HWND, UINT, WPARAM, LPARAM);
	
	public:

	private:
		
		void CreateScissorRect(UINT width, UINT height);
		
		void CreateViewPort(UINT width, UINT height);

	public:

		Window();
		
		~Window();

		HRESULT			Create(std::wstring windowName, UINT width, UINT height);
		
		HRESULT			CustomCreate(std::wstring windowName, UINT width, UINT height, LRESULT(_stdcall*windowProc)(HWND, UINT, WPARAM, LPARAM));

		HWND &			GetWindowHandle();
		
		HINSTANCE&		GetWindowInstance();
		
		D3D12_VIEWPORT	GetViewPort();
		
		D3D12_RECT		GetScissorRect();
		
		void			Discard();
	};
}