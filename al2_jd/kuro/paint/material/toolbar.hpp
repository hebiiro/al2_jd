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

			auto background = create_pigment(L"ToolBar", L"Background",
				Style::Color::Background,
				{},
				Style::Color::Text);

			struct {
				Pigment normal = create_pigment(L"ToolBarButton", L"Normal",
					Style::Color::ButtonBody,
					Style::Color::Border,
					Style::Color::Text);

				Pigment disabled = create_pigment(L"ToolBarButton", L"Disable",
					Style::Color::ButtonBodyDisable,
					Style::Color::Border,
					Style::Color::TextDisable);

				Pigment hot = create_pigment(L"ToolBarButton", L"Hot",
					Style::Color::ButtonBodyHover,
					Style::Color::Border,
					Style::Color::Text);

				Pigment pressed = create_pigment(L"ToolBarButton", L"Press",
					Style::Color::ButtonBodyPress,
					Style::Color::BorderSelect,
					Style::Color::Text);

				Pigment checked = create_pigment(L"ToolBarButton", L"Check",
					Style::Color::ButtonBodySelect,
					Style::Color::BorderFocus,
					Style::Color::Text);
			} button;

			struct {
				Pigment normal = create_pigment(L"ToolBarFlatButton", L"Normal",
					Style::Color::Background,
					{},
					Style::Color::Text);

				Pigment disabled = create_pigment(L"ToolBarFlatButton", L"Disable",
					Style::Color::Background,
					{},
					Style::Color::TextDisable);

				Pigment hot = create_pigment(L"ToolBarFlatButton", L"Hot",
					Style::Color::ButtonBodySelect,
					Style::Color::Border,
					Style::Color::Text);

				Pigment pressed = create_pigment(L"ToolBarFlatButton", L"Press",
					Style::Color::ButtonBodyPress,
					Style::Color::BorderSelect,
					Style::Color::Text);

				Pigment checked = create_pigment(L"ToolBarFlatButton", L"Check",
					Style::Color::ButtonBodySelect,
					Style::Color::BorderFocus,
					Style::Color::Text);
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
