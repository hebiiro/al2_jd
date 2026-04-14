#pragma once

namespace apn::dark::kuro::gdi::aviutl2::new_project
{
	struct dialog_renderer_t : gdi::dialog_renderer_t, my::lockable_t
	{
		//
		// このクラスは既存のコントロールです。
		//
		struct control_t {
			HWND hwnd;
			RECT rc;
			operator HWND() const { return hwnd; }
		};

		//
		// 既存のコントロールのコレクションです。
		//
		std::vector<control_t> controls;

		//
		// 既存のコントロールです。
		//
		struct readymade_t {
			using element_t = const control_t*;
			element_t name_stc, name;
			element_t video_size_stc, video_width, video_height;
			element_t video_rate_stc, video_rate;
			element_t audio_rate_stc, audio_rate;
			element_t background_color_stc, background_color, background_color_picker;
			element_t horz_border;
			element_t output_size_stc, output_width, output_height;
			element_t output_audio_stc, output_audio;
			element_t tip_stc;
			element_t ok;
		} readymade = {};

		//
		// 後付けのコントロールです。
		//
		struct retrofit_t {
			HWND preset_stc; // "プリセット"のスタティックコントロールです。
			HWND preset; // "プリセット"のコンボボックスです。
			HWND swap_video_size; // "縦横反転"のチェックボックスです。
			HWND name_preset; // "名前プリセット"のコンボボックスです。
			HWND video_size_preset; // "映像サイズプリセット"のコンボボックスです。
			HWND video_rate_preset; // "映像レートプリセット"のコンボボックスです。
			HWND audio_rate_preset; // "音声レートプリセット"のコンボボックスです。
			HWND cancel; // キャンセルボタンです。
		} retrofit = {};

		//
		// 指定されたテキストを翻訳して返します。
		//
		inline static LPCWSTR tr(LPCWSTR text) { return text; }

		//
		// 指定されたテキストを翻訳して返します。
		//
		inline static LPCWSTR tr(LPCWSTR category, LPCWSTR text) { return text; }

		//
		// コントロール配列を初期化します。
		//
		BOOL init_controls(HWND hwnd)
		{
			MY_TRACE_FUNC("{/hex}", hwnd);

			// 既存のコントロールを列挙します。
			::EnumChildWindows(hwnd,
				[](HWND child, LPARAM l_param) -> BOOL
			{
				MY_TRACE_HWND(child);

				// thisポインタを取得します。
				auto p = (dialog_renderer_t*)l_param;

				// 既存のコントロールの矩形を取得します。
				auto rc = my::get_window_rect(child);
				my::map_window_points(nullptr, ::GetParent(child), &rc);

				// 既存のコントロールをコレクションに追加します。
				p->controls.emplace_back(child, rc);

				return TRUE;
			},
			(LPARAM)this);

			// 既存のコントロールの数が無効の場合は失敗します。
			if (controls.size() < 13) return FALSE;

			{
				// コントロールのインデックスです。
				auto index = size_t {};

				// 基準となるコントロールを取得します。
				const auto& control = controls[2];

				// 基準となるコントロールのクラス名を取得します。
				auto class_name = my::get_class_name(control);

				// 基準となるコントロールがエディットボックスの場合は
				if (::lstrcmpiW(class_name.c_str(), WC_EDITW) == 0)
				{
					// 「プロジェクトを新規作成」または(出力時の)「シーンの設定」ダイアログです。
				}
				else
				{
					// 「シーンを新規作成」または(通常の)「シーンの設定」ダイアログです。

					readymade.name_stc = &controls[index++];
					readymade.name = &controls[index++];
				}

				readymade.video_size_stc = &controls[index++];
				readymade.video_width = &controls[index++];
				readymade.video_height = &controls[index++];
				readymade.video_rate_stc = &controls[index++];
				readymade.video_rate = &controls[index++];
				readymade.audio_rate_stc = &controls[index++];
				readymade.audio_rate = &controls[index++];
				readymade.background_color_stc = &controls[index++];
				readymade.background_color = &controls[index++];
				readymade.background_color_picker = &controls[index++];
				readymade.horz_border = &controls[index++];
				readymade.output_size_stc = &controls[index++];
				readymade.output_width = &controls[index++];
				readymade.output_height = &controls[index++];
				readymade.output_audio_stc = &controls[index++];
				readymade.output_audio = &controls[index++];
				readymade.tip_stc = &controls[index++];
				readymade.ok = &controls.back();
			}

			return TRUE;
		}

