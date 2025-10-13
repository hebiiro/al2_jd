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
		// コンフィグをaviutl2ウィンドウに適用します。
		//
		BOOL apply_config()
		{
			MY_TRACE_FUNC("");

			// aviutl2ウィンドウを最前面にします。
			::SetForegroundWindow(hive.theme_window);

			return TRUE;
		}

		//
		// スリムバーの設定をaviutl2ウィンドウに適用します。
		//
		BOOL apply_slim_menubar()
		{
			return !!::SendMessage(hive.theme_window, my::slimbar_t::c_message.c_apply_config, 0, 0);
		}

		//
		// スリムバーのレイアウトを更新します。
		//
		BOOL recalc_slim_menubar()
		{
			return !!::SendMessage(hive.theme_window, my::slimbar_t::c_message.c_update_layout, 0, 0);
		}
	} aviutl2_window;
}
