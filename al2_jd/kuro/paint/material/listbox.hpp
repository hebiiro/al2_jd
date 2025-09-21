#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはリストボックスのマテリアルです。
	//
	inline struct ListBoxMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			auto ets_normal = create_pigment(L"ListBox", L"Normal",
				Style::Color::Background,
				{},
				Style::Color::Text);

			auto ets_disabled = create_pigment(L"ListBox", L"Disable",
				Style::Color::ButtonBodyDisable,
				{},
				Style::Color::Text);

			auto ets_selected = create_pigment(L"ListBox", L"Select",
				Style::Color::ButtonBodySelect,
				{},
				Style::Color::Text);

			auto ets_readonly = create_pigment(L"ListBox", L"ReadOnly",
				Style::Color::ButtonBody,
				{},
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
	} listbox_material;
}