		//
		// コントロールを再配置します。
		//
		BOOL arrange_controls(HWND hwnd)
		{
			MY_TRACE_FUNC("{/hex}", hwnd);

			// クライアント矩形を取得します。
			auto client_rc = my::get_client_rect(hwnd);

			// 基準サイズを取得します。
			auto base = SIZE {
				my::get_width(readymade.video_rate->rc),
				my::get_height(readymade.video_rate->rc),
			};

			// 余白サイズを取得します。
			auto space = SIZE {
				readymade.video_height->rc.left - readymade.video_width->rc.right,
				readymade.video_rate->rc.top - readymade.video_width->rc.bottom,
			};

			// 移動予定のオフセットサイズを取得します。
			auto offset = SIZE {
				base.cx + space.cx,
				base.cy + space.cy,
			};

			// 追加のコントロールを作成します。
			{
				// コントロールのフォントです。
				auto font = (HFONT)::SendMessage(*readymade.ok, WM_GETFONT, 0, 0);

				// 追加コントロールの基準X座標です。
				auto x = readymade.video_height->rc.right + space.cx;

				{
					// Y座標の基準となるコントロール(一番上のコントロール)です。
					auto base_control = readymade.name_stc ? readymade.name_stc : readymade.video_size_stc;

					// 追加コントロールの基準Y座標です。
					auto y = base_control->rc.top;

					//
					// この関数はコントロールを作成します。
					//
					const auto create_control = [&](
						LPCWSTR class_name, LPCWSTR window_name,
						DWORD style, int x, int y, int w, int h)
					{
						style |= WS_VISIBLE | WS_CHILD;

						auto control = ::CreateWindowExW(
							0, class_name, window_name, style, x, y, w, h,
							hwnd, nullptr, hive.instance, nullptr);
						::SendMessage(control, WM_SETFONT, (WPARAM)font, TRUE);
						return control;
					};

					//
					// この関数はコンボボックスを作成します。
					//
					const auto create_combobox = [&](int x, int y, int w, int h)
					{
						return create_control(
							WC_COMBOBOXW, nullptr,
							WS_VSCROLL |
							CBS_DISABLENOSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
							x, y, w, h * 20);
					};

					retrofit.preset_stc = create_control(
						WC_STATICW, tr(L"プリセット"), SS_CENTERIMAGE, readymade.video_size_stc->rc.left, y, base.cx, base.cy);
					retrofit.preset = create_combobox(readymade.video_width->rc.left, y, base.cx, base.cy);
					retrofit.swap_video_size = create_control(
						WC_BUTTONW, tr(L"縦横反転"), BS_AUTOCHECKBOX, x, y, base.cx, base.cy);
					if (readymade.name_stc)
						retrofit.name_preset = create_combobox(x, readymade.name_stc->rc.top + offset.cy, base.cx, base.cy);
					retrofit.video_size_preset = create_combobox(x, readymade.video_size_stc->rc.top + offset.cy, base.cx, base.cy);
					retrofit.video_rate_preset = create_combobox(x, readymade.video_rate_stc->rc.top + offset.cy, base.cx, base.cy);
					retrofit.audio_rate_preset = create_combobox(x, readymade.audio_rate_stc->rc.top + offset.cy, base.cx, base.cy);

					{
						auto w = my::get_width(readymade.ok->rc);
						auto h = my::get_height(readymade.ok->rc);
						auto x = client_rc.right + offset.cx - (w + space.cx * 2);
						auto y = client_rc.bottom + offset.cy - (h + space.cy * 2);

						retrofit.cancel = create_control(WC_BUTTONW, tr(L"Dialog", L"キャンセル"), BS_PUSHBUTTON, x, y, w, h);
					}

					//
					// この関数はコンボボックスをカスタマイズします。
					//
					const auto customize_combobox = [](HWND combobox) {
						auto rc = my::get_window_rect(combobox);
						auto dropped_width = my::get_width(rc) * 2;
						::SendMessage(combobox, CB_SETDROPPEDWIDTH, dropped_width, 0);
					};

					{
						// プロジェクトプリセットのコンボボックスを初期化します。
						auto combobox = retrofit.preset;
						customize_combobox(combobox);
						for (const auto& preset : hive.presets.project_collection)
						{
							if (preset.display_name.empty())
							{
								if (preset.scene_name.length())
								{
									my::combobox::add_text(combobox,
										my::format(L"{/}, {/} x {/} px, {/} fps, {/} Hz",
											preset.scene_name,
											preset.video_width, preset.video_height,
											preset.video_rate, preset.audio_rate).c_str());
								}
								else
								{
									my::combobox::add_text(combobox,
										my::format(L"{/} x {/} px, {/} fps, {/} Hz",
											preset.video_width, preset.video_height,
											preset.video_rate, preset.audio_rate).c_str());
								}
							}
							else
							{
								my::combobox::add_text(combobox, preset.display_name.c_str());
							}
						}
					}

					{
						// 名前プリセットのコンボボックスを初期化します。
						auto combobox = retrofit.name_preset;
						customize_combobox(combobox);
						for (const auto& preset : hive.presets.scene_name_collection)
							my::combobox::add_text(combobox, preset.scene_name.c_str());
					}

					{
						// 映像サイズプリセットのコンボボックスを初期化します。
						auto combobox = retrofit.video_size_preset;
						customize_combobox(combobox);
						for (const auto& preset : hive.presets.video_size_collection)
						{
							if (preset.display_name.empty())
							{
								my::combobox::add_text(combobox,
									my::format(L"{/} x {/}", preset.video_width, preset.video_height).c_str());
							}
							else
							{
								my::combobox::add_text(combobox, preset.display_name.c_str());
							}
						}
					}

					{
						// 映像レートプリセットのコンボボックスを初期化します。
						auto combobox = retrofit.video_rate_preset;
						customize_combobox(combobox);
						for (const auto& preset : hive.presets.video_rate_collection)
						{
							if (preset.display_name.empty())
							{
								my::combobox::add_text(combobox,
									my::format(L"{/}fps", preset.video_rate).c_str());
							}
							else
							{
								my::combobox::add_text(combobox, preset.display_name.c_str());
							}
						}
					}

					{
						// 音声レートのコンボボックスを初期化します。
						auto combobox = retrofit.audio_rate_preset;
						customize_combobox(combobox);
						for (const auto& preset : hive.presets.audio_rate_collection)
						{
							if (preset.display_name.empty())
							{
								my::combobox::add_text(combobox,
									my::format(L"{/}Hz", preset.audio_rate).c_str());
							}
							else
							{
								my::combobox::add_text(combobox, preset.display_name.c_str());
							}
						}
					}
				}
			}

			// ダイアログのサイズを変更します。
			{
				WINDOWPLACEMENT wp = { sizeof(wp) };
				::GetWindowPlacement(hwnd, &wp);
				wp.rcNormalPosition.right += offset.cx;
				wp.rcNormalPosition.bottom += offset.cy;
				::SetWindowPlacement(hwnd, &wp);
			}

			// コントロールの位置を変更します。
			{
				my::DeferWindowPos dwp;

				for (const auto& control : controls)
				{
					auto rc = control.rc;

					if (&control == readymade.ok)
					{
						auto w = my::get_width(rc);
						auto h = my::get_height(rc);

						rc.left = client_rc.right + offset.cx - (w + space.cx * 2) * 2;
						rc.top = client_rc.bottom + offset.cy - (h + space.cy * 2);
						rc.right = rc.left + w;
						rc.bottom = rc.top + h;
					}
					else
					{
						::OffsetRect(&rc, 0, offset.cy);
					}

					dwp.set_window_pos(control, nullptr, &rc, SWP_NOZORDER);
				}
			}

			return TRUE;
		}

