#pragma once

namespace apn::dark
{
	//
	// このクラスは他クラスから共通して使用される変数を保持します。
	//
	inline struct Hive
	{
		inline static constexpr auto c_name = L"JD";
		inline static constexpr auto c_display_name = L"ダークモード化";

		//
		// このアドインのインスタンスハンドルです。
		//
		HINSTANCE instance = nullptr;

		//
		// テーマ用のウィンドウです。
		//
		HWND theme_window = nullptr;

		//
		// スキンの角の丸みです。
		//
		int32_t ellipse = 5;

		//
		// スキンの縁の幅です。
		//
		int32_t border_width = 1;

		//
		// スキンの影の濃度です。
		//
		int32_t shadow_density = 80;

		//
		// スクロールバーの縮小率です。
		//
		int32_t scrollbar_reduction = 50;

		//
		// TRUEの場合はファイル選択ダイアログもダークモード化します。
		//
		BOOL apply_file_dialog = FALSE;

		//
		// TRUEの場合はチェックボックス(とラジオボタン)を特殊化します。
		//
		BOOL specialize_checkbox = FALSE;

		//
		// レンダラを使用するかどうかのフラグです。
		// レンダラの使用を停止したい場合はTRUEに設定します。
		//
		thread_local inline static BOOL renderer_locked = FALSE;
/*
		//
		// 現在処理中のビジュアルスタイルです。
		//
		thread_local inline static struct {
			HTHEME theme;
			int part_id;
			int state_id;
			inline void set(HTHEME theme, int part_id, int state_id) {
				this->theme = theme;
				this->part_id = part_id;
				this->state_id = state_id;
			}
		} current_processing_vs = { nullptr, 0, 0 };
*/
		//
		// このクラスはフックする前のオリジナルのAPIを保持します。
		//
		struct {
			LRESULT (WINAPI *CallWindowProcWInternal)(void* u1, WNDPROC wnd_proc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
			decltype(&::Rectangle) Rectangle;
			decltype(&::FillRect) FillRect;
			BOOL (WINAPI *DrawFrame)(HDC dc, LPRECT rc, UINT width, UINT type);
			decltype(&::DrawFrameControl) DrawFrameControl;
			decltype(&::FrameRect) FrameRect;
			decltype(&::DrawEdge) DrawEdge;
			decltype(&::DrawFocusRect) DrawFocusRect;
			decltype(&::DrawStateW) DrawStateW;
			decltype(&::ExtTextOutW) ExtTextOutW;
			decltype(&::PatBlt) PatBlt;

			decltype(&::GetThemeColor) GetThemeColor;
			decltype(&::DrawThemeParentBackground) DrawThemeParentBackground;
			decltype(&::DrawThemeBackground) DrawThemeBackground;
			decltype(&::DrawThemeBackgroundEx) DrawThemeBackgroundEx;
			decltype(&::DrawThemeText) DrawThemeText;
			decltype(&::DrawThemeTextEx) DrawThemeTextEx;
			decltype(&::DrawThemeIcon) DrawThemeIcon;
			decltype(&::DrawThemeEdge) DrawThemeEdge;
		} orig = {};

		//
		// メッセージボックスを表示します。
		//
		int32_t message_box(const std::wstring& text, HWND hwnd = nullptr, int32_t type = MB_OK | MB_ICONWARNING) {
			if (!hwnd) hwnd = theme_window;
			return ::MessageBoxW(hwnd, text.c_str(), c_display_name, type);
		}
	} hive;
}
