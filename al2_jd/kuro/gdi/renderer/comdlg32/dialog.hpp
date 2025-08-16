#pragma once

namespace apn::dark::kuro::gdi::comdlg32
{
	struct DialogRenderer : gdi::DialogRenderer
	{
		//
		// コモンダイアログが開いたときの処理です。
		//
		virtual BOOL on_attach(HWND hwnd)
		{
			// 現在表示されているコモンダイアログの数を増やします。
			hive.comdlg32_visible_count++;

			return __super::on_attach(hwnd);
		}

		//
		// コモンダイアログが閉じたときの処理です。
		//
		virtual BOOL on_detach(HWND hwnd)
		{
			// スレットの有効性を取得しておきます。
			auto is_valid_thread = hive.is_valid_thread();

			// 現在表示されているコモンダイアログの数を減らします。
			hive.comdlg32_visible_count--;

			// スレットの有効性が変化した場合は
			if (is_valid_thread != hive.is_valid_thread())
			{
				// すべてのウィンドウを再描画します。
				app->redraw();
			}

			return __super::on_detach(hwnd);
		}
#ifdef _DEBUG // テスト用コードです。
		virtual LRESULT on_subclass_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			MY_TRACE_FUNC("{/hex}, {/hex}, {/hex}, {/hex}", hwnd, message, wParam, lParam);

			switch (message)
			{
			case WM_MEASUREITEM:
				{
					MY_TRACE_FUNC("{/hex}, WM_MEASUREITEM, {/hex}, {/hex}", hwnd, wParam, lParam);

					break;
				}
			case WM_DRAWITEM:
				{
					MY_TRACE_FUNC("{/hex}, WM_DRAWITEM, {/hex}, {/hex}", hwnd, wParam, lParam);

					break;
				}
			}

			return __super::on_subclass_proc(hwnd, message, wParam, lParam);
		}
#endif
		virtual BOOL on_fill_rect(MessageState* current_state, HDC dc, LPCRECT rc, HBRUSH brush) override
		{
			MY_TRACE_FUNC("{/hex}, ({/}), {/hex}", dc, safe_string(rc), brush);

			// ブラシがカラーIDの場合は
			if (!HIWORD(brush))
			{
				// カラーIDに対応するブラシを返すようにします。

				auto color_id = LOWORD(brush) - 1;

				if (auto pigment = paint::sys_color_material.palette.get(color_id, 0))
					brush = pigment->background.get_brush();
			}

			return __super::on_fill_rect(current_state, dc, rc, brush);
		}
#ifdef _DEBUG // テスト用コードです。
		virtual BOOL on_ext_text_out_w(MessageState* current_state, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/}, {/hex}, {/}, {/}, {/}, {/hex}, bk_color = {/hex}, text_color = {/hex}", dc, x, y, options, safe_string(rc), text, c, dx, ::GetBkColor(dc), ::GetTextColor(dc));

			return __super::on_ext_text_out_w(current_state, dc, x, y, options, rc, text, c, dx);
		}
#endif
		virtual COLORREF on_get_sys_color(int color_id) override
		{
			MY_TRACE_FUNC("{/}", color_id);

			if (auto pigment = paint::sys_color_material.palette.get(color_id, 0))
				return pigment->background.color;

			return __super::on_get_sys_color(color_id);
		}

		virtual HBRUSH on_get_sys_color_brush(int color_id) override
		{
			MY_TRACE_FUNC("{/}", color_id);

			if (auto pigment = paint::sys_color_material.palette.get(color_id, 0))
				return pigment->background.get_brush();

			return __super::on_get_sys_color_brush(color_id);
		}
	};
}
