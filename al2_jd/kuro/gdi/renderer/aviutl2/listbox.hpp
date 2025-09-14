#pragma once

namespace apn::dark::kuro::gdi::aviutl2
{
	//
	// オーナーウィンドウがaviutl2の場合のリストボックスの処理です。
	//
	struct ListBoxRenderer : gdi::ListBoxRenderer
	{
		//
		// このリストボックスがフォント一覧の場合はTRUEになります。
		//
		BOOL is_fonts_flag = FALSE;

		//
		// 元のアイテムの高さです。
		//
		int orig_item_height = 0;

		//
		// チェック済みの項目数です。
		//
		int nb_checks = 0;

		//
		// チェックする最大項目数です。
		//
		int nb_max_checks = 4; // 全数検査は冗長なので、最初のn個だけチェックするようにします。

		//
		// フォント一覧リストボックスの表示予定位置を変更します。
		//
		BOOL set_fonts_pos(WINDOWPOS* wp)
		{
			MY_TRACE_FUNC("");
#if 0
			auto dpi = ::GetDpiForWindow(wp->hwnd);
			auto combobox_height = ::MulDiv(orig_item_height, USER_DEFAULT_SCREEN_DPI, dpi);
#else
			auto combobox_height = orig_item_height + 4;
#endif
			auto rc_combobox = RECT { wp->x, wp->y - combobox_height, wp->x + wp->cx, wp->y };
			auto rc_listbox = RECT { 0, 0, hive.fonts.window_size.cx, hive.fonts.window_size.cy };
			my::client_to_window(wp->hwnd, &rc_listbox);
			auto x = wp->x;
			auto y = wp->y;
			auto w = my::get_width(rc_listbox);
			auto h = my::get_height(rc_listbox);

			auto is_horz_available = FALSE;
			auto rc_monitor = my::get_monitor_rect(wp->hwnd);

			// リストボックスのボトムがモニタのボトムからはみ出す場合は
			if (rc_combobox.bottom + h > rc_monitor.bottom)
			{
				// リストボックスのトップがモニタのトップからはみ出す場合は
				if (rc_combobox.top - h < rc_monitor.top)
				{
					h = std::min<int>(h, my::get_height(rc_monitor));

					// 可能な限りコンボボックスの真横に配置します。
					y = my::get_center_y(rc_combobox) - h / 2;
					y = std::max<int>(y, rc_monitor.top);
					y = std::min<int>(y, rc_monitor.bottom - h);

					// リストボックスのレフトがモニタのレフトからはみ出す場合は
					if (rc_combobox.left - w < rc_monitor.left)
					{
						// リストボックスをコンボボックスの右側に配置します。
						x = rc_combobox.right;
						w = std::min<int>(w, rc_monitor.right - x);
					}
					else
					{
						// リストボックスをコンボボックスの左側に配置します。
						x = rc_combobox.left - w;
					}

					// xとwをセットしたのでフラグを立てます。
					is_horz_available = TRUE;
				}
				else
				{
					// リストボックスをコンボボックスの上側に配置します。
					y = rc_combobox.top - h;
				}
			}
			else
			{
				// リストボックスをコンボボックスの下側に配置します。
				y = rc_combobox.bottom;
			}

			// xとwがまだセットされていない場合は
			if (!is_horz_available)
			{
				// リストボックスの幅がモニタの幅を超えないように調整します。
				w = std::min<int>(w, my::get_width(rc_monitor));

				// リストボックスのレフトがモニタのレフトからはみ出す場合は
				if (rc_combobox.left < rc_monitor.left)
				{
					// リストボックスのレフトがモニタのレフトと一致するように配置します。
					x = rc_monitor.left;
				}
				// リストボックスのライトがモニタのライトからはみ出す場合は
				else if (rc_combobox.left + w > rc_monitor.right)
				{
					// リストボックスのライトがモニタのライトと一致するように配置します。
					x = rc_monitor.right - w;
				}
				else
				{
					// リストボックスのレフトがコンボボックスのレフトと一致するように配置します。
					x = rc_combobox.left;
				}
			}

			wp->x = x;
			wp->y = y;
			wp->cx = w;
			wp->cy = h;

			return TRUE;
		}
#if 1
		virtual LRESULT on_subclass_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

