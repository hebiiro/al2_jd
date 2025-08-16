#pragma once

namespace apn::dark::kuro::hook
{
	//
	// このクラスはテーマのフックを担当します。
	//
	inline struct Theme : Entry
	{
		//
		// 初期化処理を実行します。
		//
		virtual BOOL on_init() override
		{
			MY_TRACE_FUNC("");

			DetourTransactionBegin();
			DetourUpdateThread(::GetCurrentThread());

			auto dpi = ::GetDpiForSystem();
			MY_TRACE_INT(dpi);

			my::hook::attach(GetThemeColor);
#if 0
			my::hook::attach(GetThemeSysColor);
			my::hook::attach(GetThemeSysColorBrush);
			my::hook::attach(DrawThemeParentBackground);
#endif
#if 1
			my::hook::attach(DrawThemeBackground);
			my::hook::attach(DrawThemeBackgroundEx);
			my::hook::attach(DrawThemeText);
			my::hook::attach(DrawThemeTextEx);
#endif
#if 0
			my::hook::attach(DrawThemeIcon);
			my::hook::attach(DrawThemeEdge);
#endif
#if 1
			my::hook::attach(OpenThemeData);
			my::hook::attach(OpenThemeDataForDpi);
			my::hook::attach(OpenThemeDataEx);
			my::hook::attach(SetWindowTheme);
#endif
			if (DetourTransactionCommit() != NO_ERROR)
			{
				MY_TRACE("APIフックに失敗しました\n");

				return FALSE;
			}

			hive.orig.GetThemeColor = GetThemeColor.orig_proc;
			hive.orig.DrawThemeParentBackground = DrawThemeParentBackground.orig_proc;
			hive.orig.DrawThemeBackground = DrawThemeBackground.orig_proc;
			hive.orig.DrawThemeBackgroundEx = DrawThemeBackgroundEx.orig_proc;
			hive.orig.DrawThemeText = DrawThemeText.orig_proc;
			hive.orig.DrawThemeTextEx = DrawThemeTextEx.orig_proc;
			hive.orig.DrawThemeIcon = DrawThemeIcon.orig_proc;
			hive.orig.DrawThemeEdge = DrawThemeEdge.orig_proc;

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		virtual BOOL on_exit() override
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// このクラスは::GetThemeColor()をフックします。
		//
		struct {
			inline static HRESULT WINAPI hook_proc(HTHEME theme, int part_id, int state_id, int prop_id, COLORREF* result)
			{
				MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}, {/}",
					ret_addr(&theme), kuro::theme::name.get_safe(theme), part_id, state_id, prop_id);

				if (hive.is_valid_thread())
				{
					if (auto renderer = kuro::theme::from_handle.get(theme))
						return renderer->on_get_theme_color(theme, part_id, state_id, prop_id, result);
				}

				return orig_proc(theme, part_id, state_id, prop_id, result);
			}
			inline static decltype(&hook_proc) orig_proc = ::GetThemeColor;
		} GetThemeColor;

		//
		// このクラスは::GetThemeSysColor()をフックします。
		//
		struct {
			inline static COLORREF WINAPI hook_proc(HTHEME theme, int color_id)
			{
				MY_TRACE_FUNC("{/hex}, {/}, {/}",
					ret_addr(&theme), kuro::theme::name.get_safe(theme), color_id);

				return orig_proc(theme, color_id);
			}
			inline static decltype(&hook_proc) orig_proc = ::GetThemeSysColor;
		} GetThemeSysColor;

		//
		// このクラスは::GetThemeSysColorBrush()をフックします。
		//
		struct {
			inline static HBRUSH WINAPI hook_proc(HTHEME theme, int color_id)
			{
				MY_TRACE_FUNC("{/hex}, {/}, {/}",
					ret_addr(&theme), kuro::theme::name.get_safe(theme), color_id);

				return orig_proc(theme, color_id);
			}
			inline static decltype(&hook_proc) orig_proc = ::GetThemeSysColorBrush;
		} GetThemeSysColorBrush;

		//
		// このクラスは::DrawThemeParentBackground()をフックします。
		//
		struct {
			inline static HRESULT WINAPI hook_proc(HWND hwnd, HDC dc, LPCRECT rc)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, ({/})",
					ret_addr(&hwnd), hwnd, dc, safe_string(rc));

