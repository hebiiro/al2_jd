#pragma once

namespace apn::dark::kuro::theme
{
	//
	// このクラスはスクロールバーのテーマをフックしてカスタム描画を実行します。
	//
	inline struct ScrollBarRenderer : Renderer
	{
		//
		// スクロールバーの太さの倍率を返します。
		//
		float get_ratio()
		{
			return (100 - hive.scrollbar_reduction) / 200.0f;
		}

		//
		// スクロールバーの背景を描画します。
		//
		BOOL draw_background(HDC dc, LPCRECT arg_rc, const auto& palette, int part_id, int state_id)
		{
			if (auto pigment = palette.get(part_id, state_id))
				return paint::stylus.draw_rect(dc, arg_rc, pigment);

			return FALSE;
		}

		//
		// 水平方向の矢印を描画します。
		//
		BOOL draw_horz_arrow(HDC dc, LPCRECT arg_rc, int part_id, int state_id, BOOL near_arrow)
		{
			const auto& palette = paint::scrollbar_material.palette;

			draw_background(dc, arg_rc, palette, 0, 0);

			if (auto pigment = palette.get(part_id, state_id))
			{
				if (paint::scrollbar_material.arrow_as_button)
				{
					auto rc = *arg_rc;
					auto ratio = get_ratio();
					::InflateRect(&rc, -int(my::get_width(rc) * ratio), -int(my::get_height(rc) * ratio));
					paint::stylus.draw_rect(dc, &rc, pigment);
				}

				return paint::stylus.draw_icon(dc, arg_rc, pigment, L"メイリオ", near_arrow ? 0xE012 : 0xE013);
			}

			return FALSE;
		}

		//
		// 垂直方向の矢印を描画します。
		//
		BOOL draw_vert_arrow(HDC dc, LPCRECT arg_rc, int part_id, int state_id, BOOL near_arrow)
		{
			const auto& palette = paint::scrollbar_material.palette;

			draw_background(dc, arg_rc, palette, 0, 0);

			if (auto pigment = palette.get(part_id, state_id))
			{
				if (paint::scrollbar_material.arrow_as_button)
				{
					auto rc = *arg_rc;
					auto ratio = get_ratio();
					::InflateRect(&rc, -int(my::get_width(rc) * ratio), -int(my::get_height(rc) * ratio));
					paint::stylus.draw_rect(dc, &rc, pigment);
				}

				return paint::stylus.draw_icon(dc, arg_rc, pigment, L"メイリオ", near_arrow ? 0xE014 : 0xE015);
			}

			return FALSE;
		}

		//
		// 左矢印を描画します。
		//
		BOOL draw_left(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			return draw_horz_arrow(dc, arg_rc, part_id, state_id, TRUE);
		}

		//
		// 右矢印を描画します。
		//
		BOOL draw_right(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			return draw_horz_arrow(dc, arg_rc, part_id, state_id, FALSE);
		}

		//
		// 上矢印を描画します。
		//
		BOOL draw_up(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			return draw_vert_arrow(dc, arg_rc, part_id, state_id, TRUE);
		}

		//
		// 下矢印を描画します。
		//
		BOOL draw_down(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			return draw_vert_arrow(dc, arg_rc, part_id, state_id, FALSE);
		}

		//
		// 水平方向のつまみを描画します。
		//
		BOOL draw_horz_thumb(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			const auto& palette = paint::scrollbar_material.palette;

			draw_background(dc, arg_rc, palette, 0, 0);

			if (auto pigment = palette.get(part_id, state_id))
			{
				auto rc = *arg_rc;
				auto ratio = get_ratio();
				::InflateRect(&rc, 0, -int(my::get_height(rc) * ratio));
				return paint::stylus.draw_rect(dc, &rc, pigment);
			}

			return FALSE;
		}

		//
		// 垂直方向のつまみを描画します。
		//
		BOOL draw_vert_thumb(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			const auto& palette = paint::scrollbar_material.palette;

			draw_background(dc, arg_rc, palette, 0, 0);

			if (auto pigment = palette.get(part_id, state_id))
			{
				auto rc = *arg_rc;
				auto ratio = get_ratio();
				::InflateRect(&rc, -int(my::get_width(rc) * ratio), 0);
				return paint::stylus.draw_rect(dc, &rc, pigment);
			}

			return FALSE;
		}

		//
		// 水平方向の下層トラックを描画します。
		//
		BOOL draw_horz_lower_track(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			return draw_background(dc, arg_rc, paint::scrollbar_material.palette, 0, 0);
		}

		//
		// 水平方向の上層トラックを描画します。
		//
		BOOL draw_horz_upper_track(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			return draw_background(dc, arg_rc, paint::scrollbar_material.palette, 0, 0);
		}

		//
		// 垂直方向の下層トラックを描画します。
		//
		BOOL draw_vert_lower_track(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			return draw_background(dc, arg_rc, paint::scrollbar_material.palette, 0, 0);
		}

		//
		// 垂直方向の上層トラックを描画します。
		//
		BOOL draw_vert_upper_track(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			return draw_background(dc, arg_rc, paint::scrollbar_material.palette, 0, 0);
		}

		//
		// サイズ変更グリップを描画します。
		//
		BOOL draw_sizebox(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			// 背景はダイアログのマテリアルで描画します。
			{
				const auto& palette = paint::dialog_material.palette;

				auto part_id = WP_DIALOG;
				auto state_id = ETS_NORMAL;

				draw_background(dc, arg_rc, palette, part_id, state_id);
			}

			const auto& palette = paint::scrollbar_material.palette;

			if (auto pigment = palette.get(part_id, state_id))
				return paint::stylus.draw_icon(dc, arg_rc, pigment, L"Marlett", 0x006F);

			return FALSE;
		}

		virtual HRESULT on_get_theme_color(HTHEME theme, int part_id, int state_id, int prop_id, COLORREF* result) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}", theme, part_id, state_id, prop_id);

