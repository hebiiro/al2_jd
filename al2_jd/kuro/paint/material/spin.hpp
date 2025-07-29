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

			auto button_normal = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBody), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto button_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyDisable), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			auto button_hot = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
				{ style.get_COLORREF(Style::Color::BorderFocus), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto button_pressed = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyPress), },
				{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

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
