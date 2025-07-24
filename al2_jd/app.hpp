#pragma once

namespace apn::dark
{
	//
	// このクラスはアプリケーションです。
	//
	inline struct App : AppInterface
	{
		//
		// コンストラクタです。
		//
		App() { app = this; }

		//
		// aviutl2ウィンドウを取得します。
		//
		BOOL get_aviutl2_window()
		{
			MY_TRACE_FUNC("");

			return ::EnumThreadWindows(::GetCurrentThreadId(), [](HWND hwnd, LPARAM lParam)
			{
				auto class_name = my::get_class_name(hwnd);
				MY_TRACE_STR(class_name);

				if (class_name == L"aviutl2Manager")
				{
					hive.theme_window = hwnd;

					return FALSE;
				}

				return TRUE;
			}, 0);
		}

		//
		// dllの初期化処理を実行します。
		//
		virtual BOOL dll_init() override
		{
			MY_TRACE_FUNC("");

			// テスト用コード。
			auto window_color = ::GetSysColor(COLOR_WINDOW);
			auto button_color = ::GetSysColor(COLOR_BTNFACE);
			auto background_color = ::GetSysColor(COLOR_BACKGROUND);
			auto menu_color = ::GetSysColor(COLOR_MENU);
			auto hilight_color = ::GetSysColor(COLOR_HIGHLIGHT);

			// aviutl2ウィンドウを取得します。
			get_aviutl2_window();

			kuro::hook::manager.init();
			kuro::style.init();
			kuro::paint::manager.init();
			kuro::gdi::manager.init(hive.theme_window);
			kuro::theme::manager.init(hive.theme_window);

			return TRUE;
		}

		//
		// dllの後始末処理を実行します。
		//
		virtual BOOL dll_exit() override
		{
			MY_TRACE_FUNC("");

			kuro::theme::manager.exit();
			kuro::gdi::manager.exit();
			kuro::paint::manager.exit();
			kuro::style.exit();
			kuro::hook::manager.exit();

			return TRUE;
		}
	} app_impl;
}
