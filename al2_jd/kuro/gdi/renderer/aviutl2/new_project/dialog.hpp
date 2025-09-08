#pragma once

namespace apn::dark::kuro::gdi::aviutl2::new_project
{
	struct DialogRenderer : gdi::DialogRenderer, Lockable
	{
		inline static constexpr size_t c_name_stat = 0;
		inline static constexpr size_t c_name = 1;
		inline static constexpr size_t c_video_size_stat = 2;
		inline static constexpr size_t c_video_width = 3;
		inline static constexpr size_t c_video_height = 4;
		inline static constexpr size_t c_video_rate_stat = 5;
		inline static constexpr size_t c_video_rate = 6;
		inline static constexpr size_t c_audio_rate_stat = 7;
		inline static constexpr size_t c_audio_rate = 8;
		inline static constexpr size_t c_ok = 9;
		inline static constexpr size_t c_nb_default_controls = 10;

		HWND controls[c_nb_default_controls] = {};

		HWND preset_stat = {}; // "プリセット"のスタティックコントロールです。
		HWND preset = {}; // "プリセット"のコンボボックスです。
		HWND swap_video_size = {}; // "縦横反転"のチェックボックスです。
		HWND name_preset = {}; // "名前プリセット"のコンボボックスです。
		HWND video_size_preset = {}; // "映像サイズプリセット"のコンボボックスです。
		HWND video_rate_preset = {}; // "映像レートプリセット"のコンボボックスです。
		HWND audio_rate_preset = {}; // "音声レートプリセット"のコンボボックスです。

		BOOL is_scene = {}; // シーンを作成する場合はTRUEになります。

		//
		// コンストラクタです。
		//
		DialogRenderer(BOOL is_scene)
			: is_scene(is_scene)
		{
		}

		//
		// コントロール配列を初期化します。
		//
		BOOL init_controls(HWND hwnd)
		{
			MY_TRACE_FUNC("{/hex}", hwnd);

			struct Param {
				DialogRenderer* dialog;
				int i;
			} param = { this, 0 };

			// 子ウィンドウを列挙します。
			::EnumChildWindows(hwnd,
				[](HWND child, LPARAM lParam) -> BOOL
			{
				MY_TRACE_HWND(child);

				// パラメータポインタを取得します。
				auto p = (Param*)lParam;

				// 子ウィンドウを配列に追加します。
				p->dialog->add_control(child, p->i++);

				return TRUE;
			}, (LPARAM)&param);

			return TRUE;
		}

		//
		// コントロールを配列に追加します。
		//
		void add_control(HWND control, int i)
		{
			if (is_scene)
			{
				auto index = c_name_stat + i;
				if (index < c_nb_default_controls)
					controls[index] = control;
			}
			else
			{
				auto index = c_video_size_stat + i;
				if (index < c_nb_default_controls)
					controls[index] = control;
			}
		}

