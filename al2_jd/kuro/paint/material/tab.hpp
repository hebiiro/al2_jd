#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはタブコントロールのマテリアルです。
	//
	inline struct TabMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			auto background = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE, },
				{ CLR_NONE, },
			};

			auto pane = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ CLR_NONE, },
			};

			auto item_normal = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBody), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto item_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyDisable), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			auto item_hot = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBody), },
				{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto item_selected = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
				{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto item_focused = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBody), },
				{ style.get_COLORREF(Style::Color::BorderFocus), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			palette.set(0, 0, background);
			palette.set(TABP_PANE, 0, pane);
			palette.set(TABP_TOPTABITEM, TIS_NORMAL, item_normal);
			palette.set(TABP_TOPTABITEM, TIS_DISABLED, item_disabled);
			palette.set(TABP_TOPTABITEM, TIS_HOT, item_hot);
			palette.set(TABP_TOPTABITEM, TIS_SELECTED, item_selected);
			palette.set(TABP_TOPTABITEM, TIS_FOCUSED, item_focused);
			palette.set(TABP_TOPTABITEMLEFTEDGE, TIS_NORMAL, item_normal);
			palette.set(TABP_TOPTABITEMLEFTEDGE, TIS_DISABLED, item_disabled);
			palette.set(TABP_TOPTABITEMLEFTEDGE, TIS_HOT, item_hot);
			palette.set(TABP_TOPTABITEMLEFTEDGE, TIS_SELECTED, item_selected);
			palette.set(TABP_TOPTABITEMLEFTEDGE, TIS_FOCUSED, item_focused);
			palette.set(TABP_TOPTABITEMRIGHTEDGE, TIS_NORMAL, item_normal);
			palette.set(TABP_TOPTABITEMRIGHTEDGE, TIS_DISABLED, item_disabled);
			palette.set(TABP_TOPTABITEMRIGHTEDGE, TIS_HOT, item_hot);
			palette.set(TABP_TOPTABITEMRIGHTEDGE, TIS_SELECTED, item_selected);
			palette.set(TABP_TOPTABITEMRIGHTEDGE, TIS_FOCUSED, item_focused);
			palette.set(TABP_TOPTABITEMBOTHEDGE, TIS_NORMAL, item_normal);
			palette.set(TABP_TOPTABITEMBOTHEDGE, TIS_DISABLED, item_disabled);
			palette.set(TABP_TOPTABITEMBOTHEDGE, TIS_HOT, item_hot);
			palette.set(TABP_TOPTABITEMBOTHEDGE, TIS_SELECTED, item_selected);
			palette.set(TABP_TOPTABITEMBOTHEDGE, TIS_FOCUSED, item_focused);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} tab_material;
}
