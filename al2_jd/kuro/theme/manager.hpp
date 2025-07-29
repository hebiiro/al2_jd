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
				register_renderer(VSCLASS_STATIC, &static_renderer);
				register_renderer(VSCLASS_BUTTON, &button_renderer);
				register_renderer(VSCLASS_EDIT, &editbox_renderer);
				register_renderer(VSCLASS_COMBOBOX, &combobox_renderer);
				register_renderer(VSCLASS_LISTBOX, &listbox_renderer);
				register_renderer(VSCLASS_TOOLTIP, &tooltip_renderer);
				register_renderer(VSCLASS_TRACKBAR, &trackbar_renderer);
				register_renderer(VSCLASS_SPIN, &spin_renderer);
				register_renderer(VSCLASS_TAB, &tab_renderer);
				register_renderer(VSCLASS_HEADER, &header_renderer);
				register_renderer(VSCLASS_LISTVIEW, &listview_renderer);
				register_renderer(VSCLASS_TREEVIEW, &treeview_renderer);
				register_renderer(L"Explorer::" VSCLASS_MENU, &menu_renderer);
				register_renderer(L"Explorer::" VSCLASS_SCROLLBAR, &scrollbar_renderer);
				register_renderer(L"Explorer::" VSCLASS_STATIC, &static_renderer);
				register_renderer(L"Explorer::" VSCLASS_BUTTON, &button_renderer);
				register_renderer(L"Explorer::" VSCLASS_EDIT, &editbox_renderer);
				register_renderer(L"Explorer::" VSCLASS_COMBOBOX, &combobox_renderer);
				register_renderer(L"Explorer::" VSCLASS_LISTBOX, &listbox_renderer);
				register_renderer(L"Explorer::" VSCLASS_TOOLTIP, &tooltip_renderer);
				register_renderer(L"Explorer::" VSCLASS_TRACKBAR, &trackbar_renderer);
				register_renderer(L"Explorer::" VSCLASS_SPIN, &spin_renderer);
				register_renderer(L"Explorer::" VSCLASS_TAB, &tab_renderer);
				register_renderer(L"Explorer::" VSCLASS_HEADER, &header_renderer);
				register_renderer(L"Explorer::" VSCLASS_LISTVIEW, &listview_renderer);
				register_renderer(L"Explorer::" VSCLASS_TREEVIEW, &treeview_renderer);

				// コモンダイアログのテーマ
				register_renderer(L"ItemsView", &itemsview_renderer);
				register_renderer(L"ItemsView::" VSCLASS_HEADER, &header_renderer);
				register_renderer(L"ItemsView::" VSCLASS_LISTVIEW, &listview_renderer);
				register_renderer(L"Button-OK;Button", &button_renderer);
				register_renderer(L"Button-CANCEL;Button", &button_renderer);
				register_renderer(L"CFD::" VSCLASS_COMBOBOX, &combobox_renderer);


/*
* その他のVSCLASS
L"Link"
L"EmptyMarkup"
VSCLASS_REBAR
L"Navigation"
L"AddressBand"
L"Progress"
L"BreadcrumbBar"
L"SearchBox"
L"SearchBoxComposited::SearchBox"
L"SearchEditBox"
L"ReadingPane"
L"PreviewPane"
L"ExplorerNavPane"
L"ButtonStyle"
L"TooltipStyle"
L"ListViewStyle" <= VSCLASS_LISTVIEWSTYLE
L"FlyoutStyle" <= VSCLASS_FLYOUTSTYLE
L"CONTROLPANELSTYLE" <= VSCLASS_CONTROLPANELSTYLE
L"TextStyle" <= VSCLASS_TEXTSTYLE
L"MediaStyle"

L"CommandModule"
L"CPLCommandModule::CommandModule"
L"Communications::" VSCLASS_REBAR
L"InfoBar"
L"MonthCal"
L"PROPERTREE"
L"TryHarder"
L"EditComposited::Edit"
L"UISubsetFooter::ControlPanel"
L"ItemsViewAccessible::Header"

L""

*/
				// シェルのテーマ
				register_renderer(L"ImmersiveStart::" VSCLASS_MENU, &immersive_start::menu_renderer);

				// ウィンドウを再描画してカスタムテーマを反映させます。
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
