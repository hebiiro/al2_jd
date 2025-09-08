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
//			if (class_name == L"#32768") return std::make_shared<MenuRenderer>();
			if (class_name == L"#32770")
			{
				auto text = my::get_window_text(hwnd);

				if (text == L"プロジェクトを新規作成")
					return std::make_shared<aviutl2::new_project::DialogRenderer>(FALSE);

				if (text == L"シーンを作成")
					return std::make_shared<aviutl2::new_project::DialogRenderer>(TRUE);

				if (text == L"シーンの設定")
					return std::make_shared<aviutl2::new_project::DialogRenderer>(TRUE);

				if (text == L"レイヤー名を変更")
					return std::make_shared<aviutl2::layer_name::DialogRenderer>();

				auto instance = (HINSTANCE)::GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
				auto comdlg32 = ::GetModuleHandleW(L"comdlg32.dll");
//				auto style = my::get_style(hwnd);

				// コモンダイアログの場合は
				if (instance == comdlg32)
//				if (instance == comdlg32 && style & WS_THICKFRAME)
					return std::make_shared<comdlg32::DialogRenderer>();

				return std::make_shared<DialogRenderer>();
			}

			if (class_name == WC_STATIC) return std::make_shared<StaticRenderer>();
			if (class_name == WC_BUTTON) return std::make_shared<ButtonRenderer>();
			if (class_name == WC_EDIT) return std::make_shared<EditBoxRenderer>();
//			if (class_name == RICHEDIT_CLASS) return std::make_shared<RichEditRenderer>();
			if (class_name == WC_LISTBOX)
			{
				// オーナーウィンドウがaviutl2の場合は
				if (::GetWindow(hwnd, GW_OWNER) == hive.theme_window)
					return std::make_shared<aviutl2::ListBoxRenderer>();
				else
					return std::make_shared<ListBoxRenderer>();
			}
			if (class_name == L"ListviewPopup") return std::make_shared<ListBoxRenderer>();
			if (class_name == L"ComboLBox") return std::make_shared<ListBoxRenderer>();
			if (class_name == WC_COMBOBOX) return std::make_shared<ComboBoxRenderer>();
			if (class_name == WC_COMBOBOXEX) return std::make_shared<ComboBoxExRenderer>();
			if (class_name == TOOLTIPS_CLASS) return std::make_shared<ToolTipRenderer>();
			if (class_name == TRACKBAR_CLASS) return std::make_shared<TrackBarRenderer>();
			if (class_name == UPDOWN_CLASS) return std::make_shared<SpinRenderer>();
			if (class_name == WC_TABCONTROL) return std::make_shared<TabRenderer>();
			if (class_name == WC_HEADER) return std::make_shared<HeaderRenderer>();
			if (class_name == WC_LISTVIEW)
			{
				// フォントを使用してリストビューを描画する場合は
				if (hive.fonts.use_on_listview)
				{
					auto parent = ::GetParent(hwnd);
					MY_TRACE_HWND(parent);

					// 親ウィンドウが「フォントメニューの設定」ダイアログの場合は
					if (my::get_window_text(parent) == hive.fonts.setting_dialog_name)
						return std::make_shared<aviutl2::font_setting::ListViewRenderer>();
				}

				return std::make_shared<ListViewRenderer>();
			}
			if (class_name == WC_TREEVIEW)
			{
				auto instance = (HINSTANCE)::GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
				auto ExplorerFrame = ::GetModuleHandleW(L"ExplorerFrame.dll");

				// エクスプローラのツリービューの場合は
				if (instance == ExplorerFrame)
				{
					// ウィンドウの縁にボーダーを追加します。
					my::modify_style(hwnd, 0, WS_BORDER);
//					my::modify_ex_style(hwnd, 0, WS_EX_CLIENTEDGE);
					::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
						SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
				}

				return std::make_shared<TreeViewRenderer>();
			}

			if (class_name == TOOLBARCLASSNAME) return std::make_shared<ToolBarRenderer>();

			// aviutl2のメインウィンドウです。
			if (class_name == L"aviutl2Manager") return std::make_shared<AviUtl2Renderer>();

			if (hive.comdlg32_visible_count)
			{
				// ファイル選択ダイアログのリストビューなどの親ウィンドウです。
//				if (class_name == L"DirectUIHWND") return std::make_shared<comdlg32::DirectUIHWNDRenderer>();

				// DirectUIHWNDの親ウィンドウです。
				if (class_name == L"DUIViewWndClassName") return std::make_shared<comdlg32::DUIViewRenderer>();

				// ファイル選択ダイアログのツリービューの親ウィンドウです。
				if (class_name == L"NamespaceTreeControl") return std::make_shared<Renderer>();

				// コマンドモジュールがドロップダウン表示するポップアップメニューのようなウィンドウです。
//				if (class_name == L"ViewControlClass") return std::make_shared<Renderer>();

				// ファイル選択ダイアログのリストビューの場合は
				if (class_name == L"SHELLDLL_DefView")
				{
#if 0
					auto parent = ::GetParent(hwnd);
					auto parent_class_name = my::get_class_name(parent);

					if (parent_class_name == L"#32770")
#endif
					{
						// ウィンドウの縁にボーダーを追加します。
						my::modify_style(hwnd, 0, WS_BORDER);
//						my::modify_ex_style(hwnd, 0, WS_EX_CLIENTEDGE);
						::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
							SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
					}
				}

				// フォルダ選択ダイアログのツリービューの親ウィンドウです。
				if (class_name == L"SHBrowseForFolder ShellNameSpace Control") return std::make_shared<Renderer>();

				// 「MP4出力」の設定ダイアログです。
				if (class_name == L"MP4ExporterConfig") return std::make_shared<mp4_exporter::DialogRenderer>();

				// 「拡張 x264 出力(GUI) Ex」の設定ダイアログです。
				{
					//
					// この関数は指定文字列から始まる場合はTRUEを返します。
					//
					const auto starts_with = [](LPCWSTR lhs, LPCWSTR rhs) {
						return ::StrStrIW(lhs, rhs) == lhs;
					};

					// 検索対象のクラス名のプレフィックスです。
					const auto prefix = std::wstring(L"WindowsForms10.");

					// クラス名がプレフィックスから始まる場合は
					if (starts_with(class_name.c_str(), prefix.c_str()))
					{
						// クラス名のプレフィックス以降を取得します。
						auto p = &class_name[prefix.size()];

						if (starts_with(p, L"Window"))
						{
							if (my::get_style(hwnd) & WS_CAPTION)
								return std::make_shared<rigaya::DialogRenderer>();
							else
								return std::make_shared<DialogRenderer>();
						}

						if (starts_with(p, WC_BUTTON)) return std::make_shared<ButtonRenderer>();
						if (starts_with(p, WC_EDIT)) return std::make_shared<EditBoxRenderer>();
						if (starts_with(p, TRACKBAR_CLASS)) return std::make_shared<TrackBarRenderer>();
					}
				}
			}

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
