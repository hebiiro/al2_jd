#pragma once

namespace apn::dark::kuro::gdi::aviutl2::new_project
{
	struct DialogRenderer : gdi::DialogRenderer
	{
		//
		// ロックカウントです。
		//
		int lock_count = 0;

		//
		// ロックされている場合はTRUEを返します。
		// ロックされているときはWM_COMMANDを無視します。
		//
		BOOL is_locked() const { return lock_count != 0; }

		//
		// ロックします。
		//
		void lock() { lock_count++; }

		//
		// ロックを解除します。
		//
		void unlock() { lock_count--; }

		//
		// このクラスはダイアログをロックします。
		//
		struct Locker {
			DialogRenderer* dialog;
			Locker(DialogRenderer* dialog) : dialog(dialog) { dialog->lock(); }
			~Locker() { dialog->unlock(); }
		};

		inline static constexpr size_t c_video_size_stat = 0;
		inline static constexpr size_t c_video_width = 1;
		inline static constexpr size_t c_video_height = 2;
		inline static constexpr size_t c_video_rate_stat = 3;
		inline static constexpr size_t c_video_rate = 4;
		inline static constexpr size_t c_audio_rate_stat = 5;
		inline static constexpr size_t c_audio_rate = 6;
		inline static constexpr size_t c_ok = 7;
		inline static constexpr size_t c_nb_default_controls = 8;

		std::vector<HWND> controls;

		HWND preset_stat = {}; // "プリセット"のスタティックコントロールです。
		HWND preset = {}; // コンボボックスです。
		HWND swap_video_size = {}; // "縦横反転"のチェックボックスです。
		HWND video_size_preset = {}; // コンボボックスです。
		HWND video_rate_preset = {}; // コンボボックスです。
		HWND audio_rate_preset = {}; // コンボボックスです。

		//
		// コントロールが不正の場合はTRUEを返します。
		//
		BOOL is_invalid_controls() const { return controls.size() < c_nb_default_controls; }

		//
		// コントロールを取得します。
		//
		BOOL get_controls(HWND hwnd)
		{
			MY_TRACE_FUNC("{/hex}", hwnd);

			// 子ウィンドウを列挙します。
			::EnumChildWindows(hwnd,
				[](HWND child, LPARAM lParam) -> BOOL
			{
				MY_TRACE_HWND(child);

				// thisポインタを取得します。
				auto p = (DialogRenderer*)lParam;

				// 子ウィンドウをコレクションに追加します。
				p->controls.emplace_back(child);

				return TRUE;
			}, (LPARAM)this);

			return TRUE;
		}

		//
		// ダイアログのレイアウトを変更します。
		//
		BOOL change_layout(HWND hwnd)
		{
			MY_TRACE_FUNC("{/hex}", hwnd);

			// コントロールが不正の場合は何もしません。
			if (is_invalid_controls()) return FALSE;

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
					// 追加コントロールの基準Y座標です。
					auto y = rcs[c_video_size_stat].top;

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
						// 全体のプリセットのコンボボックスを初期化します。
						auto combobox = preset;
						customize_combobox(combobox);
						for (const auto& preset : hive.presets.preset_collection)
						{
							if (preset.display_name.empty())
							{
								my::combobox::add_text(combobox,
									my::format(L"{/} x {/} px, {/} fps, {/} Hz",
										preset.video_width, preset.video_height,
										preset.video_rate, preset.audio_rate).c_str());
							}
							else
							{
								my::combobox::add_text(combobox, preset.display_name.c_str());
							}
						}
					}

					{
						// 映像サイズのコンボボックスを初期化します。
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
						// 映像レートのコンボボックスを初期化します。
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
					auto rc = rcs[i];
					::OffsetRect(&rc, 0, offset.cy);
					dwp.set_window_pos(controls[i], nullptr, &rc, SWP_NOZORDER);
				}
			}

