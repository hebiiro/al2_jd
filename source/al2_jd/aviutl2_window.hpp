#pragma once

namespace apn::dark
{
	//
	// このクラスはaviutl2ウィンドウを管理します。
	//
	inline struct aviutl2_window_t
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

			// カレントスレッドのウィンドウを列挙します。
			return ::EnumThreadWindows(::GetCurrentThreadId(), [](HWND hwnd, LPARAM lParam)
			{
				// クラス名を取得します。
				auto class_name = my::get_class_name(hwnd);
				MY_TRACE_STR(class_name);

				// クラス名が一致しない場合は除外します。
				if (class_name != L"aviutl2Manager") return TRUE;

				// トップレベルウィンドウではない場合は除外します。
				if (::GetWindow(hwnd, GW_OWNER)) return TRUE;

				// aviutl2ウィンドウをセットします。
				hive.theme_window = hwnd;

				// 列挙を終了します。
				return FALSE;
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
