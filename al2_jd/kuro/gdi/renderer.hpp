#pragma once

namespace apn::dark::kuro::gdi
{
	//
	// このクラスはGDIレンダラーです。
	//
	struct Renderer
	{
		//
		// このクラスは標準化されたウィンドウのクラス名です。
		//
		struct NormalizedClassName : my::tstring {
			NormalizedClassName(const my::tstring& class_name) : my::tstring(class_name) {}
			bool operator==(LPCTSTR b) const { return ::lstrcmpi(c_str(), b) == 0; }
		};

		//
		// このクラスはメッセージの状態です。
		//
		struct MessageState
		{
			HWND hwnd;
			UINT message;
			WPARAM wParam;
			LPARAM lParam;
		};

		//
		// 現在のメッセージの状態です。
		//
		thread_local inline static MessageState current_message_state;

		//
		// レンダラーのコレクションです。
		//
		thread_local inline static std::unordered_map<HWND, std::shared_ptr<Renderer>> collection;

		//
		// レンダラーとウィンドウを関連付けます。
		//
		inline static void attach(HWND hwnd, const std::shared_ptr<Renderer>& renderer, UINT_PTR subclass_id)
		{
			// レンダラーをコレクションに追加します。
			collection[hwnd] = renderer;

			// ウィンドウをサブクラス化します。
			::SetWindowSubclass(hwnd, Renderer::subclass_proc, subclass_id, 0);
		}

		//
		// レンダラーとウィンドウの関連付けを解除します。
		//
		inline static void detach(HWND hwnd)
		{
			// レンダラーをコレクションから削除します。
			collection.erase(hwnd);
		}

		//
		// 指定されたウィンドウに関連付けられているレンダラーを返します。
		//
		inline static std::shared_ptr<Renderer> from_handle(HWND hwnd)
		{
			// レンダラの使用が抑制されている場合はnullptrを返します。
			if (hive.renderer_locked) return nullptr;

			auto it = collection.find(hwnd);
			if (it == collection.end()) return nullptr;
			return it->second;
		}

		//
		// オリジナルのウィンドウプロシージャを呼び出します。
		//
		inline static LRESULT CALLBACK orig_wnd_proc(MessageState* current_state)
		{
			return ::DefSubclassProc(current_state->hwnd, current_state->message, current_state->wParam, current_state->lParam);
		}

		//
		// サブクラスプロシージャです。
		//
		inline static LRESULT CALLBACK subclass_proc(
			HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam,
			UINT_PTR subclass_id, DWORD_PTR subclass_data)
		{
#ifdef _DEBUG
//			if (0)
			{
				// デバッグ用のコードです。

				auto class_name = my::get_class_name(hwnd);

				MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}, {/}", hwnd, message, wParam, lParam, class_name);
			}
#endif
			// レンダラーの使用が抑制されている場合はデフォルト処理のみ行います。
			if (hive.renderer_locked)
				return ::DefSubclassProc(hwnd, message, wParam, lParam);

			auto message_state = MessageState { hwnd, message, wParam, lParam };

