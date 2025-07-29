#pragma once

namespace apn::dark::kuro::theme::immersive_start
{
	//
	// このクラスはメニューのテーマをフックしてカスタム描画を実行します。
	//
	inline struct MenuRenderer : base::MenuRenderer
	{
		virtual HRESULT on_draw_theme_background(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, LPCRECT rc_clip) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), ({/})", theme, dc, part_id, state_id, safe_string(rc), safe_string(rc_clip));

			{
				switch (part_id)
				{
				case MENU_POPUPSEPARATOR:
					{
						// ポップアップメニューのセパレータ用の描画です。

						auto rc2 = *rc;
						::InflateRect(&rc2, 0, -3);

						if (draw_rect(dc, &rc2, palette, part_id, state_id))
							return S_OK;

						break;
					}
				}

				if (draw_rect(dc, rc, palette, part_id, state_id))
					return S_OK;
			}

			return __super::on_draw_theme_background(theme, dc, part_id, state_id, rc, rc_clip);
		}

		virtual HRESULT on_draw_theme_text_ex(HTHEME theme, HDC dc, int part_id, int state_id, LPCWSTR text, int c, DWORD text_flags, LPRECT rc, const DTTOPTS* options) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/hex}, ({/}), {/hex}", theme, dc, part_id, state_id, safe_string(text, c), text_flags, safe_string(rc), options);

			if (!(text_flags & DT_CALCRECT))
			{
				if (auto pigment = palette.get(part_id, state_id))
				{
					auto options2 = *options;
					options2.dwFlags |= DTT_TEXTCOLOR;
					options2.crText = pigment->text.color;
					return __super::on_draw_theme_text_ex(theme, dc, part_id, state_id, text, c, text_flags, rc, &options2);
				}
			}

			return __super::on_draw_theme_text_ex(theme, dc, part_id, state_id, text, c, text_flags, rc, options);
		}
	} menu_renderer;
}
