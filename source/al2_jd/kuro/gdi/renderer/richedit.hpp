#pragma once

namespace apn::dark::kuro::gdi
{
	struct richedit_renderer_t : editbox_renderer_t
	{
		virtual LRESULT on_subclass_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			switch (message)
			{
			case WM_PAINT:
				{
					MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), wParam, lParam);

					break;
				}
			case WM_ERASEBKGND:
				{
					MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), wParam, lParam);

					auto dc = (HDC)wParam;
					auto rc = my::get_client_rect(hwnd);

					// エディットボックスとして描画します。
					const auto& palette = paint::editbox_material.palette;

					auto part_id = EP_EDITTEXT;
					auto state_id = ::IsWindowEnabled(hwnd) ? ETS_NORMAL : ETS_DISABLED;

					// リッチエディット専用メッセージで背景色を変更します。
					if (auto pigment = palette.get(part_id, state_id))
					{
						::SendMessage(hwnd, EM_SETBKGNDCOLOR, FALSE, pigment->background.get_win32_color());
					}

					break;
				}
			case WM_PRINT:
				{
					MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), wParam, lParam);

					break;
				}
			case WM_PRINTCLIENT:
				{
					MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), wParam, lParam);

					break;
				}
			default:
				{
					MY_TRACE_FUNC("{/}, {/hex}, {/hex}", my::message_to_string(message), wParam, lParam);

					break;
				}
			}

			return __super::on_subclass_proc(hwnd, message, wParam, lParam);
		}
	};
}
