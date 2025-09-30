#pragma once

namespace apn::dark::kuro::theme
{
	inline struct ToolBarRenderer : Renderer
	{
		const paint::Palette& palette = paint::toolbar_material.palette;

		//
		// ドロップダウンボタンを描画します。
		//
		BOOL draw_drop_down_button(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			auto rc = *arg_rc;

			paint::stylus.draw_rect(dc, &rc, palette, part_id, state_id);

			auto width = my::get_width(rc);
			rc.top = (rc.top + rc.bottom - width) / 2 - 2;
			rc.bottom = rc.top + width;

			return paint::stylus.draw_icon(dc, &rc, palette, part_id, state_id, paint::c_symbol, 0xE015, 900);
		}

		//
		// ドロップダウンボタングリフを描画します。
		//
		BOOL draw_drop_down_button_glyph(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			auto rc = *arg_rc;

			auto w = my::get_width(rc);
			auto h = my::get_height(rc);

			// 縦長矩形の場合は
			if (w < h)
			{
				// センタリングされた正方形に変更します。
				rc.top = (rc.top + rc.bottom - w) / 2;
				rc.bottom = rc.top + w;
			}

//			::InflateRect(&rc, 8, 8);
			::OffsetRect(&rc, 2, -1);

			return paint::stylus.draw_icon(dc, &rc, palette, part_id, state_id, paint::c_symbol, 0xE015, 900);
		}

		HRESULT on_draw_theme_background(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, LPCRECT rc_clip) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), ({/})", theme, dc, part_id, state_id, safe_string(rc), safe_string(rc_clip));

			switch (part_id)
			{
			case 0:
			case TP_BUTTON:
			case TP_DROPDOWNBUTTON:
				{
					if (state_id)
					{
						// ボタンを描画します。
						if (paint::stylus.draw_round_rect(dc, rc, palette, part_id, state_id))
							return S_OK;
					}
					else
					{
						// 背景を描画します。
						if (paint::stylus.draw_rect(dc, rc, palette, part_id, state_id))
							return S_OK;
					}

					break;
				}
			case TP_SPLITBUTTON:
			case TP_SEPARATOR:
			case TP_SEPARATORVERT:
				{
					// ボタンを描画します。
					if (paint::stylus.draw_rect(dc, rc, palette, part_id, state_id))
						return S_OK;

					break;
				}
			case TP_SPLITBUTTONDROPDOWN:
				{
					// ドロップダウンを描画します。

					if (draw_drop_down_button(dc, rc, part_id, state_id))
						return S_OK;

					break;
				}
			case TP_DROPDOWNBUTTONGLYPH:
				{
					// ドロップダウングリフを描画します。

					if (draw_drop_down_button_glyph(dc, rc, part_id, state_id))
						return S_OK;

					break;
				}
			}

			return hive.orig.DrawThemeBackground(theme, dc, part_id, state_id, rc, rc_clip);
		}

		HRESULT on_draw_theme_background_ex(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, const DTBGOPTS* options) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), {/hex}", theme, dc, part_id, state_id, safe_string(rc), options);

			return hive.orig.DrawThemeBackgroundEx(theme, dc, part_id, state_id, rc, options);
		}

		HRESULT on_draw_theme_text(HTHEME theme, HDC dc, int part_id, int state_id, LPCWSTR text, int c, DWORD text_flags, DWORD text_flags2, LPCRECT rc) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/hex}, {/hex}, ({/})", theme, dc, part_id, state_id, safe_string(text, c), text_flags, text_flags2, safe_string(rc));

			if (!(text_flags & DT_CALCRECT))
			{
				if (paint::stylus.draw_text(dc, rc, text, c, text_flags, palette, part_id, state_id))
					return S_OK;
			}

			return hive.orig.DrawThemeText(theme, dc, part_id, state_id, text, c, text_flags, text_flags2, rc);
		}

		HRESULT on_draw_theme_text_ex(HTHEME theme, HDC dc, int part_id, int state_id, LPCWSTR text, int c, DWORD text_flags, LPRECT rc, const DTTOPTS* options) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/hex}, ({/}), {/hex}", theme, dc, part_id, state_id, safe_string(text, c), text_flags, safe_string(rc), options);

			return hive.orig.DrawThemeTextEx(theme, dc, part_id, state_id, text, c, text_flags, rc, options);
		}

		HRESULT on_draw_theme_icon(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, HIMAGELIST image_list, int image_index) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), {/hex}, {/}", theme, dc, part_id, state_id, safe_string(rc), image_list, image_index);

			return hive.orig.DrawThemeIcon(theme, dc, part_id, state_id, rc, image_list, image_index);
		}

		HRESULT on_draw_theme_edge(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT dest_rect, UINT edge, UINT flags, LPRECT content_rect) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), {/hex}, {/hex}", theme, dc, part_id, state_id, safe_string(dest_rect), edge, flags);

			return hive.orig.DrawThemeEdge(theme, dc, part_id, state_id, dest_rect, edge, flags, content_rect);
		}
	} toolbar_renderer;
}