			switch (message)
			{
			case WM_SHOWWINDOW:
				{
					MY_TRACE("WM_SHOWWINDOW\n");

					if (wParam && is_fonts_flag)
					{
						MY_TRACE("フォント一覧リストボックスの表示サイズを変更します\n");

						::SendMessage(hwnd, LB_SETITEMHEIGHT, 0, hive.fonts.item_height);

						auto rc = my::get_window_rect(hwnd);
						auto wp = WINDOWPOS {
							.hwnd = hwnd,
							.x = rc.left,
							.y = rc.top,
							.cx = my::get_width(rc),
							.cy = my::get_height(rc),
						};
						set_fonts_pos(&wp);

						::SetWindowPos(hwnd, nullptr, wp.x, wp.y, wp.cx, wp.cy,
							SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
					}
					else
					{
						::SendMessage(hwnd, LB_SETITEMHEIGHT, 0, orig_item_height);
					}

					break;
				}
			case LB_RESETCONTENT:
				{
					MY_TRACE("LB_RESETCONTENT\n");

					if (!orig_item_height)
						orig_item_height = (LONG)::SendMessage(hwnd, LB_GETITEMHEIGHT, 0, 0);

					// フォントを使用して描画する場合は
					// とりあえずフォント一覧であるフラグをセットします。
					// このフラグは後続のLB_ADDSTRINGでリセットされる可能性があります。
					is_fonts_flag = hive.fonts.use_on_listbox;
					nb_checks = 0;

					break;
				}
			case LB_ADDSTRING:
				{
					MY_TRACE("LB_ADDSTRING\n");

					// リストボックスがフォント一覧で
					// なおかつチェックがまだ終わっていない場合は
					if (is_fonts_flag && nb_checks < nb_max_checks)
					{
						// 項目名を取得します。
						auto text = (LPCWSTR)lParam;

						MY_TRACE("『{/}』がフォント名かチェックします\n", text);

						// 項目名がフォント名ではない場合は
						if (!hive.available_fonts.contains(text))
						{
							MY_TRACE("このリストボックスはフォント一覧ではありませんでした\n");

							// フォント一覧であるフラグをリセットします。
							is_fonts_flag = FALSE;
						}

						// チェック済みの項目数を増やします。
						nb_checks++;
					}

					break;
				}
			}

			return __super::on_subclass_proc(hwnd, message, wParam, lParam);
		}
#endif
		virtual BOOL on_ext_text_out_w(MessageState* current_state, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/}, options = {/hex}, {/}, {/}, {/}, {/hex}, bk_color = {/hex}, text_color = {/hex}",
				dc, x, y, options, safe_string(rc), safe_string(text, c, options), c, dx, ::GetBkColor(dc), ::GetTextColor(dc));
#if 1
//			if (!(options & (ETO_GLYPH_INDEX | ETO_IGNORELANGUAGE)))
			{
				auto part_id = EP_EDITTEXT;
				auto state_id = ::IsWindowEnabled(current_state->hwnd) ? ETS_NORMAL : ETS_DISABLED;

				// 選択カラーの場合は
				if (::GetBkColor(dc) == hive.orig.GetSysColor(COLOR_HIGHLIGHT))
					state_id = ETS_SELECTED; // 選択状態として描画します。

				if (auto pigment = palette.get(part_id, state_id))
				{
					if (is_fonts_flag && text)
					{
						// フォントを使用して項目名を描画します。

						auto font_name = std::wstring(text, c);
						auto font_height = hive.fonts.font_height;

						my::gdi::unique_ptr<HFONT> font(::CreateFontW(
							font_height, 0, 0, 0, 0, FALSE, FALSE, FALSE,
							DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH, font_name.c_str()));
						my::gdi::selector font_selector(dc, font.get());

						auto preview_text = hive.fonts.sample_text_format.length() ?
							my::replace(hive.fonts.sample_text_format, L"%font%", font_name) : font_name;

						return paint::stylus.ext_text_out(dc, x, y, options, rc,
							preview_text.c_str(), (UINT)preview_text.length(), dx, pigment);
					}
					else
					{
						return paint::stylus.ext_text_out(dc, x, y, options, rc, text, c, dx, pigment);
					}
				}
			}
#endif
			return hive.orig.ExtTextOutW(dc, x, y, options, rc, text, c, dx);
		}
	};
}
