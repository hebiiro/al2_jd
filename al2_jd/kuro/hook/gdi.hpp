#pragma once

namespace apn::dark::kuro::hook
{
	//
	// このクラスはGDIのフックを担当します。
	//
	inline struct Gdi
	{
		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			DetourTransactionBegin();
			DetourUpdateThread(::GetCurrentThread());

			auto user32 = (my::addr_t)::GetModuleHandleW(L"user32.dll");
			MY_TRACE_HEX(user32);
#if 0 // テスト用コードです。
			my::addr_t address1 = ::CallWindowProcW((WNDPROC)get_ret_addr, nullptr, 0, 0, 0);
			MY_TRACE_HEX(address1);

			my::addr_t address2 = address1 + ((DWORD*)address1)[-1];
			MY_TRACE_HEX(address2);

			my::addr_t address3 = user32 + 0x17E80;
			MY_TRACE_HEX(address3);

			my::addr_t address4 = user32 + 0xB0010; // address2と同じ。
			MY_TRACE_HEX(address4);

			my::hook::attach(CallWindowProcInternal, address3);
#endif
			my::hook::attach(Rectangle);
			my::hook::attach(FillRect);
//			my::hook::attach(DrawFrame, user32);
//			my::hook::attach(DrawFrameControl);
//			my::hook::attach(FrameRect);
			my::hook::attach(DrawEdge);
			my::hook::attach(DrawFocusRect);
//			my::hook::attach(DrawStateW);
			my::hook::attach(ExtTextOutW);
//			my::hook::attach(PatBlt);
			my::hook::attach(GetSysColor);
			my::hook::attach(GetSysColorBrush);
//			my::hook::attach(InsertMenuW);

			if (DetourTransactionCommit() != NO_ERROR)
			{
				MY_TRACE("APIフックに失敗しました\n");

				return FALSE;
			}

//			hive.orig.CallWindowProcWInternal = CallWindowProcWInternal.orig_proc;
			hive.orig.Rectangle = Rectangle.orig_proc;
			hive.orig.FillRect = FillRect.orig_proc;
			hive.orig.DrawFrame = DrawFrame.orig_proc;
			hive.orig.DrawFrameControl = DrawFrameControl.orig_proc;
			hive.orig.FrameRect = FrameRect.orig_proc;
			hive.orig.DrawEdge = DrawEdge.orig_proc;
			hive.orig.DrawFocusRect = DrawFocusRect.orig_proc;
			hive.orig.DrawStateW = DrawStateW.orig_proc;
			hive.orig.ExtTextOutW = ExtTextOutW.orig_proc;
			hive.orig.PatBlt = PatBlt.orig_proc;
			hive.orig.GetSysColor = GetSysColor.orig_proc;
			hive.orig.GetSysColorBrush = GetSysColorBrush.orig_proc;

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// このクラスは::CallWindowProcInternal()をフックします。
		//
		struct {
			inline static LRESULT WINAPI hook_proc(void* u1, WNDPROC wnd_proc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//			inline static LRESULT WINAPI hook_proc(void* u1, WNDPROC wnd_proc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void* u2, void* u3)
			{
				// この関数はテスト用コードです。

				switch (message)
				{
				case WM_CREATE:
					{
						auto class_name = my::get_class_name(hwnd);

						MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}, {/}, {/}",
							hwnd, class_name, message, wParam, lParam, ::GetCurrentThreadId());

						break;
					}
				}

				return orig_proc(u1, wnd_proc, hwnd, message, wParam, lParam);
//				return orig_proc(u1, wnd_proc, hwnd, message, wParam, lParam, u2, u3);
			}
			inline static decltype(&hook_proc) orig_proc = nullptr;
		} CallWindowProcInternal;

