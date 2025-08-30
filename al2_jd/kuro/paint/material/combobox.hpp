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

			auto border_normal = create_pigment(L"ComboBox", L"Normal",
				Style::Color::ButtonBody,
				Style::Color::Border,
				Style::Color::Text);

			auto border_hot = create_pigment(L"ComboBox", L"Hot",
				Style::Color::ButtonBodyHover,
				Style::Color::Border,
				Style::Color::Text);

			auto border_pressed = create_pigment(L"ComboBox", L"Press",
				Style::Color::ButtonBodyPress,
				Style::Color::BorderSelect,
				Style::Color::Text);

			auto border_disabled = create_pigment(L"ComboBox", L"Disable",
				Style::Color::ButtonBodyDisable,
				Style::Color::Border,
				Style::Color::TextDisable);

			auto readonly_normal = create_pigment(L"ComboBoxReadOnly", L"Normal", border_normal);
			auto readonly_disabled = create_pigment(L"ComboBoxReadOnly", L"Disable", border_disabled);
			auto readonly_hot = create_pigment(L"ComboBoxReadOnly", L"Hot", border_hot);
			auto readonly_pressed = create_pigment(L"ComboBoxReadOnly", L"Press", border_pressed);

			auto drop_down_normal = create_pigment(L"ComboBoxDropDown", L"Normal", border_normal);
			auto drop_down_disabled = create_pigment(L"ComboBoxDropDown", L"Disable", border_disabled);
			auto drop_down_hot = create_pigment(L"ComboBoxDropDown", L"Hot", border_hot);
			auto drop_down_pressed = create_pigment(L"ComboBoxDropDown", L"Press", border_pressed);

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
