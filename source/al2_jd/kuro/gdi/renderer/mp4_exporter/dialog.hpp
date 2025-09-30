#pragma once

namespace apn::dark::kuro::gdi::mp4_exporter
{
	struct DialogRenderer : gdi::DialogRenderer
	{
		virtual LRESULT on_subclass_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

			switch (message)
			{
			case WM_ERASEBKGND:
				{
					MY_TRACE_FUNC("{/hex}, WM_ERASEBKGND, {/hex}, {/hex}", hwnd, wParam, lParam);

					// ダイアログの背景を描画します。

					auto dc = (HDC)wParam;
					auto rc = my::get_client_rect(hwnd);

					const auto& palette = paint::dialog_material.palette;

					auto part_id = WP_DIALOG;
					auto state_id = ::IsWindowEnabled(hwnd) ? ETS_NORMAL : ETS_DISABLED;

					if (auto pigment = palette.get(part_id, state_id))
						return paint::stylus.draw_rect(dc, &rc, pigment);

					break;
				}
			}

			return __super::on_subclass_proc(hwnd, message, wParam, lParam);
		}
	};
}