		//
		// ウィンドウメッセージを処理します。
		//
		virtual LRESULT on_subclass_proc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/hex}, {/hex}", hwnd, my::message_to_string(message), w_param, l_param);

			switch (message)
			{
			case WM_INITDIALOG:
				{
					// スコープ終了時(デフォルト処理の後)に実行します。
					my::scope_exit scope_exit([&]()
					{
						my::locker_t locker(this);

						if (init_controls(hwnd))
							arrange_controls(hwnd);
					});

					return __super::on_subclass_proc(hwnd, message, w_param, l_param);
				}
			case WM_COMMAND:
				{
					//
					// この関数はコントロールのテキストを変更します。
					//
					const auto set_control_text = [this](const control_t* control, const std::wstring& text)
					{
						// コントロールとテキストがどちらも有効の場合は
						if (control && ::IsWindowEnabled(*control) && text.length())
						{
							// コントロールのテキストを変更します。
							::SetWindowTextW(*control, text.c_str());
						}
					};

					//
					// この関数は映像サイズのコントロールを返します。
					//
					const auto get_video_size_controls = [this]()
					{
						// 映像サイズのコントロールを取得します。
						auto video_width = readymade.video_width;
						auto video_height = readymade.video_height;

						// 映像サイズのコントロールが無効状態の場合は
						if (!::IsWindowEnabled(*video_width) || !::IsWindowEnabled(*video_height))
						{
							// 出力サイズのコントロールを取得します。
							video_width = readymade.output_width;
							video_height = readymade.output_height;
						}

						// 縦横反転にチェックが入っている場合はコントロールを入れ替えます。
						return (::SendMessage(retrofit.swap_video_size, BM_GETCHECK, 0, 0) == BST_CHECKED) ?
							std::make_pair(video_height, video_width) : std::make_pair(video_width, video_height);
					};

					// ロックされている場合は何もしません。
					if (is_locked()) break;

					my::locker_t locker(this);

//					auto control_id = LOWORD(w_param);
					auto code = HIWORD(w_param);
					auto control = (HWND)l_param;

					// コントロールが無効の場合は何もしません。
					if (!control) break;

					if (control == retrofit.preset)
					{
						if (code != CBN_SELCHANGE) break;

						// コンボボックスから選択されているプリセットのインデックスを取得します。
						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.video_size_collection.size()) break;

						// テーブルからプロジェクトのプリセットを取得します。
						const auto& preset = hive.presets.project_collection[index];

						// プロジェクトのプリセットを適用します。
						{
							auto video_size = get_video_size_controls();

							set_control_text(video_size.first, preset.video_width);
							set_control_text(video_size.second, preset.video_height);

							set_control_text(readymade.video_rate, preset.video_rate);
							set_control_text(readymade.audio_rate, preset.audio_rate);

							if (readymade.name)
								set_control_text(readymade.name, preset.scene_name);
						}
					}
					else if (control == retrofit.name_preset)
					{
						if (code != CBN_SELCHANGE) break;

						// コンボボックスから選択されているプリセットのインデックスを取得します。
						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.scene_name_collection.size()) break;

						// テーブルからシーン名のプリセットを取得します。
						const auto& preset = hive.presets.scene_name_collection[index];

						// シーン名のプリセットを適用します。
						{
							if (readymade.name)
								set_control_text(readymade.name, preset.scene_name);
						}
					}
					else if (control == retrofit.video_size_preset)
					{
						if (code != CBN_SELCHANGE) break;

						// コンボボックスから選択されているプリセットのインデックスを取得します。
						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.video_size_collection.size()) break;

