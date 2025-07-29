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

			auto separator = Pigment {
				{ style.get_COLORREF(Style::Color::Border), },
				{ CLR_NONE, },
				{ CLR_NONE, },
			};

			auto& item_normal = *editbox_material.palette.get(EP_EDITTEXT, ETS_NORMAL);
			auto& item_disabled = *editbox_material.palette.get(EP_EDITTEXT, ETS_DISABLED);
			auto& item_hot = *editbox_material.palette.get(EP_EDITTEXT, ETS_HOT);
			auto& item_selected = *editbox_material.palette.get(EP_EDITTEXT, ETS_SELECTED);

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
