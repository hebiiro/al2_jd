#pragma once

namespace apn::dark::kuro
{
	enum EDITTEXTSTATES {
		ETS_FOOTER = 100,
	};
}

namespace apn::dark::kuro::paint
{
	//
	// このクラスはダイアログのマテリアルです。
	//
	inline struct DialogMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			auto normal = create_pigment(L"Dialog", L"Normal",
				Style::Color::Background,
				{},
				Style::Color::Text);

			auto disabled = create_pigment(L"Dialog", L"Disable",
				Style::Color::ButtonBodyDisable,
				{},
				Style::Color::Text);

			auto selected = create_pigment(L"Dialog", L"Select",
				Style::Color::ButtonBodySelect,
				{},
				Style::Color::Text);

			auto readonly = create_pigment(L"Dialog", L"ReadOnly",
				Style::Color::ButtonBody,
				{},
				Style::Color::Text);

			auto footer = create_pigment(L"Dialog", L"Footer",
				Style::Color::Footer,
				{},
				{});

			palette.set(WP_DIALOG, ETS_NORMAL, normal);
			palette.set(WP_DIALOG, ETS_DISABLED, disabled);
			palette.set(WP_DIALOG, ETS_SELECTED, selected);
			palette.set(WP_DIALOG, ETS_HOT, selected);
			palette.set(WP_DIALOG, ETS_FOCUSED, selected);
			palette.set(WP_DIALOG, ETS_READONLY, readonly);
			palette.set(WP_DIALOG, ETS_ASSIST, disabled);
			palette.set(WP_DIALOG, ETS_CUEBANNER, disabled);
			palette.set(WP_DIALOG, ETS_FOOTER, footer);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} dialog_material;
}
