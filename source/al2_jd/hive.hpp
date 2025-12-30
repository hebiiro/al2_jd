#pragma once

namespace apn::dark
{
	namespace
	{
		//
		// このクラスは::PolyPatBlt()の引数です。
		//
		struct PATRECT {
			int x, y, w, h; HBRUSH brush;
		};
	}

	//
	// このクラスは他クラスから共通して使用される変数を保持します。
	//
	inline struct hive_t
	{
		inline static constexpr auto c_name = L"JD";
		inline static constexpr auto c_display_name = L"ダークモード化";

		inline static constexpr struct message_t {
			//
			// 初期化後のウィンドウメッセージです。
			//
			inline static const auto c_post_init = ::RegisterWindowMessageW(L"al2_jd::post_init");
		} c_message;

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
		// 現在表示されているコモンダイアログの数です。
		//
		ULONG comdlg32_visible_count = {};

		//
		// このクラスはダイアログ名(正規表現パターン)です。
		//
		struct dialog_name_t
		{
			//
			// 「プロジェクトを新規作成」のダイアログ名です。
			//
			std::wstring new_project = L"プロジェクトを新規作成|New Project";

			//
			// 「シーンを作成」のダイアログ名です。
			//
			std::wstring new_scene = L"シーンを作成|New Scene";

			//
			// 「シーンの設定」のダイアログ名です。
			//
			std::wstring set_scene = L"シーンの設定|Scene Settings";

			//
			// 「レイヤー名を変更」のダイアログ名です。
			//
			std::wstring set_layer_name = L"レイヤー名を変更|Rename Layer";

			//
			// 「フォントメニューの設定」のダイアログ名です。
			//
			std::wstring set_font_menu = L"フォントメニュー|Font Menu";
		} dialog_name;

		//
		// このクラスはダークモード化の設定です。
		//
		struct jd_t
		{
			//
			// スタイルのファイル名です。
			//
			std::wstring style_file_name;

			//
			// カスタムカラーのファイル名です。
			//
			std::wstring custom_color_file_name;

			//
			// TRUEの場合はコモンダイアログをダークモード化から除外します。
			//
			BOOL exclude_comdlg32 = FALSE;

			//
			// TRUEの場合はDirect2Dを使用して描画します。
			//
			BOOL use_d2d = FALSE;
		} jd;

		//
		// このクラスはスクロールバーの設定です。
		//
		struct scrollbar_t
		{
			//
			// TRUEの場合は矢印をボタンのように描画します。
			//
			BOOL arrow_as_button = FALSE;

			//
			// TRUEの場合はつまみのグリッパーを描画します。
			//
			BOOL has_gripper = FALSE;

			//
			// スクロールバーの縮小率です。%単位です。
			//
			int32_t reduction = 50;
		} scrollbar;

		//
		// このクラスは丸みの設定です。
		//
		struct round_t
		{
			//
			// TRUEの場合は矩形の角を丸めます。
			//
			BOOL flag_use = TRUE;

			//
			// 丸角のサイズです。%単位です。
			//
			int32_t size = 20;
		} round;

		//
		// このクラスは縁の設定です。
		//
		struct border_t
		{
			//
			// TRUEの場合は縁を3Dのように描画します。
			//
			BOOL flag_3d_edge = FALSE;

			//
			// 縁の幅です。1/10px単位です。
			//
			int32_t width = 10;
		} border;

		//
		// このクラスはグラデーションの設定です。
		//
		struct gradient_t
		{
			//
			// TRUEの場合は背景をグラデーションで描画します。
			//
			BOOL flag_use = TRUE;

			//
			// TRUEの場合はグラデーション終了色にアルファを適用します。
			//
			BOOL flag_end_alpha = TRUE;

			//
			// グラデーション終了色のアルファです。%単位です。
			//
			int32_t end_alpha = 60;
		} gradient;

		//
		// このクラスは影の設定です。
		//
		struct shadow_t
		{
			//
			// TRUEの場合はテキストの影を描画します。
			//
			BOOL flag_use = TRUE;

			//
			// TRUEの場合は影をぼかします。
			//
			BOOL flag_blur = TRUE;

			//
			// 影のオフセット(位置)です。1/10px単位です。
			//
			POINT offset = { 10, 30 };

			//
			// ぼかしの範囲です。1/10px単位です。
			//
			int32_t size = 15;
		} shadow;

		//
		// このクラスはその他の設定です。
		//
		struct etc_t
		{
			//
			// TRUEの場合は通常のマウスアクティブ化処理を実行します。
			//
			BOOL default_mouse_activate = TRUE;
		} etc;

		//
		// このクラスはフォントプレビューの設定です。
		//
		struct fonts_t
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
		} fonts;

