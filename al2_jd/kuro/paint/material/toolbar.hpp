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

			struct {
				Pigment normal = {
					{ style.get_COLORREF(Style::Color::ButtonBody), },
					{ style.get_COLORREF(Style::Color::Border), 1, },
					{ style.get_COLORREF(Style::Color::Text), },
				};

				Pigment disabled = {
					{ style.get_COLORREF(Style::Color::ButtonBodyDisable), },
					{ style.get_COLORREF(Style::Color::Border), 1, },
					{ style.get_COLORREF(Style::Color::TextDisable), },
				};

				Pigment hot = {
					{ style.get_COLORREF(Style::Color::ButtonBodyHover), },
					{ style.get_COLORREF(Style::Color::Border), 1, },
					{ style.get_COLORREF(Style::Color::Text), },
				};

				Pigment pressed = {
					{ style.get_COLORREF(Style::Color::ButtonBodyPress), },
					{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
					{ style.get_COLORREF(Style::Color::Text), },
				};

				Pigment checked = {
					{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
					{ style.get_COLORREF(Style::Color::BorderFocus), 1, },
					{ style.get_COLORREF(Style::Color::Text), },
				};
			} button;

			struct {
				Pigment normal = {
					{ style.get_COLORREF(Style::Color::Background), },
					{ CLR_NONE, },
					{ style.get_COLORREF(Style::Color::Text), },
				};

				Pigment disabled = {
					{ style.get_COLORREF(Style::Color::Background), },
					{ CLR_NONE, },
					{ style.get_COLORREF(Style::Color::TextDisable), },
				};

				Pigment hot = {
					{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
	//				{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
					{ style.get_COLORREF(Style::Color::Border), 1, },
					{ style.get_COLORREF(Style::Color::Text), },
				};

				Pigment pressed = {
					{ style.get_COLORREF(Style::Color::ButtonBodyPress), },
					{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
					{ style.get_COLORREF(Style::Color::Text), },
				};

				Pigment checked = {
					{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
					{ style.get_COLORREF(Style::Color::BorderFocus), 1, },
					{ style.get_COLORREF(Style::Color::Text), },
				};
			} flat_button;

			const auto set = [&](auto part_id, const auto& button)
			{
				palette.set(part_id, 0, background);
				palette.set(part_id, TS_NORMAL, button.normal);
				palette.set(part_id, TS_DISABLED, button.disabled);
				palette.set(part_id, TS_HOT, button.hot);
				palette.set(part_id, TS_PRESSED, button.pressed);
				palette.set(part_id, TS_CHECKED, button.checked);
				palette.set(part_id, TS_HOTCHECKED, button.checked);
				palette.set(part_id, TS_NEARHOT, button.normal);
				palette.set(part_id, TS_OTHERSIDEHOT, button.normal);
			};

			set(0, button);
			set(TP_BUTTON, flat_button);
			set(TP_DROPDOWNBUTTON, flat_button);
			set(TP_SPLITBUTTON, flat_button);
			set(TP_SPLITBUTTONDROPDOWN, flat_button);
			set(TP_SEPARATOR, flat_button);
			set(TP_SEPARATORVERT, flat_button);
			set(TP_DROPDOWNBUTTONGLYPH, flat_button);
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
