#pragma once

namespace apn::dark::kuro::gdi
{
	//
	// このクラスはGDIレンダラーを管理します。
	//
	inline struct Manager
	{
		//
		// 指定されたウィンドウに対応するGDIレンダラーを返します。
		//
		std::shared_ptr<Renderer> find_renderer(
			HWND hwnd, const Renderer::NormalizedClassName& class_name)
		{
//			auto instance = (HINSTANCE)::GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
//			auto style = my::get_style(hwnd);

			if (class_name == L"#32770")
			{
//				if (instance == ::GetModuleHandleW(L"comdlg32.dll") && style & WS_THICKFRAME)
/*				if (instance == ::GetModuleHandleW(L"comdlg32.dll"))
					return std::make_shared<comdlg32::DialogRenderer>();
				else*/
					return std::make_shared<DialogRenderer>();
			}

			if (class_name == WC_STATIC) return std::make_shared<StaticRenderer>();
			if (class_name == WC_BUTTON) return std::make_shared<ButtonRenderer>();
			if (class_name == WC_EDIT) return std::make_shared<EditBoxRenderer>();
//			if (class_name == RICHEDIT_CLASS) return std::make_shared<RichEditRenderer>();
			if (class_name == WC_LISTBOX) return std::make_shared<ListBoxRenderer>();
			if (class_name == L"ListviewPopup") return std::make_shared<ListBoxRenderer>();
			if (class_name == L"ComboLBox") return std::make_shared<ListBoxRenderer>();
			if (class_name == WC_COMBOBOX) return std::make_shared<ComboBoxRenderer>();
			if (class_name == WC_COMBOBOXEX) return std::make_shared<ComboBoxExRenderer>();
			if (class_name == TOOLTIPS_CLASS) return std::make_shared<ToolTipRenderer>();
			if (class_name == TRACKBAR_CLASS) return std::make_shared<TrackBarRenderer>();
			if (class_name == UPDOWN_CLASS) return std::make_shared<SpinRenderer>();
			if (class_name == WC_TABCONTROL) return std::make_shared<TabRenderer>();
			if (class_name == WC_HEADER) return std::make_shared<HeaderRenderer>();
			if (class_name == WC_LISTVIEW) return std::make_shared<ListViewRenderer>();
			if (class_name == WC_TREEVIEW) return std::make_shared<TreeViewRenderer>();
//			if (class_name == TOOLBARCLASSNAME) return std::make_shared<ToolBarRenderer>();

			// aviutl2のメインウィンドウです。
			if (class_name == L"aviutl2Manager") return std::make_shared<AviUtl2Renderer>();

			// ファイル選択ダイアログのリストビューなどの親ウィンドウです。
//			if (class_name == L"DirectUIHWND") return std::make_shared<comdlg32::DirectUIHWNDRenderer>();

			// DirectUIHWNDの親ウィンドウです。
//			if (class_name == L"DUIViewWndClassName") return std::make_shared<comdlg32::DirectUIHWNDRenderer>();

			// ファイル選択ダイアログのツリービューの親ウィンドウです。
			if (class_name == L"NamespaceTreeControl") return std::make_shared<Renderer>();

			if (0) // テスト用コードです。
			{
				return std::make_shared<Renderer>();
			}

			return nullptr;
		}

		//
		// GDIレンダラーをウィンドウに関連付けされます。
		//
		void attach_renderer(HWND hwnd)
		{
			auto class_name = my::get_class_name(hwnd);

			MY_TRACE_FUNC("{/hex}, {/}", hwnd, class_name);

			if (auto renderer = find_renderer(hwnd, class_name))
				renderer->attach(hwnd);
		}
		//
		// 初期化処理を実行します。
		//
		BOOL init(HWND hwnd)
		{
			MY_TRACE_FUNC("");

			// aviutl2ウィンドウにレンダラーを関連付けます。
			attach_renderer(hwnd);

			// aviutl2ウィンドウがアクティブな場合は
			// WM_ACTIVATEを送信してNC領域の配色を更新させます。
			if (::GetActiveWindow() == hwnd)
				::SendMessage(hwnd, WM_ACTIVATE, TRUE, 0);

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