		//
		// このクラスはプリセットの設定です。
		//
		struct presets_t
		{
			//
			// (全体の)プリセットです。
			//
			struct preset_t {
				std::wstring display_name;
				std::wstring name;
				std::wstring video_width;
				std::wstring video_height;
				std::wstring video_rate;
				std::wstring audio_rate;
			};
			std::vector<preset_t> preset_collection;

			//
			// 名前のプリセットです。
			//
			struct name_t {
				std::wstring display_name;
				std::wstring name;
			};
			std::vector<name_t> name_collection;

			//
			// 映像サイズのプリセットです。
			//
			struct video_size_t {
				std::wstring display_name;
				std::wstring width;
				std::wstring height;
			};
			std::vector<video_size_t> video_size_collection;

			//
			// 映像レートのプリセットです。
			//
			struct video_rate_t {
				std::wstring display_name;
				std::wstring rate;
			};
			std::vector<video_rate_t> video_rate_collection;

			//
			// 音声レートのプリセットです。
			//
			struct audio_rate_t {
				std::wstring display_name;
				std::wstring rate;
			};
			std::vector<audio_rate_t> audio_rate_collection;

			//
			// レイヤー名のプリセットです。
			//
			struct layer_name_t {
				std::wstring name;
			};
			std::vector<layer_name_t> layer_name_collection;
		} presets;

		//
		// レンダラを使用するかどうかのフラグです。
		// レンダラの使用を停止したい場合はTRUEに設定します。
		//
		thread_local inline static BOOL is_renderer_locked = FALSE;

		//
		// メインスレッドのIDです。
		//
		const DWORD main_thread_id = ::GetCurrentThreadId();

		//
		// 現在のスレッドが有効の場合はTRUEを返します。
		//
		BOOL is_valid_thread() const
		{
			// レンダラーがロックされている場合は無効です。
			if (is_renderer_locked) return FALSE;

			// メインスレッドではない場合は無効です。
			if (main_thread_id != ::GetCurrentThreadId()) return FALSE;

			// コモンダイアログを除外する場合は
			if (jd.exclude_comdlg32)
			{
				// コモンダイアログが表示されていない場合は有効です。
				return comdlg32_visible_count == 0;
			}

			// このスレッドは有効です。
			return TRUE;
		}

		//
		// このクラスはフックする前のオリジナルのAPIを保持します。
		//
		struct orig_t
		{
			LRESULT (WINAPI *CallWindowProcWInternal)(void* u1, WNDPROC wnd_proc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
			decltype(&::Rectangle) Rectangle;
			decltype(&::FillRect) FillRect;
			BOOL (WINAPI *DrawFrame)(HDC dc, LPRECT rc, UINT width, UINT type);
			decltype(&::DrawFrameControl) DrawFrameControl;
			decltype(&::FrameRect) FrameRect;
			decltype(&::DrawEdge) DrawEdge;
			decltype(&::DrawFocusRect) DrawFocusRect;
			decltype(&::DrawStateW) DrawStateW;
			decltype(&::GrayStringW) GrayStringW;
			decltype(&::DrawTextW) DrawTextW;
			decltype(&::DrawTextExW) DrawTextExW;
			decltype(&::DrawShadowText) DrawShadowText;
			decltype(&::DrawMenuBar) DrawMenuBar;
			decltype(&::ExtTextOutW) ExtTextOutW;
			decltype(&::PatBlt) PatBlt;
			BOOL (WINAPI *PolyPatBlt)(HDC dc, DWORD rop, const PATRECT* rects, int nb_rects, ULONG reserved);
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
