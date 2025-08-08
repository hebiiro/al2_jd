#pragma once

namespace apn::dark::kuro::gdi
{
	struct AviUtl2Renderer : Renderer
	{
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

		virtual BOOL on_fill_rect(MessageState* current_state, HDC dc, LPCRECT rc, HBRUSH brush) override
		{
			MY_TRACE_FUNC("{/hex}, ({/}), {/hex}", dc, safe_string(rc), brush);

//			if (0) // コモンダイアログの背景をダークモード化するために必要です。
			{
				if (brush == hive.orig.GetSysColorBrush(COLOR_WINDOW) ||
					paint::get_brush_color(brush) == hive.orig.GetSysColor(COLOR_WINDOW))
				{
					MY_TRACE("コモンダイアログの背景を描画します\n");

					// ダイアログの背景を描画します。
					if (draw_dialog_background(dc, rc))
						return TRUE;
				}
			}

			return hive.orig.FillRect(dc, rc, brush);
		}
	};
}
