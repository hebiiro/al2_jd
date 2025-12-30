#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはウィンドウのマテリアルです。
	//
	inline struct window_material_t : material_t
	{
		kuro::paint::pigment_t active, inactive;

		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			active = create_pigment(L"Window", L"Active",
				style_t::color_e::Background,
				style_t::color_e::WindowBorder,
				style_t::color_e::Text);

			inactive = create_pigment(L"Window", L"Inactive",
				style_t::color_e::TitleHeader,
				style_t::color_e::TitleHeader,
				style_t::color_e::TextDisable);
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
