#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはコンボボックスのマテリアルです。
	//
	inline struct ComboBoxMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			auto border_normal = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBody), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto border_hot = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyHover), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto border_pressed = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyPress), },
				{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto border_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyDisable), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			auto readonly_normal = border_normal;
			auto readonly_disabled = border_disabled;
			auto readonly_hot = border_hot;
			auto readonly_pressed = border_pressed;

			auto drop_down_normal = border_normal;
			auto drop_down_disabled = border_disabled;
			auto drop_down_hot = border_hot;
			auto drop_down_pressed = border_pressed;

			palette.set(CP_BORDER, CBXS_NORMAL, border_normal);
			palette.set(CP_BORDER, CBXS_DISABLED, border_disabled);
			palette.set(CP_BORDER, CBXS_HOT, border_hot);
			palette.set(CP_BORDER, CBXS_PRESSED, border_pressed);
			palette.set(CP_READONLY, CBXS_NORMAL, readonly_normal);
			palette.set(CP_READONLY, CBXS_DISABLED, readonly_disabled);
			palette.set(CP_READONLY, CBXS_HOT, readonly_hot);
			palette.set(CP_READONLY, CBXS_PRESSED, readonly_pressed);
			palette.set(CP_DROPDOWNBUTTONRIGHT, CBXSR_NORMAL, drop_down_normal);
			palette.set(CP_DROPDOWNBUTTONRIGHT, CBXSR_DISABLED, drop_down_disabled);
			palette.set(CP_DROPDOWNBUTTONRIGHT, CBXSR_HOT, drop_down_hot);
			palette.set(CP_DROPDOWNBUTTONRIGHT, CBXSR_PRESSED, drop_down_pressed);
			palette.set(CP_DROPDOWNBUTTONLEFT, CBXSR_NORMAL, drop_down_normal);
			palette.set(CP_DROPDOWNBUTTONLEFT, CBXSR_DISABLED, drop_down_disabled);
			palette.set(CP_DROPDOWNBUTTONLEFT, CBXSR_HOT, drop_down_hot);
			palette.set(CP_DROPDOWNBUTTONLEFT, CBXSR_PRESSED, drop_down_pressed);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} combobox_material;
}
