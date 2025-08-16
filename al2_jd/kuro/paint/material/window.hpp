#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはウィンドウのマテリアルです。
	//
	inline struct WindowMaterial : Material
	{
		kuro::paint::Pigment active, inactive;

		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			active = {
				{ style.get_COLORREF(Style::Color::Background), },
				{ style.get_COLORREF(Style::Color::WindowBorder), 1 },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			inactive = {
				{ style.get_COLORREF(Style::Color::TitleHeader), },
				{ style.get_COLORREF(Style::Color::TitleHeader), 1 },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} window_material;
}
