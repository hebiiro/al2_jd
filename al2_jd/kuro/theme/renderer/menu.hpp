#pragma once

namespace apn::dark::kuro::theme
{
	//
	// このクラスはメニューのテーマをフックしてカスタム描画を実行します。
	//
	inline struct MenuRenderer : base::MenuRenderer
	{
		//
		// ポップアップメニューのガーターの右側の座標です。
		//
		int gutter_right = {};

		virtual HRESULT on_draw_theme_background(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, LPCRECT rc_clip) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), ({/})", theme, dc, part_id, state_id, safe_string(rc), safe_string(rc_clip));

			const auto& menu_palette = paint::menu_material.palette;

			if (auto pigment = menu_palette.get(part_id, state_id))
			{
				Clipper clipper(dc, rc_clip);

				switch (part_id)
				{
				case MENU_BARBACKGROUND:
					{
						// メニューバーの背景を描画します。

						auto rc2 = *rc;
						rc2.bottom += 1;

						return (ToHRESULT)paint::stylus.draw_rect(dc, &rc2, pigment);
					}
#if 0
				case MENU_POPUPBACKGROUND:
					{
						// ポップアップメニューの背景を描画します。

						auto rc2 = *rc;

						return (ToHRESULT)paint::stylus.draw_rect(dc, &rc2, pigment);
					}
				case MENU_POPUPBORDERS:
					{
						// ポップアップメニューのボーダーを描画します。

						auto rc2 = *rc;

						return (ToHRESULT)paint::stylus.draw_rect(dc, &rc2, pigment);
					}
#endif
				case MENU_POPUPGUTTER:
					{
						// ポップアップメニューの背景左側を描画します。

						auto rc2 = *rc;

						// 後で使うので、ガーターの位置を記憶しておきます。
						gutter_right = rc2.right;

						return (ToHRESULT)paint::stylus.draw_rect(dc, &rc2, pigment);
					}
				case MENU_POPUPSEPARATOR:
					{
						// ポップアップメニューのセパレータを描画します。

						auto rc2 = *rc;
						::InflateRect(&rc2, 0, -2);

						return (ToHRESULT)paint::stylus.draw_rect(dc, &rc2, pigment);
					}
				case MENU_POPUPITEM:
				case MENU_POPUPITEMFOCUSABLE:
					{
						// ポップアップメニューのアイテムを描画します。

						if (state_id != MPI_HOT)
						{
							auto rc2 = *rc;

							// ガーターが上書きされないように描画矩形を縮小します。
							rc2.left = gutter_right;

							return (ToHRESULT)paint::stylus.draw_rect(dc, &rc2, pigment);
						}

						break;
					}
				case MENU_POPUPCHECK:
					{
						switch (state_id)
						{
						case MC_CHECKMARKNORMAL:
						case MC_CHECKMARKDISABLED: return draw_icon(dc, rc, pigment, L"メイリオ", L'✅');
						case MC_BULLETNORMAL:
						case MC_BULLETDISABLED: return draw_icon(dc, rc, pigment, L"メイリオ", L'⬤');
						}

						break;
					}
				case MENU_POPUPSUBMENU:
					{
						auto rc2 = *rc;
						::InflateRect(&rc2, 6, 6);
						::OffsetRect(&rc2, 0, -2);

						return draw_icon(dc, &rc2, pigment, L"Meiryo", 0xE013);
					}
				case MENU_SYSTEMCLOSE: return draw_icon(dc, rc, pigment, L"Webdings", 0x0072);
				case MENU_SYSTEMMAXIMIZE: return draw_icon(dc, rc, pigment, L"Webdings", 0x0031);
				case MENU_SYSTEMMINIMIZE: return draw_icon(dc, rc, pigment, L"Webdings", 0x0030);
				case MENU_SYSTEMRESTORE: return draw_icon(dc, rc, pigment, L"Webdings", 0x0032);
				}

				return (ToHRESULT)paint::stylus.draw_rect(dc, rc, pigment);
			}

			return __super::on_draw_theme_background(theme, dc, part_id, state_id, rc, rc_clip);
		}

		virtual HRESULT on_draw_theme_background_ex(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, const DTBGOPTS* options) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), {/hex}", theme, dc, part_id, state_id, safe_string(rc), options);

			return __super::on_draw_theme_background_ex(theme, dc, part_id, state_id, rc, options);
		}

		virtual HRESULT on_draw_theme_text(HTHEME theme, HDC dc, int part_id, int state_id, LPCWSTR text, int c, DWORD text_flags, DWORD text_flags2, LPCRECT rc) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/hex}, {/hex}, ({/})", theme, dc, part_id, state_id, safe_string(text, c), text_flags, text_flags2, safe_string(rc));

			if (!(text_flags & DT_CALCRECT))
			{
				const auto& menu_palette = paint::menu_material.palette;

				if (auto pigment = menu_palette.get(part_id, state_id))
					return (ToHRESULT)paint::stylus.draw_text(dc, rc, text, c, text_flags, pigment);
			}

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
	} menu_renderer;
}
