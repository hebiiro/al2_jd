#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはシステムカラーのマテリアルです。
	//
	inline struct SysColorMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			palette.set(COLOR_WINDOW, 0, { style.get_COLORREF(Style::Color::Background) });
			palette.set(COLOR_WINDOWTEXT, 0, { style.get_COLORREF(Style::Color::Text) });
			palette.set(COLOR_HIGHLIGHT, 0, { style.get_COLORREF(Style::Color::ButtonBodySelect) });
			palette.set(COLOR_HIGHLIGHTTEXT, 0, { style.get_COLORREF(Style::Color::Text) });
			palette.set(COLOR_BTNFACE, 0, { style.get_COLORREF(Style::Color::ButtonBody) });
			palette.set(COLOR_BTNTEXT, 0, { style.get_COLORREF(Style::Color::Text) });
			palette.set(COLOR_GRAYTEXT, 0, { style.get_COLORREF(Style::Color::TextDisable) });
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} sys_color_material;
}
