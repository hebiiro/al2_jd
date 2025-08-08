#pragma once

namespace apn::dark::kuro::gdi
{
	struct TrackBarRenderer : Renderer
	{
		virtual LRESULT on_subclass_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			switch (message)
			{
			case WM_ERASEBKGND:
				{
//					MY_TRACE_FUNC("WM_ERASEBKGND, {/hex}, {/hex}, {/hex}, {/hex}", current_state->hwnd, current_state->message, current_state->wParam, current_state->lParam);

					// ちらつき防止用の処理です。
					// デフォルトの塗りつぶし処理をスキップします。
					return TRUE;
				}
#if 0 // aviutl2の場合はこの処理はいらないと思われます。
			case WM_LBUTTONDOWN:
				{
//					MY_TRACE_FUNC("WM_LBUTTONDOWN, {/hex}, {/hex}, {/hex}, {/hex}", current_state->hwnd, current_state->message, current_state->wParam, current_state->lParam);

					auto result = __super::on_subclass_proc(hwnd, message, wParam, lParam);

					// コモンコントロールをバージョン6にすると、
					// WM_LBUTTONDOWNでSB_THUMBTRACKが送られてこなくなります。
					// なので、SB_THUMBTRACKを手動で送ります。
					auto hwnd = current_state->hwnd;
					auto pos = (int)::SendMessage(hwnd, TBM_GETPOS, 0, 0);
					::SendMessage(::GetParent(hwnd), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, pos), (LPARAM)hwnd);

					return result;
				}
#endif
			}

			return __super::on_subclass_proc(hwnd, message, wParam, lParam);
		}

		virtual HBRUSH on_ctl_color(HWND hwnd, UINT message, HDC dc, HWND control, HBRUSH brush) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, dc, control, brush);

			// トラックバーの背景の色を変更します。
			// トラックバーがダイアログに配置されることを前提に処理しています。
			return get_dialog_brush(hwnd, message, dc, control, brush);
		}

		virtual BOOL on_fill_rect(MessageState* current_state, HDC dc, LPCRECT rc, HBRUSH brush) override
		{
			MY_TRACE_FUNC("{/hex}, ({/}), {/hex}", dc, safe_string(rc), brush);

			return hive.orig.FillRect(dc, rc, brush);
		}

		virtual BOOL on_draw_frame(MessageState* current_state, HDC dc, LPRECT rc, UINT width, UINT type) override
		{
			MY_TRACE_FUNC("{/hex}, ({/}), {/}, {/hex}", dc, safe_string(rc), width, type);

			return hive.orig.DrawFrame(dc, rc, width, type);
		}

		virtual BOOL on_draw_frame_control(MessageState* current_state, HDC dc, LPRECT rc, UINT type, UINT state) override
		{
			MY_TRACE_FUNC("{/hex}, ({/}), {/hex}, {/hex}", dc, safe_string(rc), type, state);

			return hive.orig.DrawFrameControl(dc, rc, type, state);
		}

		virtual BOOL on_frame_rect(MessageState* current_state, HDC dc, LPCRECT rc, HBRUSH brush) override
		{
			MY_TRACE_FUNC("{/hex}, ({/}), {/hex}", dc, safe_string(rc), brush);

			return hive.orig.FrameRect(dc, rc, brush);
		}

		virtual BOOL on_draw_edge(MessageState* current_state, HDC dc, LPRECT rc, UINT edge, UINT flags) override
		{
			MY_TRACE_FUNC("{/hex}, ({/}), {/hex}, {/hex}", dc, safe_string(rc), edge, flags);

			return hive.orig.DrawEdge(dc, rc, edge, flags);
		}

		virtual BOOL on_draw_focus_rect(MessageState* current_state, HDC dc, LPCRECT rc) override
		{
			MY_TRACE_FUNC("{/hex}, ({/})", dc, safe_string(rc));

			return hive.orig.DrawFocusRect( dc, rc);
		}

		virtual BOOL on_draw_state_w(MessageState* current_state, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/}, {/hex}", dc, fore, x, y, cx, cy, flags);

			return hive.orig.DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
		}

		virtual BOOL on_ext_text_out_w(MessageState* current_state, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx) override
		{
//			MY_TRACE_FUNC("{/hex}, {/}, {/}, {/hex}, {/}, {/}, {/}, {/hex}, {/hex}, {/hex}", dc, x, y, options, safe_string(rc), text, c, dx, ::GetBkColor(dc), ::GetTextColor(dc));

			// トラックの選択部分の色を変更します。
			{
				// 背景色が選択色の場合は
				if (::GetBkColor(dc) == hive.orig.GetSysColor(COLOR_HIGHLIGHT))
				{
					const auto& palette = paint::trackbar_material.palette;

					auto part_id = EP_EDITTEXT;
					auto state_id = ETS_SELECTED;

					if (auto pigment = palette.get(part_id, state_id))
					{
						if (pigment->background.is_valid())
							::SetBkColor(dc, pigment->background.color);
					}
				}
			}

			return hive.orig.ExtTextOutW(dc, x, y, options, rc, text, c, dx);
		}

		virtual BOOL on_pat_blt(MessageState* current_state, HDC dc, int x, int y, int w, int h, DWORD rop) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}, {/}, {/hex}", dc, x, y, w, h, rop);

			return hive.orig.PatBlt(dc, x, y, w, h, rop);
		}
	};
}
