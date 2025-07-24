#pragma once

namespace apn::dark::kuro::theme::base
{
	//
	// このクラスはメニューレンダラーのベースクラスです。
	//
	struct MenuRenderer : Renderer
	{
		virtual HRESULT on_get_theme_color(HTHEME theme, int part_id, int state_id, int prop_id, COLORREF* result) override
		{
			MY_TRACE_FUNC("{/hex}, {/}, {/}, {/}", theme, part_id, state_id, prop_id);

			const auto& menu_palette = paint::menu_material.palette;

			// ポップアップメニューの背景の色を変更します。
			if (part_id == MENU_POPUPBACKGROUND && state_id == 0 && prop_id == TMT_FILLCOLOR)
				return *result = menu_palette.get(part_id, state_id)->background.color, S_OK;

			// ポップアップメニューの縁の色を変更します。
			if (part_id == MENU_POPUPBORDERS && state_id == 0 && prop_id == TMT_FILLCOLORHINT)
				return *result = menu_palette.get(part_id, state_id)->background.color, S_OK;

			// ポップアップメニューのアイコンの色を変更します。
			if (part_id == MENU_POPUPITEMFOCUSABLE && prop_id == TMT_TEXTCOLOR)
				return *result = menu_palette.get(part_id, state_id)->text.color, S_OK;

			return __super::on_get_theme_color(theme, part_id, state_id, prop_id, result);
		}
	};
}
