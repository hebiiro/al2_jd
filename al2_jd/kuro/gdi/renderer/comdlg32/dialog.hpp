#pragma once

namespace apn::dark::kuro::gdi::comdlg32
{
	struct DialogRenderer : gdi::DialogRenderer
	{
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
