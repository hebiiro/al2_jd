#pragma once

namespace apn::dark::kuro::gdi::rigaya
{
	struct dialog_renderer_t : gdi::dialog_renderer_t
	{
		virtual LRESULT on_subclass_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

			switch (message)
			{
			case WM_SHOWWINDOW:
				{
					MY_TRACE_FUNC("{/hex}, WM_SHOWWINDOW, {/hex}, {/hex}", hwnd, wParam, lParam);

					if (wParam) ::SetActiveWindow(hwnd);

					break;
				}
			}

			return __super::on_subclass_proc(hwnd, message, wParam, lParam);
		}
	};
}
