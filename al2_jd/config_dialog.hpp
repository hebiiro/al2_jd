#pragma once

namespace apn::dark
{
	//
	// このクラスはコンフィグダイアログです。
	//
	inline struct ConfigDialog : my::Dialog, Lockable
	{
		virtual void on_update_controls() {}
		virtual void on_update_config() {}
		virtual void on_init_dialog() {}
		virtual void on_command(UINT code, UINT id, HWND control) {}

		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			{
				// 初期化中にエディットボックスがコマンドを発行してしまうので、
				// それを防ぐためにロックしておきます。
				Locker locker(this);

				if (!__super::create(
					hive.instance,
					MAKEINTRESOURCE(IDD_CONFIG_DIALOG),
					hive.theme_window))
				{
					hive.message_box(L"コンフィグダイアログの作成に失敗しました");

					return FALSE;
				}
			}

			return app->read_config();
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return __super::destroy();
		}

		//
		// コンフィグでコントロールを更新します。
		//
		BOOL to_ui()
		{
			MY_TRACE_FUNC("");

			if (is_locked()) return FALSE;

			Locker locker(this);

			set_text(IDC_JD_STYLE_FILE_NAME, hive.jd.style_file_name);

			set_check(IDC_JD_EXCLUDE_COMDLG32, hive.jd.exclude_comdlg32);
			set_check(IDC_JD_SLIM_MENUBAR, hive.jd.slim_menubar);

			set_int(IDC_SCROLLBAR_REDUCTION, hive.scrollbar.reduction);
			set_check(IDC_SCROLLBAR_ARROW_AS_BUTTON, hive.scrollbar.arrow_as_button);
			set_check(IDC_SCROLLBAR_HAS_GRIPPER, hive.scrollbar.has_gripper);

			set_check(IDC_ETC_MAXIMIZE_AVIUTL2, hive.etc.maximize_aviutl2);
			set_check(IDC_ETC_OPEN_RECENT_PROJECT, hive.etc.open_recent_project);
			set_check(IDC_ETC_DEFAULT_MOUSE_ACTIVATE, hive.etc.default_mouse_activate);
			set_check(IDC_ETC_USE_RECENT_SETTING, hive.etc.use_recent_setting);

			set_text(IDC_FONTS_SAMPLE_TEXT_FORMAT, hive.fonts.sample_text_format);
			set_int(IDC_FONTS_ITEM_HEIGHT, hive.fonts.item_height);
			set_int(IDC_FONTS_FONT_HEIGHT, hive.fonts.font_height);
			set_int(IDC_FONTS_WINDOW_WIDTH, hive.fonts.window_size.cx);
			set_int(IDC_FONTS_WINDOW_HEIGHT, hive.fonts.window_size.cy);
			set_check(IDC_FONTS_USE_ON_MENU, hive.fonts.use_on_menu);
			set_check(IDC_FONTS_USE_ON_LISTBOX, hive.fonts.use_on_listbox);
			set_check(IDC_FONTS_USE_ON_LISTVIEW, hive.fonts.use_on_listview);
			set_text(IDC_FONTS_SETTING_DIALOG_NAME, hive.fonts.setting_dialog_name);

			return TRUE;
		}

		//
		// コントロールでコンフィグを更新します。
		//
		BOOL from_ui(BOOL redraw)
		{
			MY_TRACE_FUNC("{/}", redraw);

			if (is_locked()) return FALSE;

			get_check(IDC_JD_EXCLUDE_COMDLG32, hive.jd.exclude_comdlg32);
			get_check(IDC_JD_SLIM_MENUBAR, hive.jd.slim_menubar);

			get_int(IDC_SCROLLBAR_REDUCTION, hive.scrollbar.reduction);
			get_check(IDC_SCROLLBAR_ARROW_AS_BUTTON, hive.scrollbar.arrow_as_button);
			get_check(IDC_SCROLLBAR_HAS_GRIPPER, hive.scrollbar.has_gripper);

			get_check(IDC_ETC_MAXIMIZE_AVIUTL2, hive.etc.maximize_aviutl2);
			get_check(IDC_ETC_OPEN_RECENT_PROJECT, hive.etc.open_recent_project);
			get_check(IDC_ETC_DEFAULT_MOUSE_ACTIVATE, hive.etc.default_mouse_activate);
			get_check(IDC_ETC_USE_RECENT_SETTING, hive.etc.use_recent_setting);

			get_text(IDC_FONTS_SAMPLE_TEXT_FORMAT, hive.fonts.sample_text_format);
			get_int(IDC_FONTS_ITEM_HEIGHT, hive.fonts.item_height);
			get_int(IDC_FONTS_FONT_HEIGHT, hive.fonts.font_height);
			get_int(IDC_FONTS_WINDOW_WIDTH, hive.fonts.window_size.cx);
			get_int(IDC_FONTS_WINDOW_HEIGHT, hive.fonts.window_size.cy);
			get_check(IDC_FONTS_USE_ON_MENU, hive.fonts.use_on_menu);
			get_check(IDC_FONTS_USE_ON_LISTBOX, hive.fonts.use_on_listbox);
			get_check(IDC_FONTS_USE_ON_LISTVIEW, hive.fonts.use_on_listview);
			get_text(IDC_FONTS_SETTING_DIALOG_NAME, hive.fonts.setting_dialog_name);

			if (redraw) app->redraw();

			return TRUE;
		}