				MY_TRACE_HWND(hwnd);

				return orig_proc(hwnd, dc, rc);
			}
			inline static decltype(&hook_proc) orig_proc = ::DrawThemeParentBackground;
		} DrawThemeParentBackground;

		//
		// このクラスは::DrawThemeBackground()をフックします。
		//
		struct {
			inline static HRESULT WINAPI hook_proc(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, LPCRECT rc_clip)
			{
				MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}, ({/}), ({/})",
					ret_addr(&theme), kuro::theme::name.get_safe(theme), part_id, state_id,
					safe_string(rc), safe_string(rc_clip));

				if (hive.is_valid_thread())
				{
					if (auto renderer = kuro::theme::from_handle.get(theme))
						return renderer->on_draw_theme_background(theme, dc, part_id, state_id, rc, rc_clip);
				}

				return orig_proc(theme, dc, part_id, state_id, rc, rc_clip);
			}
			inline static decltype(&hook_proc) orig_proc = ::DrawThemeBackground;
		} DrawThemeBackground;

		//
		// このクラスは::DrawThemeBackgroundEx()をフックします。
		//
		struct {
			inline static HRESULT WINAPI hook_proc(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, const DTBGOPTS* options)
			{
				MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}, ({/}), {/hex}",
					ret_addr(&theme), kuro::theme::name.get_safe(theme), part_id, state_id,
					safe_string(rc), options);

				if (hive.is_valid_thread())
				{
					if (auto renderer = kuro::theme::from_handle.get(theme))
						return renderer->on_draw_theme_background_ex(theme, dc, part_id, state_id, rc, options);
				}

				return orig_proc(theme, dc, part_id, state_id, rc, options);
			}
			inline static decltype(&hook_proc) orig_proc = ::DrawThemeBackgroundEx;
		} DrawThemeBackgroundEx;

		//
		// このクラスは::DrawThemeText()をフックします。
		//
		struct {
			inline static HRESULT WINAPI hook_proc(HTHEME theme, HDC dc, int part_id, int state_id, LPCWSTR text, int c, DWORD text_flags, DWORD text_flags2, LPCRECT rc)
			{
				MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}, {/}, {/hex}, {/hex}, ({/})",
					ret_addr(&theme), kuro::theme::name.get_safe(theme), part_id, state_id,
					safe_string(text, c), text_flags, text_flags2, safe_string(rc));

				if (hive.is_valid_thread())
				{
					if (auto renderer = kuro::theme::from_handle.get(theme))
						return renderer->on_draw_theme_text(theme, dc, part_id, state_id, text, c, text_flags, text_flags2, rc);
				}

				return orig_proc(theme, dc, part_id, state_id, text, c, text_flags, text_flags2, rc);
			}
			inline static decltype(&hook_proc) orig_proc = ::DrawThemeText;
		} DrawThemeText;

		//
		// このクラスは::DrawThemeTextEx()をフックします。
		//
		struct {
			inline static HRESULT WINAPI hook_proc(HTHEME theme, HDC dc, int part_id, int state_id, LPCWSTR text, int c, DWORD text_flags, LPRECT rc, const DTTOPTS* options)
			{
				MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}, {/}, {/hex}, ({/}), {/hex}",
					ret_addr(&theme), kuro::theme::name.get_safe(theme), part_id, state_id,
					safe_string(text, c), text_flags, safe_string(rc), options);

				if (hive.is_valid_thread())
				{
					if (auto renderer = kuro::theme::from_handle.get(theme))
						return renderer->on_draw_theme_text_ex(theme, dc, part_id, state_id, text, c, text_flags, rc, options);
				}

				return orig_proc(theme, dc, part_id, state_id, text, c, text_flags, rc, options);
			}
			inline static decltype(&hook_proc) orig_proc = ::DrawThemeTextEx;
		} DrawThemeTextEx;

		//
		// このクラスは::DrawThemeIcon()をフックします。
		//
		struct {
			inline static HRESULT WINAPI hook_proc(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT rc, HIMAGELIST image_list, int image_index)
			{
				MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}, ({/}), {/hex}, {/}",
					ret_addr(&theme), kuro::theme::name.get_safe(theme), part_id, state_id,
					safe_string(rc), image_list, image_index);

				if (hive.is_valid_thread())
				{
					if (auto renderer = kuro::theme::from_handle.get(theme))
						return renderer->on_draw_theme_icon(theme, dc, part_id, state_id, rc, image_list, image_index);
				}

				return orig_proc(theme, dc, part_id, state_id, rc, image_list, image_index);
			}
			inline static decltype(&hook_proc) orig_proc = ::DrawThemeIcon;
		} DrawThemeIcon;

		//
		// このクラスは::DrawThemeEdge()をフックします。
		//
		struct {
			inline static HRESULT WINAPI hook_proc(HTHEME theme, HDC dc, int part_id, int state_id, LPCRECT dest_rect, UINT edge, UINT flags, LPRECT content_rect)
			{
				MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}, ({/}), {/hex}, {/hex}",
					ret_addr(&theme), kuro::theme::name.get_safe(theme), part_id, state_id,
					safe_string(dest_rect), edge, flags);

				if (hive.is_valid_thread())
				{
					if (auto renderer = kuro::theme::from_handle.get(theme))
						return renderer->on_draw_theme_edge(theme, dc, part_id, state_id, dest_rect, edge, flags, content_rect);
				}

				return orig_proc(theme, dc, part_id, state_id, dest_rect, edge, flags, content_rect);
			}
			inline static decltype(&hook_proc) orig_proc = ::DrawThemeEdge;
		} DrawThemeEdge;

		//
		// テーマハンドルにレンダラーを関連付けます。
		//
		inline static void attach_renderer(HTHEME theme, LPCWSTR class_list)
		{
			kuro::theme::name.set(theme, class_list);
			if (auto renderer  = kuro::theme::from_vsclass.get(class_list))
				kuro::theme::from_handle.set(theme, renderer);
		}

		//
		// このクラスは::OpenThemeData()をフックします。
		//
		struct {
			inline static HTHEME WINAPI hook_proc(HWND hwnd, LPCWSTR class_list)
			{
				auto theme = orig_proc(hwnd, class_list);
				MY_TRACE_FUNC("{/hex}, {/} => {/hex}", hwnd, class_list, theme);
				attach_renderer(theme, class_list);
				return theme;
			}
			inline static decltype(&hook_proc) orig_proc = ::OpenThemeData;
		} OpenThemeData;

		//
		// このクラスは::OpenThemeDataForDpi()をフックします。
		//
		struct {
			inline static HTHEME WINAPI hook_proc(HWND hwnd, LPCWSTR class_list, UINT dpi)
			{
				auto theme = orig_proc(hwnd, class_list, dpi);
				MY_TRACE_FUNC("{/hex}, {/}, {/} => {/hex}", hwnd, class_list, dpi, theme);
				attach_renderer(theme, class_list);
				return theme;
			}
			inline static decltype(&hook_proc) orig_proc = ::OpenThemeDataForDpi;
		} OpenThemeDataForDpi;

		//
		// このクラスは::OpenThemeDataEx()をフックします。
		//
		struct {
			inline static HTHEME WINAPI hook_proc(HWND hwnd, LPCWSTR class_list, DWORD flags)
			{
				auto theme = orig_proc(hwnd, class_list, flags);
				MY_TRACE_FUNC("{/hex}, {/}, {/hex} => {/hex}", hwnd, class_list, flags, theme);
				attach_renderer(theme, class_list);
				return theme;
			}
			inline static decltype(&hook_proc) orig_proc = ::OpenThemeDataEx;
		} OpenThemeDataEx;

		//
		// このクラスは::SetWindowTheme()をフックします。
		//
		struct {
			inline static HRESULT WINAPI hook_proc(HWND hwnd, LPCWSTR sub_app_name, LPCWSTR sub_id_list)
			{
				MY_TRACE_FUNC("{/hex}, {/}, {/}", hwnd, sub_app_name, sub_id_list);

				return orig_proc(hwnd, sub_app_name, sub_id_list);
			}
			inline static decltype(&hook_proc) orig_proc = ::SetWindowTheme;
		} SetWindowTheme;
	} theme;
}
