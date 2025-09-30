#pragma once

namespace apn::dark::kuro::gdi
{
	struct ListViewRenderer : Renderer
	{
		const paint::Palette& palette = paint::listview_material.palette;
#if 0 // テスト用コードです。
		virtual LRESULT on_subclass_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			switch (message)
			{
			case WM_ERASEBKGND:
				{
					MY_TRACE_FUNC("WM_ERASEBKGND, {/hex}, {/hex}, {/hex}, {/hex}",
						current_state->hwnd, current_state->message, current_state->wParam, current_state->lParam);

					break;
				}
			}

			return __super::on_subclass_proc(hwnd, message, wParam, lParam);
		}
#endif
		virtual BOOL on_fill_rect(MessageState* current_state, HDC dc, LPCRECT rc, HBRUSH brush) override
		{
			MY_TRACE_FUNC("{/hex}, ({/}), {/hex}", dc, safe_string(rc), brush);

			// 背景色を描画します。
			{
				auto color = paint::get_brush_color(brush);
				auto part_id = LVP_LISTITEM;
				auto state_id = LISS_NORMAL;

				if (color == hive.orig.GetSysColor(COLOR_HIGHLIGHT))
				{
					state_id = LISS_HOT;
				}
				else if (color == hive.orig.GetSysColor(COLOR_BTNFACE))
				{
					state_id = LISS_SELECTEDNOTFOCUS;
				}

				MY_TRACE("color = {/hex}, part_id = {/}, state_id = {/}\n", color, (int)part_id, (int)state_id);

				if (auto pigment = palette.get(part_id, state_id))
					return paint::stylus.draw_rect(dc, rc, pigment);
			}

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

			return TRUE; // フォーカス矩形を描画しないようにします。
//			return hive.orig.DrawFocusRect( dc, rc);
		}

		virtual BOOL on_draw_state_w(MessageState* current_state, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/}, {/hex}", dc, fore, x, y, cx, cy, flags);

			return hive.orig.DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
		}

		virtual BOOL on_ext_text_out_w(MessageState* current_state, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/}, {/hex}, {/}, {/}, {/}, {/hex}, {/hex}, {/hex}", dc, x, y, options, safe_string(rc), text, c, dx, ::GetBkColor(dc), ::GetTextColor(dc));
#if 1
//			if (!(options & ETO_IGNORELANGUAGE))
			{
				if (options == ETO_OPAQUE && rc)
				{
					auto part_id = LVP_LISTITEM;
					auto state_id = LISS_NORMAL;

					// セパレータの場合は
					if (my::get_width(*rc) == 1 || my::get_height(*rc) == 1)
						state_id = (decltype(state_id))0;

					if (auto pigment = palette.get(part_id, state_id))
						return paint::stylus.ext_text_out(dc, x, y, options, rc, text, c, dx, pigment);
				}
				else
				{
					auto current_color = ::GetPixel(dc, x, y);
					auto part_id = LVP_LISTITEM;
					auto state_id = LISS_NORMAL;

					if (auto pigment = palette.get(part_id, state_id))
					{
						if (current_color != pigment->background.get_win32_color())
							state_id = LISS_HOT;
					}

					MY_TRACE("current_color = {/hex}, part_id = {/}, state_id = {/}\n",
						current_color, (int)part_id, (int)state_id);

					if (auto pigment = palette.get(part_id, state_id))
						return paint::stylus.ext_text_out(dc, x, y, options, rc, text, c, dx, pigment);
				}
			}
#endif
			return hive.orig.ExtTextOutW(dc, x, y, options, rc, text, c, dx);
		}

		virtual BOOL on_pat_blt(MessageState* current_state, HDC dc, int x, int y, int w, int h, DWORD rop) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}, {/}, {/hex}", dc, x, y, w, h, rop);

			return hive.orig.PatBlt(dc, x, y, w, h, rop);
		}
	};
}