		//
		// ウィンドウプロシージャです。
		//
		virtual LRESULT on_wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			switch (message)
			{
			case WM_COMMAND:
				{
					// ロックされている場合はWM_COMMANDを処理しません。
					if (is_locked()) break;

					auto id = LOWORD(wParam);
					auto code = HIWORD(wParam);
					auto control = (HWND)lParam;

					switch (id)
					{
					case IDC_JD_STYLE_FILE_NAME_REF:
						{
							// ユーザーが指定したスタイルファイルのパスを取得します。
							auto file_name = get_open_file_name(
								hwnd,
								L"スタイルファイルを選択",
								L"スタイルファイル (*.conf)\0*.conf\0"
								L"すべてのファイル (*.*)\0*.*\0",
								hive.jd.style_file_name.c_str());
							if (file_name.empty()) break;

							app->set_style_file_name(file_name);

							break;
						}
					case IDC_JD_EXCLUDE_COMDLG32:
						{
							from_ui(FALSE);

							break;
						}
					case IDC_JD_SLIM_MENUBAR:
						{
							from_ui(FALSE);

							app->apply_slim_menubar();

							break;
						}
					case IDC_SCROLLBAR_REDUCTION:
						{
							if (code == EN_UPDATE)
								from_ui(TRUE);

							break;
						}
					case IDC_SCROLLBAR_ARROW_AS_BUTTON:
					case IDC_SCROLLBAR_HAS_GRIPPER:
						{
							from_ui(TRUE);

							break;
						}
					case IDC_ETC_MAXIMIZE_AVIUTL2:
					case IDC_ETC_OPEN_RECENT_PROJECT:
					case IDC_ETC_DEFAULT_MOUSE_ACTIVATE:
					case IDC_ETC_USE_RECENT_SETTING:
						{
							from_ui(FALSE);

							break;
						}
					case IDC_FONTS_WINDOW_WIDTH:
					case IDC_FONTS_WINDOW_HEIGHT:
					case IDC_FONTS_ITEM_HEIGHT:
					case IDC_FONTS_FONT_HEIGHT:
					case IDC_FONTS_SAMPLE_TEXT_FORMAT:
					case IDC_FONTS_SETTING_DIALOG_NAME:
						{
							if (code == EN_UPDATE)
								from_ui(FALSE);

							break;
						}
					case IDC_FONTS_USE_ON_MENU:
					case IDC_FONTS_USE_ON_LISTBOX:
					case IDC_FONTS_USE_ON_LISTVIEW:
						{
							from_ui(FALSE);

							break;
						}
					}

					break;
				}
			case WM_NOTIFY:
				{
					auto header = (NMHDR*)lParam;
					if (header->code == UDN_DELTAPOS)
					{
						auto nm = (NMUPDOWN*)header;
						auto edit_id = (UINT)header->idFrom - 1;

						switch (edit_id)
						{
						case IDC_SCROLLBAR_REDUCTION:
							{
								auto value = get_int(edit_id);
								value += (nm->iDelta > 0) ? -10 : +10;
								value = std::clamp(value, 0, +100);
								set_int(edit_id, value);
								break;
							}
						}
					}

					return FALSE;
				}
			}

			return __super::on_wnd_proc(hwnd, message, wParam, lParam);
		}
	} config_dialog;
}
