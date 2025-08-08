#pragma once

namespace apn::dark::kuro::gdi
{
	struct RendererNc : Renderer
	{
		virtual LRESULT on_subclass_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			switch (message)
			{
			case WM_NCPAINT:
				{
					MY_TRACE_FUNC("WM_NCPAINT, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

					auto style = my::get_style(hwnd);

					// キャプションやスクロールバーが存在する場合は
					if ((style & WS_CAPTION) == WS_CAPTION ||
						(style & WS_HSCROLL) == WS_HSCROLL ||
						(style & WS_VSCROLL) == WS_VSCROLL)
					{
						// 先にデフォルトの描画処理を実行します。
						__super::on_subclass_proc(hwnd, message, wParam, lParam);
					}

					// 非クライアント領域を描画します。

					auto rc = my::get_window_rect(hwnd);
					auto origin = POINT { rc.left, rc.top };
					::OffsetRect(&rc, -origin.x, -origin.y);

					my::WindowDC dc(hwnd);

					return draw_nc_paint(hwnd, dc, origin, &rc);
				}
			}

			return __super::on_subclass_proc(hwnd, message, wParam, lParam);
		}
	};
}
