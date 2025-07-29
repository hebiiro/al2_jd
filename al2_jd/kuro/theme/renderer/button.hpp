#pragma once

namespace apn::dark::kuro::theme
{
	//
	// このクラスはボタンのテーマをフックしてカスタム描画を実行します。
	//
	inline struct ButtonRenderer : Renderer
	{
		const paint::Palette& palette = paint::button_material.palette;

		int get_my_state_id(int part_id, int state_id)
		{
			switch (part_id)
			{
			case BP_RADIOBUTTON:
				{
					switch (state_id)
					{
						case RBS_UNCHECKEDNORMAL: return PBS_NORMAL;
						case RBS_UNCHECKEDDISABLED: return PBS_DISABLED;
						case RBS_UNCHECKEDHOT: return PBS_HOT;
						case RBS_UNCHECKEDPRESSED: return PBS_PRESSED;
						case RBS_CHECKEDNORMAL: return PBS_NORMAL;
						case RBS_CHECKEDDISABLED: return PBS_DISABLED;
						case RBS_CHECKEDHOT: return PBS_HOT;
						case RBS_CHECKEDPRESSED: return PBS_PRESSED;
					}

					break;
				}
			case BP_CHECKBOX:
				{
					switch (state_id)
					{
						case CBS_UNCHECKEDNORMAL: return PBS_NORMAL;
						case CBS_UNCHECKEDDISABLED: return PBS_DISABLED;
						case CBS_UNCHECKEDHOT: return PBS_HOT;
						case CBS_UNCHECKEDPRESSED: return PBS_PRESSED;
						case CBS_CHECKEDNORMAL: return PBS_NORMAL;
						case CBS_CHECKEDDISABLED: return PBS_DISABLED;
						case CBS_CHECKEDHOT: return PBS_HOT;
						case CBS_CHECKEDPRESSED: return PBS_PRESSED;
					}

					break;
				}
			}

			return 0;
		}

		//
		// プッシュボタンを描画します。
		//
		BOOL draw_push_button(HDC dc, LPCRECT arg_rc, int part_id, int state_id, BOOL pressed)
		{
			return draw_rect(dc, arg_rc, palette, part_id, state_id);
		}

		//
		// ラジオボタンを描画します。
		//
		BOOL draw_radio_button(HDC dc, LPCRECT arg_rc, int part_id, int state_id, BOOL pressed, BOOL checked)
		{
			if (checked)
				return draw_icon(dc, arg_rc, palette, part_id, state_id, L"メイリオ", 0xE005);
			else
				return draw_icon(dc, arg_rc, palette, part_id, state_id, L"メイリオ", 0xE002);
		}

		//
		// チェックボックスを描画します。
		//
		BOOL draw_checkbox(HDC dc, LPCRECT arg_rc, int part_id, int state_id, BOOL pressed, BOOL checked)
		{
			if (checked)
				return draw_icon(dc, arg_rc, palette, part_id, state_id, L"メイリオ", 0xE005);
			else
				return draw_icon(dc, arg_rc, palette, part_id, state_id, L"メイリオ", 0xE002);
		}

		//
		// グループボックスを描画します。
		//
		BOOL draw_groupbox(HDC dc, LPCRECT arg_rc, int part_id, int state_id)
		{
			return paint::draw_etched_edge(dc, arg_rc), TRUE;
		}

		HRESULT on_draw_theme_background(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, LPCRECT rc_clip) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), ({/})", theme, dc, part_id, state_id, safe_string(rc), safe_string(rc_clip));

			switch (part_id)
			{
			case BP_PUSHBUTTON:
				{
					auto pressed = FALSE;

					switch (state_id)
					{
					case PBS_NORMAL: pressed = FALSE; break;
					case PBS_DISABLED: pressed = FALSE; break;
					case PBS_DEFAULTED: pressed = FALSE; break;
					case PBS_DEFAULTED_ANIMATING: pressed = FALSE; break;
					case PBS_HOT: pressed = FALSE; break;
					case PBS_PRESSED: pressed = TRUE; break;
					}

					if (draw_push_button(dc, rc, part_id, state_id, pressed))
						return S_OK;

					break;
				}
			case BP_RADIOBUTTON:
				{
					auto pressed = FALSE;
					auto checked = FALSE;

					switch (state_id)
					{
					case RBS_UNCHECKEDNORMAL: pressed = FALSE; checked = FALSE; break;
					case RBS_UNCHECKEDDISABLED: pressed = FALSE; checked = FALSE; break;
					case RBS_UNCHECKEDHOT: pressed = FALSE; checked = FALSE; break;
					case RBS_UNCHECKEDPRESSED: pressed = TRUE; checked = FALSE; break;

					case RBS_CHECKEDNORMAL: pressed = FALSE; checked = TRUE; break;
					case RBS_CHECKEDDISABLED: pressed = FALSE; checked = TRUE; break;
					case RBS_CHECKEDHOT: pressed = FALSE; checked = TRUE; break;
					case RBS_CHECKEDPRESSED: pressed = TRUE; checked = TRUE; break;
					}

					if (draw_radio_button(dc, rc, part_id, state_id, pressed, checked))
						return S_OK;

					break;
				}
			case BP_CHECKBOX:
				{
					auto pressed = FALSE;
					auto checked = FALSE;

					switch (state_id)
					{
					case CBS_UNCHECKEDNORMAL: pressed = FALSE; checked = FALSE; break;
					case CBS_UNCHECKEDDISABLED: pressed = FALSE; checked = FALSE; break;
					case CBS_UNCHECKEDHOT: pressed = FALSE; checked = FALSE; break;
					case CBS_UNCHECKEDPRESSED: pressed = TRUE; checked = FALSE; break;

					case CBS_CHECKEDNORMAL: pressed = FALSE; checked = TRUE; break;
					case CBS_CHECKEDDISABLED: pressed = FALSE; checked = TRUE; break;
					case CBS_CHECKEDHOT: pressed = FALSE; checked = TRUE; break;
					case CBS_CHECKEDPRESSED: pressed = TRUE; checked = TRUE; break;
					}

					if (draw_checkbox(dc, rc, part_id, state_id, pressed, checked))
						return S_OK;

					break;
				}
			case BP_GROUPBOX:
				{
					switch (state_id)
					{
					case GBS_NORMAL:
					case GBS_DISABLED:
						{
							if (draw_groupbox(dc, rc, part_id, state_id))
								return S_OK;

							break;
						}
					}

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
				if (draw_text(dc, rc, text, c, text_flags, palette, part_id, state_id))
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
	} button_renderer;
}
