#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはエディットボックスのマテリアルです。
	//
	inline struct EditBoxMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			auto ets_normal = create_pigment(L"EditBox", L"Normal",
				Style::Color::Background,
				CLR_NONE,
				Style::Color::Text);

			auto ets_disabled = create_pigment(L"EditBox", L"Disable",
				Style::Color::ButtonBodyDisable,
				CLR_NONE,
				Style::Color::Text);

			auto ets_selected = create_pigment(L"EditBox", L"Select",
				Style::Color::ButtonBodySelect,
				CLR_NONE,
				Style::Color::Text);

			auto ets_readonly = create_pigment(L"EditBox", L"ReadOnly",
				Style::Color::ButtonBody,
				CLR_NONE,
				Style::Color::Text);

			palette.set(EP_EDITTEXT, ETS_NORMAL, ets_normal);
			palette.set(EP_EDITTEXT, ETS_DISABLED, ets_disabled);
			palette.set(EP_EDITTEXT, ETS_SELECTED, ets_selected);
			palette.set(EP_EDITTEXT, ETS_HOT, ets_selected);
			palette.set(EP_EDITTEXT, ETS_FOCUSED, ets_selected);
			palette.set(EP_EDITTEXT, ETS_READONLY, ets_readonly);
			palette.set(EP_EDITTEXT, ETS_ASSIST, ets_disabled);
			palette.set(EP_EDITTEXT, ETS_CUEBANNER, ets_disabled);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} editbox_material;
}
