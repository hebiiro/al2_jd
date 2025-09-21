#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはアドレスバンドのマテリアルです。
	//
	inline struct AddressBandMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			auto background = create_pigment(L"AddressBand", L"Background",
				Style::Color::Background,
				Style::Color::Border,
				Style::Color::Text);

			palette.set(0, 0, background);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} address_band_material;
}
