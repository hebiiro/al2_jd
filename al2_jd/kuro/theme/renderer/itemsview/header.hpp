#pragma once

namespace apn::dark::kuro::theme::itemsview
{
	//
	// このクラスはヘッダーコントロールのテーマをフックしてカスタム描画を実行します。
	//
	inline struct HeaderRenderer : base::HeaderRenderer
	{
		virtual HRESULT on_get_theme_color(HTHEME theme, int part_id, int state_id, int prop_id, COLORREF* result) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}", theme, part_id, state_id, prop_id);

			const auto& header_palette = paint::header_material.palette;

			if (auto pigment = header_palette.get(part_id, state_id))
			{
				switch (prop_id)
				{
				case TMT_TEXTCOLOR: return *result = pigment->text.color, S_OK;
				}
			}

			return __super::on_get_theme_color(theme, part_id, state_id, prop_id, result);
		}

		virtual HRESULT on_draw_theme_background(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, LPCRECT rc_clip) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), ({/})", theme, dc, part_id, state_id, safe_string(rc), safe_string(rc_clip));

			return __super::on_draw_theme_background(theme, dc, part_id, state_id, rc, rc_clip);
		}

		virtual HRESULT on_draw_theme_background_ex(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, const DTBGOPTS* options) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), {/hex}", theme, dc, part_id, state_id, safe_string(rc), options);

			const auto& header_palette = paint::header_material.palette;

			if (auto pigment = header_palette.get(part_id, state_id))
			{
				switch (part_id)
				{
				case HP_HEADERITEMLEFT:
				case HP_HEADERITEMRIGHT:
				case HP_HEADERDROPDOWN:
				case HP_HEADERDROPDOWNFILTER: return draw_drop_down_button(dc, rc, pigment);
				case HP_HEADERSORTARROW:
					{
						switch (state_id)
						{
						case HSAS_SORTEDUP: return draw_sort_button(dc, rc, pigment, L"メイリオ", 0xE014);
						case HSAS_SORTEDDOWN: return draw_sort_button(dc, rc, pigment, L"メイリオ", 0xE015);
//						case HSAS_SORTEDUP: return draw_sort_button(dc, rc, pigment, L"Webdings", 0x0036);
//						case HSAS_SORTEDDOWN: return draw_sort_button(dc, rc, pigment, L"Webdings", 0x0035);
						}

						break;
					}
				}

				return (ToHRESULT)paint::stylus.draw_rect(dc, rc, pigment);
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