		//
		// このクラスは::Rectangle()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HDC dc, int left, int top, int right, int bottom)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/}",
					ret_addr(&dc), dc, left, top, right, bottom);

				return gdi::Renderer::fire_rectangle(dc, left, top, right, bottom);
			}
			inline static decltype(&hook_proc) orig_proc = ::Rectangle;
		} Rectangle;

		//
		// このクラスは::FillRect()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HDC dc, LPCRECT rc, HBRUSH brush)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, ({/}), {/hex} : {/}",
					ret_addr(&dc), dc, safe_string(rc), brush, ::GetCurrentThreadId());

				if (theme::Renderer::is_gdi_hook_locked())
				{
					MY_TRACE("GDIフックがロックされています\n");

					return orig_proc(dc, rc, brush);
				}

				return gdi::Renderer::fire_fill_rect(dc, rc, brush);
			}
			inline static decltype(&hook_proc) orig_proc = ::FillRect;
		} FillRect;

		//
		// このクラスは::DrawFrame()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HDC dc, LPRECT rc, UINT width, UINT type)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, ({/}), {/}, {/hex}",
					ret_addr(&dc), dc, safe_string(rc), width, type);

				return gdi::Renderer::fire_draw_frame(dc, rc, width, type);
			}
			inline static decltype(&hook_proc) orig_proc = nullptr;
		} DrawFrame;

		//
		// このクラスは::DrawFrameControl()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HDC dc, LPRECT rc, UINT type, UINT state)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, ({/}), {/hex}, {/hex}",
					ret_addr(&dc), dc, safe_string(rc), type, state);

				return gdi::Renderer::fire_draw_frame_control(dc, rc, type, state);
			}
			inline static decltype(&hook_proc) orig_proc = ::DrawFrameControl;
		} DrawFrameControl;

		//
		// このクラスは::FrameRect()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HDC dc, LPCRECT rc, HBRUSH brush)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, ({/}), {/hex}",
					ret_addr(&dc), dc, safe_string(rc), brush);

				return gdi::Renderer::fire_frame_rect(dc, rc, brush);
			}
			inline static decltype(&hook_proc) orig_proc = ::FrameRect;
		} FrameRect;

		//
		// このクラスは::DrawEdge()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HDC dc, LPRECT rc, UINT edge, UINT flags)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, ({/}), {/hex}, {/hex}",
					ret_addr(&dc), dc, safe_string(rc), edge, flags);

				return gdi::Renderer::fire_draw_edge(dc, rc, edge, flags);
			}
			inline static decltype(&hook_proc) orig_proc = ::DrawEdge;
		} DrawEdge;

		//
		// このクラスは::DrawFocusRect()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HDC dc, LPCRECT rc)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, ({/})",
					ret_addr(&dc), dc, safe_string(rc));

				return gdi::Renderer::fire_draw_focus_rect(dc, rc);
			}
			inline static decltype(&hook_proc) orig_proc = ::DrawFocusRect;
		} DrawFocusRect;

		//
		// このクラスは::DrawStateW()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/}, {/}, {/}, {/}, {/hex}",
					ret_addr(&dc), dc, fore, x, y, cx, cy, flags);

				return gdi::Renderer::fire_draw_state_w(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
			}
			inline static decltype(&hook_proc) orig_proc = ::DrawStateW;
		} DrawStateW;

		//
		// このクラスは::ExtTextOutW()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
			{
				if (ExtTextOutLocker::locked)
				{
//					MY_TRACE_FUNC("再帰呼び出し時はフックしません\n");

					return orig_proc(dc, x, y, options, rc, text, c, dx);
				}
				ExtTextOutLocker locker;
#if 0
#define ETO_OPAQUE                   0x0002
#define ETO_CLIPPED                  0x0004
#if(WINVER >= 0x0400)
#define ETO_GLYPH_INDEX              0x0010
#define ETO_RTLREADING               0x0080
#define ETO_NUMERICSLOCAL            0x0400
#define ETO_NUMERICSLATIN            0x0800
#define ETO_IGNORELANGUAGE           0x1000
#endif /* WINVER >= 0x0400 */
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
#define ETO_PDY                      0x2000
#endif // (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
#if (_WIN32_WINNT >= _WIN32_WINNT_LONGHORN)
#define ETO_REVERSE_INDEX_MAP        0x10000
#endif
#endif
				MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/hex}, ({/}), {/}, {/} : {/}",
					ret_addr(&dc), dc, x, y, options, safe_string(rc),
					safe_string(text, c, options), c, ::GetCurrentThreadId());

//				if (0)
				{
					// コマンドモジュールDCがセットされている場合は
					if (dc == paint::command_module_material.dc)
					{
						MY_TRACE("コマンドモジュールを描画します\n");

						// コマンドモジュールのテキストを描画します。
						if (gdi::Renderer::draw_dialog_text(dc, x, y, options, rc, text, c, dx))
							return TRUE;
					}
					else
					{
						// コマンドモジュールDCをリセットします。
						paint::command_module_material.dc = {};
					}
				}

				if (theme::Renderer::is_gdi_hook_locked())
				{
					MY_TRACE("GDIフックがロックされています\n");

					return orig_proc(dc, x, y, options, rc, text, c, dx);
				}

				return gdi::Renderer::fire_ext_text_out_w(dc, x, y, options, rc, text, c, dx);
			}
			inline static decltype(&hook_proc) orig_proc = ::ExtTextOutW;
		} ExtTextOutW;

		//
		// このクラスは::PatBlt()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HDC dc, int x, int y, int w, int h, DWORD rop)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, {/}, {/}, {/}, {/}, {/hex}",
					ret_addr(&dc), dc, x, y, w, h, rop);

				return gdi::Renderer::fire_pat_blt(dc, x, y, w, h, rop);
			}
			inline static decltype(&hook_proc) orig_proc = ::PatBlt;
		} PatBlt;

		//
		// このクラスは::GetSysColor()をフックします。
		//
		struct {
			inline static COLORREF WINAPI hook_proc(int color_id)
			{
				MY_TRACE_FUNC("{/hex}, {/}", ret_addr(&color_id), color_id);

				return gdi::Renderer::fire_get_sys_color(color_id);
			}
			inline static decltype(&hook_proc) orig_proc = ::GetSysColor;
		} GetSysColor;

		//
		// このクラスは::GetSysColorBrush()をフックします。
		//
		struct {
			inline static HBRUSH WINAPI hook_proc(int color_id)
			{
				MY_TRACE_FUNC("{/hex}, {/}", ret_addr(&color_id), color_id);

				return gdi::Renderer::fire_get_sys_color_brush(color_id);
			}
			inline static decltype(&hook_proc) orig_proc = ::GetSysColorBrush;
		} GetSysColorBrush;

		//
		// このクラスは::InsertMenuW()をフックします。
		//
		struct {
			inline static BOOL WINAPI hook_proc(HMENU menu, UINT position, UINT flags, UINT_PTR id, LPCWSTR text)
			{
				MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}, {/hex}, {/hex}",
					ret_addr(&menu), menu, position, flags, id, text);

				return orig_proc(menu, position, flags, id, text);
			}
			inline static decltype(&hook_proc) orig_proc = ::InsertMenuW;
		} InsertMenuW;
	} gdi;
}
