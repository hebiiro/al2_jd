#pragma once

namespace apn::dark::kuro::theme
{
	//
	// このクラスはテーマレンダラーを管理します。
	//
	inline struct Manager
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init(HWND hwnd)
		{
			MY_TRACE_FUNC("");

			// テーマハンドルとレンダラーを関連付けます。
			// テーマハンドルを開くとフック関数内でレンダラーと関連付けされます。
			{
				// ウィンドウのDPIを取得します。
				auto dpi = ::GetDpiForWindow(hwnd);
				MY_TRACE_INT(dpi);

				//
				// この関数はテーマレンダラーを登録します。
				//
				const auto register_renderer = [&](LPCWSTR class_list, Renderer* renderer)
				{
					// レンダラーをマップに追加します。
					from_vsclass.set(class_list, renderer);

					// テーマハンドルを開きます。
					my::theme::unique_ptr<> theme(::OpenThemeData(hwnd, class_list));
					MY_TRACE("{/} = {/hex}\n", class_list, theme.get());

					// 高DPI用のテーマハンドルを開きます。
					my::theme::unique_ptr<> theme_for_dpi(::OpenThemeDataForDpi(hwnd, class_list, dpi));
					MY_TRACE("{/} {/} = {/hex}\n", class_list, dpi, theme_for_dpi.get());

					// テーマハンドルを閉じるとハンドル値が変わってしまうので、閉じないようにします。
					theme.release();
					theme_for_dpi.release();
				};

				// 通常のテーマ
				register_renderer(VSCLASS_MENU, &menu_renderer);
				register_renderer(VSCLASS_SCROLLBAR, &scrollbar_renderer);
				register_renderer(VSCLASS_TOOLTIP, &tooltip_renderer);
				register_renderer(L"Explorer::" VSCLASS_MENU, &menu_renderer);
				register_renderer(L"Explorer::" VSCLASS_SCROLLBAR, &scrollbar_renderer);
				register_renderer(L"Explorer::" VSCLASS_TOOLTIP, &tooltip_renderer);

				// コモンダイアログのテーマ
				register_renderer(L"ItemsView", &itemsview_renderer);
				register_renderer(L"ItemsView::" VSCLASS_HEADER, &itemsview::header_renderer);
//				register_renderer(L"ItemsView::" VSCLASS_LISTVIEW, &itemsview::listview_renderer);

				// シェルのテーマ
				register_renderer(L"ImmersiveStart::" VSCLASS_MENU, &immersive_start::menu_renderer);

				::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
					SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE |
					SWP_FRAMECHANGED | SWP_NOCOPYBITS);
			}

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}
	} manager;
}