			return __super::on_get_theme_color(theme, part_id, state_id, prop_id, result);
		}

		virtual HRESULT on_draw_theme_background(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, LPCRECT rc_clip) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), ({/})", theme, dc, part_id, state_id, safe_string(rc), safe_string(rc_clip));

			switch (part_id)
			{
			case SBP_ARROWBTN:
				{
					switch (state_id)
					{
					case ABS_LEFTNORMAL:
					case ABS_LEFTHOVER:
					case ABS_LEFTHOT:
					case ABS_LEFTPRESSED:
					case ABS_LEFTDISABLED:
						return (ToHRESULT)draw_left(dc, rc, part_id, state_id);

					case ABS_RIGHTNORMAL:
					case ABS_RIGHTHOVER:
					case ABS_RIGHTHOT:
					case ABS_RIGHTPRESSED:
					case ABS_RIGHTDISABLED:
						return (ToHRESULT)draw_right(dc, rc, part_id, state_id);

					case ABS_UPNORMAL:
					case ABS_UPHOVER:
					case ABS_UPHOT:
					case ABS_UPPRESSED:
					case ABS_UPDISABLED:
						return (ToHRESULT)draw_up(dc, rc, part_id, state_id);

					case ABS_DOWNNORMAL:
					case ABS_DOWNHOVER:
					case ABS_DOWNHOT:
					case ABS_DOWNPRESSED:
					case ABS_DOWNDISABLED:
						return (ToHRESULT)draw_down(dc, rc, part_id, state_id);
					}

					break;
				}
			case SBP_THUMBBTNHORZ: return (ToHRESULT)draw_horz_thumb(dc, rc, part_id, state_id);
			case SBP_THUMBBTNVERT: return (ToHRESULT)draw_vert_thumb(dc, rc, part_id, state_id);
			case SBP_LOWERTRACKHORZ: return (ToHRESULT)draw_horz_lower_track(dc, rc, part_id, state_id);
			case SBP_UPPERTRACKHORZ: return (ToHRESULT)draw_horz_upper_track(dc, rc, part_id, state_id);
			case SBP_LOWERTRACKVERT: return (ToHRESULT)draw_vert_lower_track(dc, rc, part_id, state_id);
			case SBP_UPPERTRACKVERT: return (ToHRESULT)draw_vert_upper_track(dc, rc, part_id, state_id);
#if 0 // ダイアログをダークモード化した場合はここをオンにします。
			case SBP_SIZEBOX: return (ToHRESULT)draw_sizebox(dc, rc, part_id, state_id);
#endif
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
	} scrollbar_renderer;
}
