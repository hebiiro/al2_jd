#pragma once

namespace apn::dark
{
	//
	// このクラスはaviutl2ウィンドウを管理します。
	//
	inline struct AviUtl2Window
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			return get_aviutl2_window();
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

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
		// スリムメニューバーの設定をaviutl2ウィンドウに適用します。
		//
		BOOL apply_slim_menubar()
		{
			// メニューバーとタイトルバーを一体化する場合は
			if (hive.jd.slim_menubar)
			{
//				my::modify_style(hive.theme_window, WS_CAPTION, 0);
//				my::modify_style(hive.theme_window, WS_DLGFRAME, 0);
				my::modify_style(hive.theme_window, WS_BORDER, 0);
			}
			// メニューバーとタイトルバーを一体化しない場合は
			else
			{
				my::modify_style(hive.theme_window, 0, WS_CAPTION);
			}

			return ::SetWindowPos(hive.theme_window, nullptr, 0, 0, 0, 0,
				SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		}
	} aviutl2_window;
}
