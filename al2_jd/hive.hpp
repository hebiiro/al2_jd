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
		// 使用可能なフォントのコレクションです。
		//
		std::unordered_set<std::wstring> available_fonts;

		//
		// アセットのファイル名です。
		//
		std::wstring assets_file_name;

		//
		// コンフィグのファイル名です。
		//
		std::wstring config_file_name;

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
		// TRUEの場合は起動時にaviutl2を最大化します。
		//
		BOOL maximize_aviutl2 = FALSE;

		//
		// TRUEの場合は起動時に最近使ったプロジェクトを開きます。
		//
		BOOL open_recent_project = FALSE;

		//
		// TRUEの場合はファイル選択ダイアログもダークモード化します。
		//
		BOOL apply_file_dialog = FALSE;

		//
		// TRUEの場合はチェックボックス(とラジオボタン)を特殊化します。
		//
		BOOL specialize_checkbox = FALSE;

		//
		// このクラスはフォントプレビューの設定です。
		//
		struct Fonts
		{
			//
			// リストボックスのサイズです。
			//
			SIZE window_size = { 1000, 800 };

			//
			// アイテムの高さです。
			//
			int item_height = 48;

			//
			// フォントの高さです。
			//
			int font_height = 48;

			//
			// サンプル文字列の書式です。
			//
			std::wstring sample_text_format = L"%font% サンプル0123456789";

			//
			// TRUEの場合はメニューでフォントを使用して描画します。
			//
			BOOL use_on_menu = TRUE;

			//
			// TRUEの場合はリストボックスでフォントを使用して描画します。
			//
			BOOL use_on_listbox = TRUE;

			//
			// TRUEの場合はリストビューでフォントを使用して描画します。
			//
			BOOL use_on_listview = TRUE;

			//
			// 「フォントメニューの設定」ダイアログの名前です。
			//
			std::wstring setting_dialog_name = L"フォントメニューの設定";
		} fonts;

		//
		// このクラスはプロジェクトを作成するときの設定です。
		//
		struct NewProject
		{
			//
			// 最近使った設定値です。
			//
			struct Recent {
				std::wstring video_width;
				std::wstring video_height;
				std::wstring video_rate;
				std::wstring audio_rate;
			} recent;
		} new_project;

		//
		// このクラスはプリセットの設定です。
		//
		struct Presets
		{
			//
			// (全体の)プリセットです。
			//
			struct Preset {
				std::wstring display_name;
				std::wstring video_width;
				std::wstring video_height;
				std::wstring video_rate;
				std::wstring audio_rate;
			};
			std::vector<Preset> preset_collection;

			//
			// 映像サイズのプリセットです。
			//
			struct VideoSize {
				std::wstring display_name;
				std::wstring width;
				std::wstring height;
			};
			std::vector<VideoSize> video_size_collection;

			//
			// 映像レートのプリセットです。
			//
			struct VideoRate {
				std::wstring display_name;
				std::wstring rate;
			};
			std::vector<VideoRate> video_rate_collection;

			//
			// 音声レートのプリセットです。
			//
			struct AudioRate {
				std::wstring display_name;
				std::wstring rate;
			};
			std::vector<AudioRate> audio_rate_collection;
		} presets;

		//
		// レンダラを使用するかどうかのフラグです。
		// レンダラの使用を停止したい場合はTRUEに設定します。
		//
		thread_local inline static BOOL renderer_locked = FALSE;

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
			decltype(&::GetSysColor) GetSysColor;
			decltype(&::GetSysColorBrush) GetSysColorBrush;

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
