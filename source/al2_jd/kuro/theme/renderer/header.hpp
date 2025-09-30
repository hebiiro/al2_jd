#pragma once

namespace apn::dark::kuro::theme
{
	//
	// このクラスはヘッダーコントロールのテーマをフックしてカスタム描画を実行します。
	//
	inline struct HeaderRenderer : Renderer
	{
		const paint::Palette& palette = paint::header_material.palette;

		//
		// ドロップダウンボタンを描画します。
		//
		BOOL draw_drop_down_button(HDC dc, LPCRECT rc, int part_id, int state_id)
		{
			auto rc2 = *rc;
			auto width = my::get_width(rc2);
			rc2.top = (rc2.top + rc2.bottom - width) / 2;
			rc2.bottom = rc2.top + width;

			return paint::stylus.draw_icon(dc, &rc2, palette, part_id, state_id, paint::c_symbol, 0xE015);
		}

		//
		// ソートボタンを描画します。
		//
		BOOL draw_sort_button(HDC dc, LPCRECT rc, int part_id, int state_id, LPCWSTR font_name, WCHAR char_code)
		{
			auto dpi = ::GetDpiForSystem();

			auto rc2 = *rc;
			rc2.top -= ::MulDiv(2, dpi, USER_DEFAULT_SCREEN_DPI);
			rc2.bottom += ::MulDiv(6, dpi, USER_DEFAULT_SCREEN_DPI);

			return paint::stylus.draw_icon(dc, &rc2, palette, part_id, state_id, font_name, char_code);
		}

		//
		// ヘッダー矩形を描画します。
		//
		BOOL draw_header(HDC dc, LPCRECT rc, int part_id, int state_id)
		{
			switch (part_id)
			{
			case HP_HEADERITEMLEFT:
			case HP_HEADERITEMRIGHT:
			case HP_HEADERDROPDOWN:
			case HP_HEADERDROPDOWNFILTER: return draw_drop_down_button(dc, rc, part_id, state_id);
			case HP_HEADERSORTARROW:
				{
					switch (state_id)
					{
					case HSAS_SORTEDUP: return draw_sort_button(dc, rc, part_id, state_id, paint::c_symbol, 0xE014);
					case HSAS_SORTEDDOWN: return draw_sort_button(dc, rc, part_id, state_id, paint::c_symbol, 0xE015);
//					case HSAS_SORTEDUP: return draw_sort_button(dc, rc, part_id, state_id, L"Webdings", 0x0036);
//					case HSAS_SORTEDDOWN: return draw_sort_button(dc, rc, part_id, state_id, L"Webdings", 0x0035);
					}

					break;
				}
			}

			return paint::stylus.draw_rect(dc, rc, palette, part_id, state_id);
		}

		virtual HRESULT on_get_theme_color(HTHEME theme, int part_id, int state_id, int prop_id, COLORREF* result) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}", theme, part_id, state_id, prop_id);

			if (auto pigment = palette.get(part_id, state_id))
			{
				switch (prop_id)
				{
				case TMT_TEXTCOLOR: return *result = pigment->text.get_win32_color(), S_OK;
				}
			}

			return __super::on_get_theme_color(theme, part_id, state_id, prop_id, result);
		}

		virtual HRESULT on_draw_theme_background(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, LPCRECT rc_clip) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), ({/})", theme, dc, part_id, state_id, safe_string(rc), safe_string(rc_clip));

			{
				if (draw_header(dc, rc, part_id, state_id))
					return S_OK;
			}

			return __super::on_draw_theme_background(theme, dc, part_id, state_id, rc, rc_clip);
		}

		virtual HRESULT on_draw_theme_background_ex(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, const DTBGOPTS* options) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), {/hex}", theme, dc, part_id, state_id, safe_string(rc), options);

			{
				if (draw_header(dc, rc, part_id, state_id))
					return S_OK;
			}

			return __super::on_draw_theme_background_ex(theme, dc, part_id, state_id, rc, options);
		}

		virtual HRESULT on_draw_theme_text(HTHEME theme, HDC dc, int part_id, int state_id, LPCWSTR text, int c, DWORD text_flags, DWORD text_flags2, LPCRECT rc) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/hex}, {/hex}, ({/})", theme, dc, part_id, state_id, safe_string(text, c), text_flags, text_flags2, safe_string(rc));

			return __super::on_draw_theme_text(theme, dc, part_id, state_id, text, c, text_flags, text_flags2, rc);
		}

		virtual HRESULT on_draw_theme_text_ex(HTHEME theme, HDC dc, int part_id, int state_id, LPCWSTR text, int c, DWORD text_flags, LPRECT rc, const DTTOPTS* options) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/hex}, ({/}), {/hex}", theme, dc, part_id, state_id, safe_string(text, c), text_flags, safe_string(rc), options);

			if (!(text_flags & DT_CALCRECT))
			{
				if (paint::stylus.draw_text(dc, rc, text, c, text_flags, palette, part_id, state_id))
					return S_OK;
			}

			return __super::on_draw_theme_text_ex(theme, dc, part_id, state_id, text, c, text_flags, rc, options);
		}

		virtual HRESULT on_draw_theme_icon(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, HIMAGELIST image_list, int image_index) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), {/hex}, {/}", theme, dc, part_id, state_id, safe_string(rc), image_list, image_index);

			return __super::on_draw_theme_icon(theme, dc, part_id, state_id, rc, image_list, image_index);
		}

		virtual HRESULT on_draw_theme_edge(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT dest_rect, UINT edge, UINT flags, LPRECT content_rect) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), {/hex}, {/hex}", theme, dc, part_id, state_id, safe_string(dest_rect), edge, flags);

			return __super::on_draw_theme_edge(theme, dc, part_id, state_id, dest_rect, edge, flags, content_rect);
		}
	} header_renderer;
}