			return TRUE;
		}

		//
		// 最近使った設定を保存します。
		//
		BOOL load_recent_setting(HWND hwnd)
		{
			MY_TRACE_FUNC("{/hex}", hwnd);

			// コントロールが不正の場合は何もしません。
			if (is_invalid_controls()) return FALSE;

			//
			// この関数は最近使った設定をコントロールに適用します。
			//
			const auto load_recent = [&](const std::wstring& setting, size_t control_index) {
				if (setting.length())
					::SetWindowText(controls[control_index], setting.c_str());
			};

			load_recent(hive.new_project.recent.video_width, c_video_width);
			load_recent(hive.new_project.recent.video_height, c_video_height);
			load_recent(hive.new_project.recent.video_rate, c_video_rate);
			load_recent(hive.new_project.recent.audio_rate, c_audio_rate);

			return TRUE;
		}

		//
		// 最近使った設定を保存します。
		//
		BOOL save_recent_setting(HWND hwnd)
		{
			MY_TRACE_FUNC("{/hex}", hwnd);

			// コントロールが不正の場合は何もしません。
			if (is_invalid_controls()) return FALSE;

			//
			// この関数は最近使った設定をコントロールから取得します。
			//
			const auto save_recent = [&](std::wstring& setting, size_t control_index) {
				setting = my::get_window_text(controls[control_index]);
			};

			save_recent(hive.new_project.recent.video_width, c_video_width);
			save_recent(hive.new_project.recent.video_height, c_video_height);
			save_recent(hive.new_project.recent.video_rate, c_video_rate);
			save_recent(hive.new_project.recent.audio_rate, c_audio_rate);

			return TRUE;
		}

		//
		// ウィンドウメッセージを処理します。
		//
		virtual LRESULT on_subclass_proc(MessageState* current_state) override
		{
			auto hwnd = current_state->hwnd;
			auto message = current_state->message;
			auto wParam = current_state->wParam;
			auto lParam = current_state->lParam;

			MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

			switch (message)
			{
			case WM_INITDIALOG:
				{
					MY_TRACE_FUNC("WM_INITDIALOG, {/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

					auto result = __super::on_subclass_proc(current_state);

					Locker locker(this);

					get_controls(hwnd);
					load_recent_setting(hwnd);
					change_layout(hwnd);

					return result;
				}
			case WM_COMMAND:
				{
					MY_TRACE_FUNC("WM_COMMAND, {/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

					if (is_locked() || is_invalid_controls()) break;

					Locker locker(this);

//					auto control_id = LOWORD(wParam);
					auto code = HIWORD(wParam);
					auto control = (HWND)lParam;

					if (control == controls[c_ok])
					{
						save_recent_setting(hwnd);
					}
					else if (control == preset)
					{
						if (code != CBN_SELCHANGE) break;

						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.video_size_collection.size()) break;

						// 全体のプリセットを適用します。
						const auto& preset = hive.presets.preset_collection[index];
						if (::SendMessage(swap_video_size, BM_GETCHECK, 0, 0) == BST_CHECKED)
						{
							::SetWindowTextW(controls[c_video_width], preset.video_height.c_str());
							::SetWindowTextW(controls[c_video_height], preset.video_width.c_str());
						}
						else
						{
							::SetWindowTextW(controls[c_video_width], preset.video_width.c_str());
							::SetWindowTextW(controls[c_video_height], preset.video_height.c_str());
						}

						::SetWindowTextW(controls[c_video_rate], preset.video_rate.c_str());
						::SetWindowTextW(controls[c_audio_rate], preset.audio_rate.c_str());
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
							::SetWindowTextW(controls[c_video_width], video_size.height.c_str());
							::SetWindowTextW(controls[c_video_height], video_size.width.c_str());
						}
						else
						{
							::SetWindowTextW(controls[c_video_width], video_size.width.c_str());
							::SetWindowTextW(controls[c_video_height], video_size.height.c_str());
						}
					}
					else if (control == video_rate_preset)
					{
						if (code != CBN_SELCHANGE) break;

						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.video_rate_collection.size()) break;

						// 映像レートのプリセットを適用します。
						const auto& video_rate = hive.presets.video_rate_collection[index];
						::SetWindowTextW(controls[c_video_rate], video_rate.rate.c_str());
					}
					else if (control == audio_rate_preset)
					{
						if (code != CBN_SELCHANGE) break;

						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.audio_rate_collection.size()) break;

						// 音声レートのプリセットを適用します。
						const auto& audio_rate = hive.presets.audio_rate_collection[index];
						::SetWindowTextW(controls[c_audio_rate], audio_rate.rate.c_str());
					}

					break;
				}
			}

			return __super::on_subclass_proc(current_state);
		}
	};
}
