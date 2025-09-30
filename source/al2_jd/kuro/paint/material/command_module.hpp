#pragma once

namespace apn::dark::kuro
{
	enum COMMANDMODULEPARTS {
		CMP_BODY = 1,
		CMP_BUTTON = 3,
		CMP_SPLITBUTTON = 4,
		CMP_SPLITBUTTONDROPDOWN = 5,
		CMP_DROPDOWNBUTTONGLYPH = 6,
	};

	enum COMMANDMODULESTATES {
		CMS_BACKGROUND = 0,
		CMS_NORMAL = 1,
		CMS_HOT = 2,
		CMS_PRESSED = 3,
		CMS_FOCUSED = 4,
		CMS_CHECKED = 5,
	};
}

namespace apn::dark::kuro::paint
{
	//
	// このクラスはコマンドモジュールのマテリアルです。
	//
	inline struct CommandModuleMaterial : Material
	{
		//
		// コマンドモジュールを描画している現在のDCです。
		//
		HDC dc = {};

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

			palette.set(CMP_BODY, CMS_BACKGROUND, background);

			palette.set(CMP_BUTTON, CMS_NORMAL, normal);
			palette.set(CMP_BUTTON, CMS_HOT, hot);
			palette.set(CMP_BUTTON, CMS_PRESSED, pressed);
			palette.set(CMP_BUTTON, CMS_FOCUSED, checked);
			palette.set(CMP_BUTTON, CMS_CHECKED, checked);

			palette.set(CMP_SPLITBUTTON, CMS_BACKGROUND, background);
			palette.set(CMP_SPLITBUTTON, CMS_NORMAL, normal);
			palette.set(CMP_SPLITBUTTON, CMS_HOT, hot);
			palette.set(CMP_SPLITBUTTON, CMS_PRESSED, pressed);
			palette.set(CMP_SPLITBUTTON, CMS_FOCUSED, checked);
			palette.set(CMP_SPLITBUTTON, CMS_CHECKED, checked);

			palette.set(CMP_SPLITBUTTONDROPDOWN, CMS_BACKGROUND, background);
			palette.set(CMP_SPLITBUTTONDROPDOWN, CMS_NORMAL, normal);
			palette.set(CMP_SPLITBUTTONDROPDOWN, CMS_HOT, hot);
			palette.set(CMP_SPLITBUTTONDROPDOWN, CMS_PRESSED, pressed);
			palette.set(CMP_SPLITBUTTONDROPDOWN, CMS_FOCUSED, checked);
			palette.set(CMP_SPLITBUTTONDROPDOWN, CMS_CHECKED, checked);

			palette.set(CMP_DROPDOWNBUTTONGLYPH, CMS_BACKGROUND, background);
			palette.set(CMP_DROPDOWNBUTTONGLYPH, CMS_NORMAL, normal);
			palette.set(CMP_DROPDOWNBUTTONGLYPH, CMS_HOT, hot);
			palette.set(CMP_DROPDOWNBUTTONGLYPH, CMS_PRESSED, pressed);
			palette.set(CMP_DROPDOWNBUTTONGLYPH, CMS_FOCUSED, checked);
			palette.set(CMP_DROPDOWNBUTTONGLYPH, CMS_CHECKED, checked);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} command_module_material;
}
