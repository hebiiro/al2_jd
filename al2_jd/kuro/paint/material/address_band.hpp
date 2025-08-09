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

			auto background = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

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
