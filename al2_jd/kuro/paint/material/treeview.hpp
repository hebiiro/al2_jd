#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはツリービューのマテリアルです。
	//
	inline struct TreeViewMaterial : Material
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

			palette.set(TVP_TREEITEM, 0, separator);
			palette.set(TVP_TREEITEM, TREIS_NORMAL, item_normal);
			palette.set(TVP_TREEITEM, TREIS_DISABLED, item_disabled);
			palette.set(TVP_TREEITEM, TREIS_HOT, item_hot);
			palette.set(TVP_TREEITEM, TREIS_SELECTED, item_selected);
			palette.set(TVP_TREEITEM, TREIS_SELECTEDNOTFOCUS, item_selected);
			palette.set(TVP_TREEITEM, TREIS_HOTSELECTED, item_selected);

			palette.set(TVP_GLYPH, GLPS_CLOSED, item_normal);
			palette.set(TVP_GLYPH, GLPS_OPENED, item_normal);
			palette.set(TVP_HOTGLYPH, GLPS_CLOSED, item_hot);
			palette.set(TVP_HOTGLYPH, GLPS_OPENED, item_hot);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} treeview_material;
}