						// テーブルから映像サイズのプリセットを取得します。
						const auto& preset = hive.presets.video_size_collection[index];

						// 映像サイズのプリセットを適用します。
						{
							auto video_size = get_video_size_controls();

							set_control_text(video_size.first, preset.video_width);
							set_control_text(video_size.second, preset.video_height);
						}
					}
					else if (control == retrofit.video_rate_preset)
					{
						if (code != CBN_SELCHANGE) break;

						// コンボボックスから選択されているプリセットのインデックスを取得します。
						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.video_rate_collection.size()) break;

						// テーブルから映像レートのプリセットを取得します。
						const auto& preset = hive.presets.video_rate_collection[index];

						// 映像レートのプリセットを適用します。
						{
							set_control_text(readymade.video_rate, preset.video_rate);
						}
					}
					else if (control == retrofit.audio_rate_preset)
					{
						if (code != CBN_SELCHANGE) break;

						// コンボボックスから選択されているプリセットのインデックスを取得します。
						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.audio_rate_collection.size()) break;

						// テーブルから音声レートのプリセットを取得します。
						const auto& preset = hive.presets.audio_rate_collection[index];

						// 音声レートのプリセットを適用します。
						{
							set_control_text(readymade.audio_rate, preset.audio_rate);
						}
					}
					else if (control == retrofit.swap_video_size)
					{
						if (code != BN_CLICKED) break;

						// 映像サイズの縦横を入れ替えます。
						{
							auto video_size = get_video_size_controls();

							auto first = my::get_window_text(*video_size.first);
							auto second = my::get_window_text(*video_size.second);

							::SetWindowTextW(*video_size.first, second.c_str());
							::SetWindowTextW(*video_size.second, first.c_str());
						}
					}
					else if (control == retrofit.cancel)
					{
						MY_TRACE("キャンセルボタンがクリックされました\n");

						// エスケープキーをエミュレートします。
						::PostMessage(hwnd, WM_KEYDOWN, VK_ESCAPE, 0);
					}

					break;
				}
			}

			return __super::on_subclass_proc(hwnd, message, w_param, l_param);
		}
	};
}
