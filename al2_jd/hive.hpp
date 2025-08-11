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
		// コモンダイアログが表示されている場合はTRUEになります。
		//
		BOOL is_comdlg32_visible = FALSE;

		//
		// このクラスはダークモード化の設定です。
		//
		struct JD
		{
			//
			// TRUEの場合はコモンダイアログをダークモード化から除外します。
			//
			BOOL exclude_comdlg32 = FALSE;
		} jd;

		//
		// このクラスはスクロールバーの設定です。
		//
		struct ScrollBar
		{
			//
			// スクロールバーの縮小率です。
			//
			int32_t reduction = 50;

			//
			// TRUEの場合は矢印をボタンのように描画します。
			//
			BOOL arrow_as_button = FALSE;

			//
			// TRUEの場合はつまみのグリッパーを描画します。
			//
			BOOL has_gripper = FALSE;
		} scrollbar;

		//
		// このクラスはその他の設定です。
		//
		struct Etc
		{
			//
			// TRUEの場合は起動時にaviutl2を最大化します。
			//
			BOOL maximize_aviutl2 = FALSE;

			//
			// TRUEの場合は起動時に最近使ったプロジェクトを開きます。
			//
			BOOL open_recent_project = FALSE;
		} etc;

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
		// このクラスはシーンを作成するときの設定です。
		//
		struct NewScene
		{
			//
			// 最近使った設定値です。
			//
			struct Recent {
				std::wstring name;
				std::wstring video_width;
				std::wstring video_height;
				std::wstring video_rate;
				std::wstring audio_rate;
			} recent;
		} new_scene;

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
				std::wstring name;
				std::wstring video_width;
				std::wstring video_height;
				std::wstring video_rate;
				std::wstring audio_rate;
			};
			std::vector<Preset> preset_collection;

			//
			// 名前のプリセットです。
			//
			struct Name {
				std::wstring display_name;
				std::wstring name;
			};
			std::vector<Name> name_collection;

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

			//
			// レイヤー名のプリセットです。
			//
			struct LayerName {
				std::wstring name;
			};
			std::vector<Name> layer_name_collection;
		} presets;

		//
		// レンダラを使用するかどうかのフラグです。
		// レンダラの使用を停止したい場合はTRUEに設定します。
		//
		thread_local inline static BOOL renderer_locked = FALSE;

		//
		// メインスレッドのIDです。
		//
		const DWORD main_thread_id = ::GetCurrentThreadId();

		//
		// 現在のスレッドが有効の場合はTRUEを返します。
		//
		BOOL is_valid_thread() const { return !renderer_locked && (main_thread_id == ::GetCurrentThreadId()); }

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
