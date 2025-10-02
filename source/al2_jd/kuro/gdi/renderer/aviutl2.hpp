#pragma once

namespace apn::dark::kuro::gdi
{
	struct AviUtl2Renderer : Renderer
	{
		//
		// スリムバーです。
		//
		std::shared_ptr<SlimBar> slimbar = std::make_shared<SlimBar>();

		//
		// ウィンドウにアタッチしたときの処理です。
		//
		virtual BOOL on_attach(HWND hwnd)
		{
			slimbar->attach(hwnd);

			return __super::on_attach(hwnd);
		}

		//
		// ウィンドウからデタッチしたときの処理です。
		//
		virtual BOOL on_detach(HWND hwnd)
		{
			slimbar->detach(hwnd);

			return __super::on_detach(hwnd);
		}

		virtual LRESULT on_subclass_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/hex}, {/hex}", hwnd, my::message_to_string(message), wParam, lParam);

			switch (message)
			{
			case WM_MOUSEACTIVATE:
				{
					MY_TRACE_FUNC("WM_MOUSEACTIVATE, hwnd = {/hex}, ht = {/hex}, mouse_message = {/hex}", wParam, LOWORD(lParam), HIWORD(lParam));

					// デフォルト処理を実行します。
					auto result = __super::on_subclass_proc(hwnd, message, wParam, lParam);

					// 通常のマウスアクティブ化処理を実行する場合は
					if (hive.etc.default_mouse_activate)
						return MA_ACTIVATE; // マウスメッセージを破棄しないようにします。

					// デフォルトの結果を返します。(マウスメッセージを破棄します)
					return result;
				}
			case WM_CLOSE:
				{
					MY_TRACE_FUNC("WM_CLOSE");

					// コンフィグをファイルに書き込みます。
					app->write_config();

					break;
				}
			}

			if (message == hive.c_message.c_post_init)
			{
				// 初期化後処理を実行します。
				app->post_init();
			}

			return __super::on_subclass_proc(hwnd, message, wParam, lParam);
		}

		virtual BOOL on_fill_rect(MessageState* current_state, HDC dc, LPCRECT rc, HBRUSH brush) override
		{
			MY_TRACE_FUNC("{/hex}, ({/}), {/hex}", dc, safe_string(rc), brush);

			// コモンダイアログが表示されている場合は
			if (hive.comdlg32_visible_count)
			{
				if (brush == hive.orig.GetSysColorBrush(COLOR_WINDOW) ||
					paint::get_brush_color(brush) == hive.orig.GetSysColor(COLOR_WINDOW))
				{
					MY_TRACE("コモンダイアログの背景を描画します\n");
#if 0
					// ボタンの背景を使用してダイアログの背景を描画します。
					if (draw_button_background(dc, rc))
						return TRUE;
#else
					// ダイアログの背景を描画します。
					if (draw_dialog_background(dc, rc))
						return TRUE;
#endif
				}
			}

			return hive.orig.FillRect(dc, rc, brush);
		}

		virtual BOOL on_ext_text_out_w(MessageState* current_state, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/}, options = {/hex}, {/}, {/}, {/}, {/hex}, bk_color = {/hex}, text_color = {/hex}",
				dc, x, y, options, safe_string(rc), safe_string(text, c, options), c, dx, ::GetBkColor(dc), ::GetTextColor(dc));

			// コモンダイアログが表示されている場合は
			if (hive.comdlg32_visible_count)
			{
				// 背景色を取得します。
				auto bk_color = ::GetBkColor(dc);

				// エディットボックスとして描画します。
				const auto& palette = paint::editbox_material.palette;

				auto part_id = EP_EDITTEXT;
				auto state_id = ETS_NORMAL;

				// 背景色がウィンドウの色ではない場合は
				if (bk_color != hive.orig.GetSysColor(COLOR_WINDOW))
					state_id = ETS_SELECTED; // 選択状態として描画します。

				if (auto pigment = palette.get(part_id, state_id))
					return paint::stylus.ext_text_out(dc, x, y, options, rc, text, c, dx, pigment);
			}

			return hive.orig.ExtTextOutW(dc, x, y, options, rc, text, c, dx);
		}
	};
}
