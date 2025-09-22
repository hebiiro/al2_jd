#pragma once

namespace apn::dark
{
	//
	// このクラスはテスト用ダイアログです。
	//
	struct TestDialog : my::Dialog
	{
		//
		// ダイアログを表示します。
		//
		int do_modal()
		{
			auto result = create(hive.instance, MAKEINTRESOURCE(IDD_TEST_DIALOG), hive.theme_window);

			int nb_items = 20;

			{
				auto control = ::GetDlgItem(*this, IDC_DROP_DOWN);
				for (auto i = 0; i < nb_items; i++)
					my::combobox::add_text(control, my::format(L"drop down {/}", i));
				::SendMessage(control, CB_SETCURSEL, 0, 0);
			}

			{
				auto control = ::GetDlgItem(*this, IDC_DROP_DOWN_LIST);
				for (auto i = 0; i < nb_items; i++)
					my::combobox::add_text(control, my::format(L"drop down list {/}", i));
				::SendMessage(control, CB_SETCURSEL, 0, 0);
			}

			{
				auto control = ::GetDlgItem(*this, IDC_EDITBOX);
				::SetWindowText(control, L"エディットボックス");
			}

			{
				auto control = ::GetDlgItem(*this, IDC_LISTBOX);
				for (auto i = 0; i < nb_items; i++)
					::SendMessage(control, LB_ADDSTRING, 0, (LPARAM)my::format(L"list box {/}", i).c_str());
				::SendMessage(control, LB_SETCURSEL, 0, 0);
			}

			{
				auto control = ::GetDlgItem(*this, IDC_CHECKBOX_2);
				::SendMessage(control, BM_SETCHECK, BST_CHECKED, 0);
			}

			{
				auto control = ::GetDlgItem(*this, IDC_RADIO_2);
				::SendMessage(control, BM_SETCHECK, BST_CHECKED, 0);
			}

			{
				auto control = ::GetDlgItem(*this, IDC_LIST_VIEW);

				{
					LVCOLUMN lvc = {};
					lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

					lvc.iSubItem = 0;
					lvc.pszText = (LPWSTR)L"col 1";
					lvc.cx = 150;
					lvc.fmt = LVCFMT_LEFT;
					ListView_InsertColumn(control, lvc.iSubItem, &lvc);

					lvc.iSubItem = 1;
					lvc.pszText = (LPWSTR)L"col 2";
					lvc.cx = 100;
					lvc.fmt = LVCFMT_RIGHT;
					ListView_InsertColumn(control, lvc.iSubItem, &lvc);
				}

				LVITEM lvi = {};
				lvi.mask = LVIF_TEXT;

				for (auto i = 0; i < nb_items; i++)
				{
					auto text = my::format(L"list view {/}", i);
					lvi.iItem = i;
					lvi.iSubItem = 0;
					lvi.pszText = text.data();
					ListView_InsertItem(control, &lvi);

					auto sub_text = my::format(L"sub {/}", i);
					lvi.iItem = i;
					lvi.iSubItem = 1;
					lvi.pszText = sub_text.data();
					ListView_SetItem(control, &lvi);
				}
			}

			{
				auto control = ::GetDlgItem(*this, IDC_TAB);

				for (auto i = 0; i < 10; i++)
				{
					auto text = my::format(L"tab {/}", i);

					TCITEM tie = {};
					tie.mask = TCIF_TEXT;
					tie.pszText = text.data();
					TabCtrl_InsertItem(control, i, &tie);
				}
			}

			{
				auto hTree = ::GetDlgItem(*this, IDC_TREE_VIEW);

				HTREEITEM hParent1, hParent2, hParent3, hChild1, hChild2;
				TV_INSERTSTRUCT tv = {};

				tv.hInsertAfter = TVI_LAST;
				tv.item.mask = TVIF_TEXT;
				tv.hParent = TVI_ROOT;
				tv.item.pszText = (LPWSTR)L"粂井";
				hParent1 = TreeView_InsertItem(hTree, &tv);

				tv.item.pszText = (LPWSTR)L"田中";
				hParent2 = TreeView_InsertItem(hTree, &tv);

				tv.item.pszText = (LPWSTR)L"佐藤";
				hParent3 = TreeView_InsertItem(hTree, &tv);

				tv.hParent = hParent1;
				tv.item.pszText = (LPWSTR)L"康孝";
				hChild1 = TreeView_InsertItem(hTree, &tv);
				tv.item.pszText = (LPWSTR)L"ひとみ";
				hChild2 = TreeView_InsertItem(hTree, &tv);
				tv.hParent = hChild1;
				tv.item.pszText = (LPWSTR)L"志麻";
				TreeView_InsertItem(hTree, &tv);
				tv.hParent = hChild1;
				tv.item.pszText = (LPWSTR)L"櫻都";
				TreeView_InsertItem(hTree, &tv);
				tv.hParent = hParent2;
				tv.item.pszText = (LPWSTR)L"マイケル";
				TreeView_InsertItem(hTree, &tv);
				tv.hParent = hParent3;
				tv.item.pszText = (LPWSTR)L"パトリシア";
				TreeView_InsertItem(hTree, &tv);

				TreeView_Expand(hTree, hParent1, TVE_EXPAND);
				TreeView_Expand(hTree, hParent2, TVE_EXPAND);
				TreeView_Expand(hTree, hChild1, TVE_EXPAND);
			}

			return __super::do_modal();
		}

		//
		// ダイアログプロシージャです。
		//
		virtual INT_PTR on_dlg_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			switch (message)
			{
			case WM_COMMAND:
				{
					auto id = LOWORD(wParam);
					auto code = HIWORD(wParam);
					auto control = (HWND)lParam;

					break;
				}
			}

			return __super::on_dlg_proc(hwnd, message, wParam, lParam);
		}
	};
}
