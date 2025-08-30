#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはスピンボタンのマテリアルです。
	//
	inline struct SpinMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			auto button_normal = create_pigment(L"Spin", L"Normal",
				Style::Color::ButtonBody,
				Style::Color::Border,
				Style::Color::Text);

			auto button_disabled = create_pigment(L"Spin", L"Disable",
				Style::Color::ButtonBodyDisable,
				Style::Color::Border,
				Style::Color::TextDisable);

			auto button_hot = create_pigment(L"Spin", L"Hot",
				Style::Color::ButtonBodySelect,
				Style::Color::BorderFocus,
				Style::Color::Text);

			auto button_pressed = create_pigment(L"Spin", L"Press",
				Style::Color::ButtonBodyPress,
				Style::Color::BorderSelect,
				Style::Color::Text);

			palette.set(SPNP_UP, UPS_NORMAL, button_normal);
			palette.set(SPNP_UP, UPS_DISABLED, button_disabled);
			palette.set(SPNP_UP, UPS_HOT, button_hot);
			palette.set(SPNP_UP, UPS_PRESSED, button_pressed);
			palette.set(SPNP_DOWN, DNS_NORMAL, button_normal);
			palette.set(SPNP_DOWN, DNS_DISABLED, button_disabled);
			palette.set(SPNP_DOWN, DNS_HOT, button_hot);
			palette.set(SPNP_DOWN, DNS_PRESSED, button_pressed);
			palette.set(SPNP_UPHORZ, UPHZS_NORMAL, button_normal);
			palette.set(SPNP_UPHORZ, UPHZS_DISABLED, button_disabled);
			palette.set(SPNP_UPHORZ, UPHZS_HOT, button_hot);
			palette.set(SPNP_UPHORZ, UPHZS_PRESSED, button_pressed);
			palette.set(SPNP_DOWNHORZ, DNHZS_NORMAL, button_normal);
			palette.set(SPNP_DOWNHORZ, DNHZS_DISABLED, button_disabled);
			palette.set(SPNP_DOWNHORZ, DNHZS_HOT, button_hot);
			palette.set(SPNP_DOWNHORZ, DNHZS_PRESSED, button_pressed);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} spin_material;
}
