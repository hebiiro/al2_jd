#pragma once

namespace apn::dark::kuro::gdi
{
	struct AviUtl2Renderer : Renderer
	{
#if 1
		virtual LRESULT on_subclass_proc(MessageState* current_state) override
		{
			auto hwnd = current_state->hwnd;
			auto message = current_state->message;
			auto wParam = current_state->wParam;
			auto lParam = current_state->lParam;

			MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

			switch (message)
			{
			case WM_CLOSE:
				{
					MY_TRACE_FUNC("WM_CLOSE");

					// コンフィグをファイルに書き込みます。
					app->write_config();

					break;
				}
			}

			return __super::on_subclass_proc(current_state);
		}
#endif
	};
}