			switch (message)
			{
			case WM_ACTIVATE:
				{
					auto active = LOWORD(wParam);

					MY_TRACE_FUNC("WM_ACTIVATE, {/}", active);

					if (active)
					{
//						auto border_color = DWMWA_COLOR_NONE;
//						auto border_color = style.get_COLORREF(Style::Color::ButtonBodySelect);
//						auto caption_color = style.get_COLORREF(Style::Color::ButtonBodySelect);
						auto border_color = style.get_COLORREF(Style::Color::WindowBorder);
						auto caption_color = style.get_COLORREF(Style::Color::Background);
						auto text_color = style.get_COLORREF(Style::Color::Text);

						::DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR , &border_color, sizeof(border_color));
						::DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR , &caption_color, sizeof(caption_color));
						::DwmSetWindowAttribute(hwnd, DWMWA_TEXT_COLOR , &text_color, sizeof(text_color));
					}
					else
					{
						auto border_color = style.get_COLORREF(Style::Color::TitleHeader);
						auto caption_color = style.get_COLORREF(Style::Color::TitleHeader);
						auto text_color = style.get_COLORREF(Style::Color::TextDisable);

						::DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR , &border_color, sizeof(border_color));
						::DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR , &caption_color, sizeof(caption_color));
						::DwmSetWindowAttribute(hwnd, DWMWA_TEXT_COLOR , &text_color, sizeof(text_color));
					}

					break;
				}
			case WM_CTLCOLORDLG:
			case WM_CTLCOLORMSGBOX:
			case WM_CTLCOLORBTN:
			case WM_CTLCOLOREDIT:
			case WM_CTLCOLORLISTBOX:
			case WM_CTLCOLORSCROLLBAR:
			case WM_CTLCOLORSTATIC:
				{
//					MY_TRACE_FUNC("WM_CTLCOLOR, {/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

					// デフォルト処理を実行してデフォルトのブラシを取得します。
					auto brush = (HBRUSH)orig_wnd_proc(&message_state);

					auto dc = (HDC)wParam;
					auto control = (HWND)lParam;
					MY_TRACE_HWND(control);

					// レンダラーを取得できた場合は処理を任せます。
					if (auto renderer = from_handle(control))
						return (LRESULT)renderer->on_ctl_color(hwnd, message, dc, control, brush);
					else
						return (LRESULT)brush;
				}
			case WM_NOTIFY:
				{
					auto nm = (NMHDR*)lParam;
/*
					if (NormalizedClassName(my::get_class_name(nm->hwndFrom)) == WC_BUTTON)
						return 0; // ボタンの場合は何もしません。
*/
					switch (nm->code)
					{
					case NM_CUSTOMDRAW:
						{
							// レンダラーを取得できた場合は処理を任せます。
							if (auto renderer = from_handle(nm->hwndFrom))
								return renderer->on_custom_draw(&message_state);

							break;
						}
					}

					break;
				}
			}

			// 現在のメッセージの状態を保存します。
			struct MessageStateSaver
			{
				MessageState old_message_state;

				MessageStateSaver(const MessageState& message_state)
				{
					// 現在のメッセージの状態を更新します。
					old_message_state = current_message_state;
					current_message_state = message_state;
				}
				~MessageStateSaver()
				{
					// 現在のメッセージの状態を復元します。
					current_message_state = old_message_state;
				}
			} message_state_saver = { message_state };

			// レンダラーを取得できた場合は処理を任せます。
			if (auto renderer = from_handle(hwnd))
				return renderer->on_subclass_proc(&message_state);

			return orig_wnd_proc(&message_state);
		}

		inline static BOOL fire_rectangle(HDC dc, int left, int top, int right, int bottom)
		{
			auto renderer = from_handle(current_message_state.hwnd);
			if (renderer) return renderer->on_rectangle(&current_message_state, dc, left, top, right, bottom);
			return hive.orig.Rectangle(dc, left, top, right, bottom);
		}

		inline static BOOL fire_fill_rect(HDC dc, LPCRECT rc, HBRUSH brush)
		{
			auto renderer = from_handle(current_message_state.hwnd);
			if (renderer) return renderer->on_fill_rect(&current_message_state, dc, rc, brush);
			return hive.orig.FillRect(dc, rc, brush);
		}

		inline static BOOL fire_draw_frame(HDC dc, LPRECT rc, UINT width, UINT type)
		{
			auto renderer = from_handle(current_message_state.hwnd);
			if (renderer) return renderer->on_draw_frame(&current_message_state, dc, rc, width, type);
			return hive.orig.DrawFrame(dc, rc, width, type);
		}

		inline static BOOL fire_draw_frame_control(HDC dc, LPRECT rc, UINT type, UINT state)
		{
			auto renderer = from_handle(current_message_state.hwnd);
			if (renderer) return renderer->on_draw_frame_control(&current_message_state, dc, rc, type, state);
			return hive.orig.DrawFrameControl(dc, rc, type, state);
		}

		inline static BOOL fire_frame_rect(HDC dc, LPCRECT rc, HBRUSH brush)
		{
			auto renderer = from_handle(current_message_state.hwnd);
			if (renderer) return renderer->on_frame_rect(&current_message_state, dc, rc, brush);
			return hive.orig.FrameRect(dc, rc, brush);
		}

		inline static BOOL fire_draw_edge(HDC dc, LPRECT rc, UINT edge, UINT flags)
		{
			auto renderer = from_handle(current_message_state.hwnd);
			if (renderer) return renderer->on_draw_edge(&current_message_state, dc, rc, edge, flags);
			return hive.orig.DrawEdge(dc, rc, edge, flags);
		}

		inline static BOOL fire_draw_focus_rect(HDC dc, LPCRECT rc)
		{
			auto renderer = from_handle(current_message_state.hwnd);
			if (renderer) return renderer->on_draw_focus_rect(&current_message_state, dc, rc);
			return hive.orig.DrawFocusRect(dc, rc);
		}

		inline static BOOL fire_draw_state_w(HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
		{
			auto renderer = from_handle(current_message_state.hwnd);
			if (renderer) return renderer->on_draw_state_w(&current_message_state, dc, fore, cb, lData, wData, x, y, cx, cy, flags);
			return hive.orig.DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
		}

		inline static BOOL fire_ext_text_out_w(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
		{
			MY_TRACE_HWND(current_message_state.hwnd);

			auto renderer = from_handle(current_message_state.hwnd);
			if (renderer) return renderer->on_ext_text_out_w(&current_message_state, dc, x, y, options, rc, text, c, dx);
			return hive.orig.ExtTextOutW(dc, x, y, options, rc, text, c, dx);
		}

		inline static BOOL fire_pat_blt(HDC dc, int x, int y, int w, int h, DWORD rop)
		{
			auto renderer = from_handle(current_message_state.hwnd);
			if (renderer) return renderer->on_pat_blt(&current_message_state, dc, x, y, w, h, rop);
			return hive.orig.PatBlt(dc, x, y, w, h, rop);
		}

		virtual LRESULT draw_nc_paint(HWND hwnd, HDC dc, const POINT& origin, LPRECT rc)
		{
			//
			// この関数は縁だけを描画します。
			// その後、描画した分だけ矩形を縮小します。
			//
			const auto draw_border = [&](int width, COLORREF color)
			{
				my::gdi::unique_ptr<HPEN> pen(::CreatePen(PS_SOLID, width, color));
				my::gdi::selector pen_selector(dc, pen.get());
				my::gdi::selector brush_selector(dc, ::GetStockObject(NULL_BRUSH));
				hive.orig.Rectangle(dc, rc->left, rc->top, rc->right, rc->bottom);
				::InflateRect(rc, -width, -width);
			};

			auto style = my::get_style(hwnd);
			auto ex_style = my::get_ex_style(hwnd);

			if (ex_style & WS_EX_WINDOWEDGE)
			{
				draw_border(2, kuro::style.get_COLORREF(Style::Color::WindowBorder));
			}
			else if (style & WS_BORDER)
			{
				draw_border(1, kuro::style.get_COLORREF(Style::Color::WindowBorder));
			}

			if (ex_style & WS_EX_STATICEDGE)
			{
				draw_border(1, kuro::style.get_COLORREF(Style::Color::WindowBorder));
			}

			if (ex_style & WS_EX_CLIENTEDGE)
			{
				draw_border(2, kuro::style.get_COLORREF(Style::Color::WindowBorder));
			}

			if (style & WS_HSCROLL && style & WS_VSCROLL)
			{
				auto corner_rc = *rc;
				corner_rc.top = corner_rc.bottom - ::GetSystemMetrics(SM_CYHSCROLL);
				corner_rc.left = corner_rc.right - ::GetSystemMetrics(SM_CXVSCROLL);

				my::gdi::unique_ptr<HBRUSH> brush(::CreateSolidBrush(
					kuro::style.get_COLORREF(Style::Color::Background)));
				hive.orig.FillRect(dc, &corner_rc, brush.get());
			}

			return 0;
		}

		virtual LRESULT on_subclass_proc(MessageState* current_state)
		{
			// 最終メッセージの場合は
			if (current_state->message == WM_NCDESTROY)
			{
				// デフォルト処理を実行してから
				auto result = orig_wnd_proc(current_state);

				// デタッチします。
				detach(current_state->hwnd);

				return result;
			}

			return orig_wnd_proc(current_state);
		}

		virtual LRESULT on_custom_draw(MessageState* current_state)
		{
			return orig_wnd_proc(current_state);
		}

		virtual HBRUSH on_ctl_color(HWND hwnd, UINT message, HDC dc, HWND control, HBRUSH brush)
		{
			return brush;
		}

		virtual BOOL on_rectangle(MessageState* current_state, HDC dc, int left, int top, int right, int bottom)
		{
			return hive.orig.Rectangle(dc, left, top, right, bottom);
		}

		virtual BOOL on_fill_rect(MessageState* current_state, HDC dc, LPCRECT rc, HBRUSH brush)
		{
			return hive.orig.FillRect(dc, rc, brush);
		}

		virtual BOOL on_draw_frame(MessageState* current_state, HDC dc, LPRECT rc, UINT width, UINT type)
		{
			return hive.orig.DrawFrame(dc, rc, width, type);
		}

		virtual BOOL on_draw_frame_control(MessageState* current_state, HDC dc, LPRECT rc, UINT type, UINT state)
		{
			return hive.orig.DrawFrameControl(dc, rc, type, state);
		}

		virtual BOOL on_frame_rect(MessageState* current_state, HDC dc, LPCRECT rc, HBRUSH brush)
		{
			return hive.orig.FrameRect(dc, rc, brush);
		}

		virtual BOOL on_draw_edge(MessageState* current_state, HDC dc, LPRECT rc, UINT edge, UINT flags)
		{
			return hive.orig.DrawEdge(dc, rc, edge, flags);
		}

		virtual BOOL on_draw_focus_rect(MessageState* current_state, HDC dc, LPCRECT rc)
		{
			return hive.orig.DrawFocusRect(dc, rc);
		}

		virtual BOOL on_draw_state_w(MessageState* current_state, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
		{
			return hive.orig.DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
		}

		virtual BOOL on_ext_text_out_w(MessageState* current_state, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
		{
			return hive.orig.ExtTextOutW(dc, x, y, options, rc, text, c, dx);
		}

		virtual BOOL on_pat_blt(MessageState* current_state, HDC dc, int x, int y, int w, int h, DWORD rop)
		{
			return hive.orig.PatBlt(dc, x, y, w, h, rop);
		}
	};
}
