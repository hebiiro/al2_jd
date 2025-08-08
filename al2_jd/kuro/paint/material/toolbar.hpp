#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはツールバーのマテリアルです。
	//
	inline struct ToolBarMaterial : Material
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
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto normal = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBody), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto disabled = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyDisable), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			auto hot = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyHover), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto pressed = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyPress), },
				{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto checked = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
				{ style.get_COLORREF(Style::Color::BorderFocus), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto button_id = 0;
			auto flat_button_id = TP_BUTTON;

			palette.set(button_id, 0, background);
			palette.set(button_id, TS_NORMAL, normal);
			palette.set(button_id, TS_DISABLED, disabled);
			palette.set(button_id, TS_HOT, hot);
			palette.set(button_id, TS_PRESSED, pressed);
			palette.set(button_id, TS_CHECKED, checked);
			palette.set(button_id, TS_HOTCHECKED, checked);

			auto flat_normal = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto flat_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE, },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			auto flat_hot = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
//				{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto flat_pressed = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyPress), },
				{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto flat_checked = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
				{ style.get_COLORREF(Style::Color::BorderFocus), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			palette.set(flat_button_id, 0, background);
			palette.set(flat_button_id, TS_NORMAL, flat_normal);
			palette.set(flat_button_id, TS_DISABLED, flat_disabled);
			palette.set(flat_button_id, TS_HOT, flat_hot);
			palette.set(flat_button_id, TS_PRESSED, flat_pressed);
			palette.set(flat_button_id, TS_CHECKED, flat_checked);
			palette.set(flat_button_id, TS_HOTCHECKED, flat_checked);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} toolbar_material;
}
