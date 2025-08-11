#pragma once

namespace apn::dark::kuro::gdi::comdlg32
{
	struct DialogRenderer : gdi::DialogRenderer
	{
		virtual BOOL on_attach(HWND hwnd)
		{
			hive.is_comdlg32_visible = TRUE;
			if (hive.jd.exclude_comdlg32)
				hive.renderer_locked = TRUE;

			return __super::on_attach(hwnd);
		}

		virtual BOOL on_detach(HWND hwnd)
		{
			// 現在レンダラーがロック状態を取得します。
			auto renderer_locked = hive.renderer_locked;

			hive.renderer_locked = FALSE;
			hive.is_comdlg32_visible = FALSE;

			// レンダラーがロックされていた場合は再描画します。
			if (renderer_locked) app->redraw();

			return __super::on_detach(hwnd);
		}

		virtual COLORREF on_get_sys_color(int color_id) override
		{
			if (auto pigment = paint::sys_color_material.palette.get(color_id, 0))
				return pigment->background.color;

			return __super::on_get_sys_color(color_id);
		}

		virtual HBRUSH on_get_sys_color_brush(int color_id) override
		{
			if (auto pigment = paint::sys_color_material.palette.get(color_id, 0))
				return pigment->background.get_brush();

			return __super::on_get_sys_color_brush(color_id);
		}
	};
}
