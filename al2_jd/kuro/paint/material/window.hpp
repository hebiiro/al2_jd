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

			active = create_pigment(L"Window", L"Active",
				Style::Color::Background,
				Style::Color::WindowBorder,
				Style::Color::Text);

			inactive = create_pigment(L"Window", L"Inactive",
				Style::Color::TitleHeader,
				Style::Color::TitleHeader,
				Style::Color::TextDisable);
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
