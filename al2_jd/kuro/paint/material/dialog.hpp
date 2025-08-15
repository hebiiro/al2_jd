#pragma once

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

			auto ets_normal = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto ets_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyDisable), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto ets_selected = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto ets_readonly = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBody), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			palette.set(WP_DIALOG, ETS_NORMAL, ets_normal);
			palette.set(WP_DIALOG, ETS_DISABLED, ets_disabled);
			palette.set(WP_DIALOG, ETS_SELECTED, ets_selected);
			palette.set(WP_DIALOG, ETS_HOT, ets_selected);
			palette.set(WP_DIALOG, ETS_FOCUSED, ets_selected);
			palette.set(WP_DIALOG, ETS_READONLY, ets_readonly);
			palette.set(WP_DIALOG, ETS_ASSIST, ets_disabled);
			palette.set(WP_DIALOG, ETS_CUEBANNER, ets_disabled);
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
