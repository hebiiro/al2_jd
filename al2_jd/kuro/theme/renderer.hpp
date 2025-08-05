#pragma once

namespace apn::dark::kuro::theme
{
	//
	// パレットを使用して矩形を描画します。
	//
	inline BOOL draw_rect(HDC dc, LPCRECT rc,
		const paint::Palette& palette, int part_id, int state_id)
	{
		if (auto pigment = palette.get(part_id, state_id))
			return paint::stylus.draw_rect(dc, rc, pigment);

		return FALSE;
	}

	//
	// パレットを使用して文字列を描画します。
	//
	inline BOOL ext_text_out(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx,
		const paint::Palette& palette, int part_id, int state_id, BOOL opaque = TRUE)
	{
		if (auto pigment = palette.get(part_id, state_id))
			return paint::stylus.ext_text_out(dc, x, y, options, rc, text, c, dx, pigment, opaque);

		return FALSE;
	}

	//
	// パレットを使用して文字列を描画します。
	//
	inline BOOL draw_text(HDC dc, LPCRECT rc, LPCWSTR text, int c, DWORD text_flags,
		const paint::Palette& palette, int part_id, int state_id, BOOL opaque = TRUE)
	{
		if (auto pigment = palette.get(part_id, state_id))
			return paint::stylus.draw_text(dc, rc, text, c, text_flags, pigment, opaque);

		return FALSE;
	}

	//
	// パレットを使用して絵文字を描画します。
	//
	inline BOOL draw_icon(HDC dc, LPCRECT rc,
		const paint::Palette& palette, int part_id, int state_id,
		LPCWSTR font_name, WCHAR char_code, int font_weight = 0)
	{
		if (auto pigment = palette.get(part_id, state_id))
			return paint::stylus.draw_icon(dc, rc, pigment, font_name, char_code, font_weight);

		return FALSE;
	}

	//
	// このクラスはテーマをフックしてカスタム描画を実行します。
	//
	struct Renderer
	{
		//
		// GDIフックのロックカウントです。
		//
		inline static thread_local int gdi_hook_lock_count = 0;

		//
		// GDIフックがロックされていない場合はTRUEを返します。
		//
		inline static BOOL is_gdi_hook_locked() { return gdi_hook_lock_count != 0; }

		//
		// このクラスはGDIフックをロックします。
		//
		struct GdiHookLocker {
			GdiHookLocker() { MY_TRACE("GDIフックのロックを開始します\n"); gdi_hook_lock_count++; }
			~GdiHookLocker() { MY_TRACE("GDIフックのロックを終了します\n"); gdi_hook_lock_count--; }
		};

		virtual HRESULT on_get_theme_color(HTHEME theme, int part_id, int state_id, int prop_id, COLORREF* result)
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}", theme, part_id, state_id, prop_id);

			return hive.orig.GetThemeColor(theme, part_id, state_id, prop_id, result);
		}

		virtual HRESULT on_draw_theme_background(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, LPCRECT rc_clip)
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), ({/})", theme, dc, part_id, state_id, safe_string(rc), safe_string(rc_clip));

			return hive.orig.DrawThemeBackground(theme, dc, part_id, state_id, rc, rc_clip);
		}

		virtual HRESULT on_draw_theme_background_ex(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, const DTBGOPTS* options)
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), {/hex}", theme, dc, part_id, state_id, safe_string(rc), options);

			return hive.orig.DrawThemeBackgroundEx(theme, dc, part_id, state_id, rc, options);
		}

		virtual HRESULT on_draw_theme_text(HTHEME theme, HDC dc, int part_id, int state_id, LPCWSTR text, int c, DWORD text_flags, DWORD text_flags2, LPCRECT rc)
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/hex}, {/hex}, ({/})", theme, dc, part_id, state_id, safe_string(text, c), text_flags, text_flags2, safe_string(rc));

			return hive.orig.DrawThemeText(theme, dc, part_id, state_id, text, c, text_flags, text_flags2, rc);
		}

		virtual HRESULT on_draw_theme_text_ex(HTHEME theme, HDC dc, int part_id, int state_id, LPCWSTR text, int c, DWORD text_flags, LPRECT rc, const DTTOPTS* options)
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/hex}, ({/}), {/hex}", theme, dc, part_id, state_id, safe_string(text, c), text_flags, safe_string(rc), options);

			return hive.orig.DrawThemeTextEx(theme, dc, part_id, state_id, text, c, text_flags, rc, options);
		}

		virtual HRESULT on_draw_theme_icon(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, HIMAGELIST image_list, int image_index)
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), {/hex}, {/}", theme, dc, part_id, state_id, safe_string(rc), image_list, image_index);

			return hive.orig.DrawThemeIcon(theme, dc, part_id, state_id, rc, image_list, image_index);
		}

		virtual HRESULT on_draw_theme_edge(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT dest_rect, UINT edge, UINT flags, LPRECT content_rect)
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, ({/}), {/hex}, {/hex}", theme, dc, part_id, state_id, safe_string(dest_rect), edge, flags);

			return hive.orig.DrawThemeEdge(theme, dc, part_id, state_id, dest_rect, edge, flags, content_rect);
		}
	};
}
