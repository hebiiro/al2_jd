#pragma once

namespace apn::dark::kuro::gdi::aviutl2::layer_name
{
	struct DialogRenderer : gdi::DialogRenderer, Lockable
	{
		inline static constexpr size_t c_name_stat = 0;
		inline static constexpr size_t c_name = 1;
		inline static constexpr size_t c_nb_default_controls = 2;

		HWND controls[c_nb_default_controls] = {};

		HWND name_preset = {}; // 名前プリセットのコンボボックスです。
		HWND ok = {}; // OKボタンです。
		HWND cancel = {}; // キャンセルボタンです。

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
			auto index = c_name_stat + i;
			if (index < c_nb_default_controls)
				controls[index] = control;
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

			// クライアント矩形を取得します。
			auto client_rc = my::get_client_rect(hwnd);

			// コントロール矩形を取得します。
			RECT rcs[c_nb_default_controls] = {};
			for (size_t i = 0; i < c_nb_default_controls; i++)
				rcs[i] = get_rc(i);

			// 基準サイズを取得します。
			auto base = SIZE {
				my::get_width(rcs[c_name]),
				my::get_height(rcs[c_name]),
			};

			// 余白サイズを取得します。
			auto space = SIZE { 5, 5 };

			// 移動予定のオフセットサイズを取得します。
			auto offset = SIZE {
				base.cx + space.cx,
				base.cy + space.cy,
			};

			// 追加のコントロールを作成します。
			{
				// コントロールのフォントです。
				auto font = (HFONT)::SendMessage(controls[c_name], WM_GETFONT, 0, 0);

				// 追加コントロールの基準X座標です。
				auto x = rcs[c_name].right + space.cx;

				{
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

					name_preset = create_combobox(x, rcs[c_name_stat].top, base.cx, base.cy);

					{
						auto button_size = SIZE {
							::MulDiv(my::get_width(rcs[c_name_stat]), 5, 2),
							base.cy,
						};
						auto button_pos = POINT {
							x + base.cx - button_size.cx,
//							client_rc.right + offset.cx - button_size.cx,
							client_rc.bottom + offset.cy - button_size.cy,
						};

						cancel = create_control(WC_BUTTONW, L"キャンセル", BS_PUSHBUTTON, button_pos.x, button_pos.y, button_size.cx, button_size.cy);
						button_pos.x -= button_size.cx + space.cx;
						ok = create_control(WC_BUTTONW, L"OK", BS_DEFPUSHBUTTON, button_pos.x, button_pos.y, button_size.cx, button_size.cy);
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
						// 名前プリセットのコンボボックスを初期化します。
						auto combobox = name_preset;
						customize_combobox(combobox);
						for (const auto& layer_name : hive.presets.layer_name_collection)
							my::combobox::add_text(combobox, layer_name.name.c_str());
					}
				}
			}

			// ダイアログのサイズを変更します。
			{
				WINDOWPLACEMENT wp = { sizeof(wp) };
				::GetWindowPlacement(hwnd, &wp);
				wp.rcNormalPosition.right += offset.cx;
				wp.rcNormalPosition.bottom += offset.cy + space.cy * 2;
				::SetWindowPlacement(hwnd, &wp);
			}
#if 0
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
#endif
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
						::SetWindowTextW(controls[index], text.c_str());
					};

					// ロックされている場合は何もしません。
					if (is_locked()) break;

					Locker locker(this);

//					auto control_id = LOWORD(wParam);
					auto code = HIWORD(wParam);
					auto control = (HWND)lParam;

					// コントロールが無効の場合は何もしません。
					if (!control) break;

					if (control == name_preset)
					{
						if (code != CBN_SELCHANGE) break;

						auto index = (size_t)my::combobox::get_cur_sel(control);
						if (index >= hive.presets.layer_name_collection.size()) break;

						// 名前のプリセットを適用します。
						const auto& layer_name = hive.presets.layer_name_collection[index];
						set_control_text(c_name, layer_name.name);
					}
					else if (control == ok)
					{
						MY_TRACE("OKボタンがクリックされました\n");

						::PostMessage(hwnd, WM_KEYDOWN, VK_RETURN, 0);
					}
					else if (control == cancel)
					{
						MY_TRACE("キャンセルボタンがクリックされました\n");

						::PostMessage(hwnd, WM_KEYDOWN, VK_ESCAPE, 0);
					}

					break;
				}
			}

			return __super::on_subclass_proc(hwnd, message, wParam, lParam);
		}
	};
}
