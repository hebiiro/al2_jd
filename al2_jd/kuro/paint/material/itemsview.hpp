#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはItemsViewのマテリアルです。
	//
	inline struct ItemsViewMaterial : Material
	{
		Pigment pigment = {};

		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			pigment = {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} itemsview_material;
}
