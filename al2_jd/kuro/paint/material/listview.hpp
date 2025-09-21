#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはリストビューのマテリアルです。
	//
	inline struct ListViewMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			auto separator = create_pigment(L"ListView", L"Separator",
				Style::Color::Border,
				{},
				{});

			auto item_normal = create_pigment(L"ListView", L"Normal",
				Style::Color::Background,
				{},
				Style::Color::Text);

			auto item_disabled = create_pigment(L"ListView", L"Disable",
				Style::Color::Background,
				{},
				Style::Color::TextDisable);

			auto item_hot = create_pigment(L"ListView", L"Hot",
				Style::Color::ButtonBodyHover,
				{},
				Style::Color::Text);

			auto item_selected = create_pigment(L"ListView", L"Select",
				Style::Color::ButtonBodySelect,
				{},
				Style::Color::Text);

			palette.set(LVP_LISTITEM, 0, separator);
			palette.set(LVP_LISTITEM, LISS_NORMAL, item_normal);
			palette.set(LVP_LISTITEM, LISS_DISABLED, item_disabled);
			palette.set(LVP_LISTITEM, LISS_HOT, item_hot);
			palette.set(LVP_LISTITEM, LISS_SELECTED, item_selected);
			palette.set(LVP_LISTITEM, LISS_SELECTEDNOTFOCUS, item_selected);
			palette.set(LVP_LISTITEM, LISS_HOTSELECTED, item_selected);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} listview_material;
}
