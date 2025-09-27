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
			set_check(IDC_JD_USE_D2D, hive.jd.use_d2d);
			set_check(IDC_JD_EXCLUDE_COMDLG32, hive.jd.exclude_comdlg32);

			set_check(IDC_SLIMBAR_FLAG_USE, hive.slimbar.flag_use);
			set_text(IDC_SLIMBAR_TITLE_FORMAT, hive.slimbar.title_format);

			set_check(IDC_ROUND_FLAG_USE, hive.round.flag_use);
			set_int(IDC_ROUND_SIZE, hive.round.size);

			set_check(IDC_BORDER_FLAG_3D_EDGE, hive.border.flag_3d_edge);
			set_int(IDC_BORDER_WIDTH, hive.border.width);

			set_check(IDC_GRADIENT_FLAG_USE, hive.gradient.flag_use);
			set_check(IDC_GRADIENT_FLAG_END_ALPHA, hive.gradient.flag_end_alpha);
			set_int(IDC_GRADIENT_END_ALPHA, hive.gradient.end_alpha);

			set_check(IDC_SHADOW_FLAG_USE, hive.shadow.flag_use);
			set_check(IDC_SHADOW_FLAG_BLUR, hive.shadow.flag_blur);
			set_int(IDC_SHADOW_OFFSET_X, hive.shadow.offset.x);
			set_int(IDC_SHADOW_OFFSET_Y, hive.shadow.offset.y);
			set_int(IDC_SHADOW_SIZE, hive.shadow.size);

			set_check(IDC_SCROLLBAR_ARROW_AS_BUTTON, hive.scrollbar.arrow_as_button);
			set_check(IDC_SCROLLBAR_HAS_GRIPPER, hive.scrollbar.has_gripper);
			set_int(IDC_SCROLLBAR_REDUCTION, hive.scrollbar.reduction);

			set_check(IDC_ETC_DEFAULT_MOUSE_ACTIVATE, hive.etc.default_mouse_activate);

			set_text(IDC_FONTS_SAMPLE_TEXT_FORMAT, hive.fonts.sample_text_format);
			set_int(IDC_FONTS_ITEM_HEIGHT, hive.fonts.item_height);
			set_int(IDC_FONTS_FONT_HEIGHT, hive.fonts.font_height);
			set_int(IDC_FONTS_WINDOW_WIDTH, hive.fonts.window_size.cx);
			set_int(IDC_FONTS_WINDOW_HEIGHT, hive.fonts.window_size.cy);
			set_check(IDC_FONTS_USE_ON_MENU, hive.fonts.use_on_menu);
			set_check(IDC_FONTS_USE_ON_LISTBOX, hive.fonts.use_on_listbox);
			set_check(IDC_FONTS_USE_ON_LISTVIEW, hive.fonts.use_on_listview);

			set_text(IDC_DIALOG_NAME_NEW_PROJECT, hive.dialog_name.new_project);
			set_text(IDC_DIALOG_NAME_NEW_SCENE, hive.dialog_name.new_scene);
			set_text(IDC_DIALOG_NAME_SET_SCENE, hive.dialog_name.set_scene);
			set_text(IDC_DIALOG_NAME_SET_LAYER_NAME, hive.dialog_name.set_layer_name);
			set_text(IDC_DIALOG_NAME_SET_FONT_MENU, hive.dialog_name.set_font_menu);

			return TRUE;
		}

		//
		// コントロールでコンフィグを更新します。
		//
		BOOL from_ui(BOOL redraw)
		{
			MY_TRACE_FUNC("{/}", redraw);

			if (is_locked()) return FALSE;

//			get_text(IDC_JD_STYLE_FILE_NAME, hive.jd.style_file_name);
			get_check(IDC_JD_USE_D2D, hive.jd.use_d2d);
			get_check(IDC_JD_EXCLUDE_COMDLG32, hive.jd.exclude_comdlg32);

			get_check(IDC_SLIMBAR_FLAG_USE, hive.slimbar.flag_use);
			get_text(IDC_SLIMBAR_TITLE_FORMAT, hive.slimbar.title_format);

			get_check(IDC_ROUND_FLAG_USE, hive.round.flag_use);
			get_int(IDC_ROUND_SIZE, hive.round.size);

			get_check(IDC_BORDER_FLAG_3D_EDGE, hive.border.flag_3d_edge);
			get_int(IDC_BORDER_WIDTH, hive.border.width);

			get_check(IDC_GRADIENT_FLAG_USE, hive.gradient.flag_use);
			get_check(IDC_GRADIENT_FLAG_END_ALPHA, hive.gradient.flag_end_alpha);
			get_int(IDC_GRADIENT_END_ALPHA, hive.gradient.end_alpha);

			get_check(IDC_SHADOW_FLAG_USE, hive.shadow.flag_use);
			get_check(IDC_SHADOW_FLAG_BLUR, hive.shadow.flag_blur);
			get_int(IDC_SHADOW_OFFSET_X, hive.shadow.offset.x);
			get_int(IDC_SHADOW_OFFSET_Y, hive.shadow.offset.y);
			get_int(IDC_SHADOW_SIZE, hive.shadow.size);

			get_check(IDC_SCROLLBAR_ARROW_AS_BUTTON, hive.scrollbar.arrow_as_button);
			get_check(IDC_SCROLLBAR_HAS_GRIPPER, hive.scrollbar.has_gripper);
			get_int(IDC_SCROLLBAR_REDUCTION, hive.scrollbar.reduction);

			get_check(IDC_ETC_DEFAULT_MOUSE_ACTIVATE, hive.etc.default_mouse_activate);

			get_text(IDC_FONTS_SAMPLE_TEXT_FORMAT, hive.fonts.sample_text_format);
			get_int(IDC_FONTS_ITEM_HEIGHT, hive.fonts.item_height);
			get_int(IDC_FONTS_FONT_HEIGHT, hive.fonts.font_height);
			get_int(IDC_FONTS_WINDOW_WIDTH, hive.fonts.window_size.cx);
			get_int(IDC_FONTS_WINDOW_HEIGHT, hive.fonts.window_size.cy);
			get_check(IDC_FONTS_USE_ON_MENU, hive.fonts.use_on_menu);
			get_check(IDC_FONTS_USE_ON_LISTBOX, hive.fonts.use_on_listbox);
			get_check(IDC_FONTS_USE_ON_LISTVIEW, hive.fonts.use_on_listview);

			get_text(IDC_DIALOG_NAME_NEW_PROJECT, hive.dialog_name.new_project);
			get_text(IDC_DIALOG_NAME_NEW_SCENE, hive.dialog_name.new_scene);
			get_text(IDC_DIALOG_NAME_SET_SCENE, hive.dialog_name.set_scene);
			get_text(IDC_DIALOG_NAME_SET_LAYER_NAME, hive.dialog_name.set_layer_name);
			get_text(IDC_DIALOG_NAME_SET_FONT_MENU, hive.dialog_name.set_font_menu);

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
					case IDC_ETC_DEFAULT_MOUSE_ACTIVATE:
					case IDC_FONTS_USE_ON_MENU:
					case IDC_FONTS_USE_ON_LISTBOX:
					case IDC_FONTS_USE_ON_LISTVIEW:
						{
							from_ui(FALSE);

							break;
						}
					case IDC_SLIMBAR_FLAG_USE:
						{
							from_ui(FALSE);

							app->apply_slim_menubar();

							break;
						}
					case IDC_JD_USE_D2D:
					case IDC_ROUND_FLAG_USE:
					case IDC_BORDER_FLAG_3D_EDGE:
					case IDC_GRADIENT_FLAG_USE:
					case IDC_GRADIENT_FLAG_END_ALPHA:
					case IDC_SHADOW_FLAG_USE:
					case IDC_SHADOW_FLAG_BLUR:
					case IDC_SCROLLBAR_ARROW_AS_BUTTON:
					case IDC_SCROLLBAR_HAS_GRIPPER:
						{
							from_ui(TRUE);

							break;
						}
					case IDC_SLIMBAR_TITLE_FORMAT:
						{
							if (hive.slimbar.flag_use)
							{
								if (code == EN_UPDATE)
									from_ui(TRUE);
							}

							break;
						}
					case IDC_ROUND_SIZE:
					case IDC_BORDER_WIDTH:
					case IDC_GRADIENT_END_ALPHA:
					case IDC_SHADOW_OFFSET_X:
					case IDC_SHADOW_OFFSET_Y:
					case IDC_SHADOW_SIZE:
					case IDC_SCROLLBAR_REDUCTION:
						{
							if (code == EN_UPDATE)
								from_ui(TRUE);

							break;
						}
					case IDC_FONTS_WINDOW_WIDTH:
					case IDC_FONTS_WINDOW_HEIGHT:
					case IDC_FONTS_ITEM_HEIGHT:
					case IDC_FONTS_FONT_HEIGHT:
					case IDC_FONTS_SAMPLE_TEXT_FORMAT:
					case IDC_DIALOG_NAME_NEW_PROJECT:
					case IDC_DIALOG_NAME_NEW_SCENE:
					case IDC_DIALOG_NAME_SET_SCENE:
					case IDC_DIALOG_NAME_SET_LAYER_NAME:
					case IDC_DIALOG_NAME_SET_FONT_MENU:
						{
							if (code == EN_UPDATE)
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
						case IDC_BORDER_WIDTH:
						case IDC_SHADOW_SIZE:
							{
								auto value = get_int(edit_id);
								value += (nm->iDelta > 0) ? -1 : +1;
								value = std::clamp(value, 0, +100);
								set_int(edit_id, value);
								break;
							}
						case IDC_ROUND_SIZE:
						case IDC_GRADIENT_END_ALPHA:
						case IDC_SCROLLBAR_REDUCTION:
							{
								auto value = get_int(edit_id);
								value += (nm->iDelta > 0) ? -10 : +10;
								value = std::clamp(value, 0, +100);
								set_int(edit_id, value);
								break;
							}
						case IDC_SHADOW_OFFSET_X:
						case IDC_SHADOW_OFFSET_Y:
							{
								auto value = get_int(edit_id);
								value += (nm->iDelta > 0) ? -10 : +10;
								value = std::clamp(value, -100, +100);
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