		//
		// ダイアログのレイアウトを変更します。
		//
		BOOL change_layout(HWND hwnd)
		{
			MY_TRACE_FUNC("{/hex}", hwnd);

			//
			// この関数はコントロール矩形を返します。
			//
			const auto get_rc = [&](size_t control_index) {
				auto rc = my::get_window_rect(controls[control_index]);
				my::map_window_points(nullptr, hwnd, &rc);
				return rc;
			};

			// コントロール矩形を取得します。
			RECT rcs[c_nb_default_controls] = {};
			for (size_t i = 0; i < c_nb_default_controls; i++)
				rcs[i] = get_rc(i);

			// 基準サイズを取得します。
			auto base = SIZE {
				my::get_width(rcs[c_video_rate]),
				my::get_height(rcs[c_video_rate]),
			};

			// 余白サイズを取得します。
			auto space = SIZE {
				rcs[c_video_height].left - rcs[c_video_width].right,
				rcs[c_video_rate].top - rcs[c_video_width].bottom,
			};

			// 移動予定のオフセットサイズを取得します。
			auto offset = SIZE {
				base.cx + space.cx,
				base.cy + space.cy,
			};

			// 追加のコントロールを作成します。
			{
				// コントロールのフォントです。
				auto font = (HFONT)::SendMessage(controls[c_ok], WM_GETFONT, 0, 0);

				// 追加コントロールの基準X座標です。
				auto x = rcs[c_video_height].right + space.cx;

				{
					// Y座標の基準となるコントロール(一番上のコントロール)のインデックスです。
					auto base_control_index = is_scene ? c_name_stat : c_video_size_stat;

					// 追加コントロールの基準Y座標です。
					auto y = rcs[base_control_index].top;

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

					preset_stat = create_control(
						WC_STATICW, L"プリセット", SS_CENTERIMAGE, rcs[c_video_size_stat].left, y, base.cx, base.cy);
					preset = create_combobox(rcs[c_video_width].left, y, base.cx, base.cy);
					swap_video_size = create_control(
						WC_BUTTONW, L"縦横反転", BS_AUTOCHECKBOX, x, y, base.cx, base.cy);
					if (is_scene)
						name_preset = create_combobox(x, rcs[c_name_stat].top + offset.cy, base.cx, base.cy);
					video_size_preset = create_combobox(x, rcs[c_video_size_stat].top + offset.cy, base.cx, base.cy);
					video_rate_preset = create_combobox(x, rcs[c_video_rate_stat].top + offset.cy, base.cx, base.cy);
					audio_rate_preset = create_combobox(x, rcs[c_audio_rate_stat].top + offset.cy, base.cx, base.cy);

					//
					// この関数はコンボボックスをカスタマイズします。
					//
					const auto customize_combobox = [](HWND combobox) {
						auto rc = my::get_window_rect(combobox);
						auto dropped_width = my::get_width(rc) * 2;
						::SendMessage(combobox, CB_SETDROPPEDWIDTH, dropped_width, 0);
					};

					{
						// (全体の)プリセットのコンボボックスを初期化します。
						auto combobox = preset;
						customize_combobox(combobox);
						for (const auto& preset : hive.presets.preset_collection)
						{
							if (preset.display_name.empty())
							{
								if (preset.name.length())
								{
									my::combobox::add_text(combobox,
										my::format(L"{/}, {/} x {/} px, {/} fps, {/} Hz",
											preset.name,
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
						auto combobox = name_preset;
						customize_combobox(combobox);
						for (const auto& name : hive.presets.name_collection)
						{
							if (name.display_name.empty())
							{
								my::combobox::add_text(combobox, name.name.c_str());
							}
							else
							{
								my::combobox::add_text(combobox, name.display_name.c_str());
							}
						}
					}

					{
						// 映像サイズプリセットのコンボボックスを初期化します。
						auto combobox = video_size_preset;
						customize_combobox(combobox);
						for (const auto& video_size : hive.presets.video_size_collection)
						{
							if (video_size.display_name.empty())
							{
								my::combobox::add_text(combobox,
									my::format(L"{/} x {/}", video_size.width, video_size.height).c_str());
							}
							else
							{
								my::combobox::add_text(combobox, video_size.display_name.c_str());
							}
						}
					}

					{
						// 映像レートプリセットのコンボボックスを初期化します。
						auto combobox = video_rate_preset;
						customize_combobox(combobox);
						for (const auto& video_rate : hive.presets.video_rate_collection)
						{
							if (video_rate.display_name.empty())
							{
								my::combobox::add_text(combobox,
									my::format(L"{/}fps", video_rate.rate).c_str());
							}
							else
							{
								my::combobox::add_text(combobox, video_rate.display_name.c_str());
							}
						}
					}

					{
						// 音声レートのコンボボックスを初期化します。
						auto combobox = audio_rate_preset;
						customize_combobox(combobox);
						for (const auto& audio_rate : hive.presets.audio_rate_collection)
						{
							if (audio_rate.display_name.empty())
							{
								my::combobox::add_text(combobox,
									my::format(L"{/}Hz", audio_rate.rate).c_str());
							}
							else
							{
								my::combobox::add_text(combobox, audio_rate.display_name.c_str());
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

				for (size_t i = 0; i < c_nb_default_controls; i++)
				{
					if (!controls[i]) continue;

					auto rc = rcs[i];
					::OffsetRect(&rc, 0, offset.cy);
					dwp.set_window_pos(controls[i], nullptr, &rc, SWP_NOZORDER);
				}
			}

			return TRUE;
		}

		//
		// ウィンドウメッセージを処理します。
		//
		virtual LRESULT on_subclass_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

			switch (message)
			{
			case WM_INITDIALOG:
				{
					MY_TRACE_FUNC("WM_INITDIALOG, {/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

					auto result = __super::on_subclass_proc(hwnd, message, wParam, lParam);

					Locker locker(this);

					init_controls(hwnd);
					change_layout(hwnd);

					return result;
				}
			case WM_COMMAND:
				{
					MY_TRACE_FUNC("WM_COMMAND, {/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

					//
					// この関数はコントロールのテキストを変更します。
					//
					const auto set_control_text = [this](size_t index, const std::wstring& text)
					{
						// コントロールとテキストが両方とも有効の場合は
						if (controls[index] && text.length())
						{
							// コントロールのテキストを変更します。
							::SetWindowTextW(controls[index], text.c_str());
						}
					};

					// ロックされている場合は何もしません。
					if (is_locked()) break;

					Locker locker(this);

//					auto control_id = LOWORD(wParam);
					auto code = HIWORD(wParam);
					auto control = (HWND)lParam;

					// コントロールが無効の場合は何もしません。
					if (!control) break;

					if (control == preset)
					{
						if (code != CBN_SELCHANGE) break;

						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.video_size_collection.size()) break;

						// 全体のプリセットを適用します。
						const auto& preset = hive.presets.preset_collection[index];
						if (::SendMessage(swap_video_size, BM_GETCHECK, 0, 0) == BST_CHECKED)
						{
							set_control_text(c_video_width, preset.video_height);
							set_control_text(c_video_height, preset.video_width);
						}
						else
						{
							set_control_text(c_video_width, preset.video_width);
							set_control_text(c_video_height, preset.video_height);
						}

						set_control_text(c_video_rate, preset.video_rate);
						set_control_text(c_audio_rate, preset.audio_rate);

						if (is_scene)
							set_control_text(c_name, preset.name);
					}
					else if (control == name_preset)
					{
						if (code != CBN_SELCHANGE) break;

						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.name_collection.size()) break;

						// 名前のプリセットを適用します。
						const auto& name = hive.presets.name_collection[index];
						set_control_text(c_name, name.name);
					}
					else if (control == video_size_preset)
					{
						if (code != CBN_SELCHANGE) break;

						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.video_size_collection.size()) break;

						// 映像サイズのプリセットを適用します。
						const auto& video_size = hive.presets.video_size_collection[index];
						if (::SendMessage(swap_video_size, BM_GETCHECK, 0, 0) == BST_CHECKED)
						{
							set_control_text(c_video_width, video_size.height);
							set_control_text(c_video_height, video_size.width);
						}
						else
						{
							set_control_text(c_video_width, video_size.width);
							set_control_text(c_video_height, video_size.height);
						}
					}
					else if (control == video_rate_preset)
					{
						if (code != CBN_SELCHANGE) break;

						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.video_rate_collection.size()) break;

						// 映像レートのプリセットを適用します。
						const auto& video_rate = hive.presets.video_rate_collection[index];
						set_control_text(c_video_rate, video_rate.rate);
					}
					else if (control == audio_rate_preset)
					{
						if (code != CBN_SELCHANGE) break;

						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.audio_rate_collection.size()) break;

						// 音声レートのプリセットを適用します。
						const auto& audio_rate = hive.presets.audio_rate_collection[index];
						set_control_text(c_audio_rate, audio_rate.rate);
					}
					else if (control == swap_video_size)
					{
						if (code != BN_CLICKED) break;

						// 映像サイズの縦横を入れ替えます。

						auto width = my::get_window_text(controls[c_video_width]);
						auto height = my::get_window_text(controls[c_video_height]);

						::SetWindowText(controls[c_video_width], height.c_str());
						::SetWindowText(controls[c_video_height], width.c_str());
					}

					break;
				}
			}

			return __super::on_subclass_proc(hwnd, message, wParam, lParam);
		}
	};
}
