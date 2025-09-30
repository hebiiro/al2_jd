#pragma once

namespace apn::dark::kuro::gdi::aviutl2::font_setting
{
	struct ListViewRenderer : gdi::ListViewRenderer
	{
		//
		// CDDS_ITEMPREPAINTでセットするフォントです。
		//
		my::gdi::unique_ptr<HFONT> font;

		//
		// カスタムドローを処理します。
		//
		virtual LRESULT on_custom_draw(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override
		{
			auto nm = (NMLVCUSTOMDRAW*)lParam;

			MY_TRACE_FUNC("stage = {/hex}, text_color = {/hex}, text_bk_color = {/hex}, sub_item = {/}",
				nm->nmcd.dwDrawStage, nm->clrText, nm->clrTextBk, nm->iSubItem);

			switch (nm->nmcd.dwDrawStage)
			{
			case CDDS_PREPAINT:
				{
					return CDRF_NOTIFYITEMDRAW;
				}
			case CDDS_ITEMPREPAINT:
				{
					auto hwnd = nm->nmcd.hdr.hwndFrom;
					auto dc = nm->nmcd.hdc;
					auto item_index = nm->nmcd.dwItemSpec;
					auto sub_item_index = 0;

					WCHAR text[MAX_PATH] = {};
					ListView_GetItemText(hwnd, item_index, sub_item_index, text, std::size(text));
					if (hive.available_fonts.contains(text))
					{
						// フォントを使用して項目名を描画します。

						auto rc = RECT {};
						ListView_GetItemRect(hwnd, item_index, &rc, LVIR_LABEL);

						auto font_name = std::wstring(text);
						auto font_height = my::get_height(rc);

						font.reset(::CreateFontW(
							font_height, 0, 0, 0, 0, FALSE, FALSE, FALSE,
							DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH, font_name.c_str()));

						::SelectObject(dc, font.get());

						return CDRF_NEWFONT;
					}

					break;
				}
			}

			return __super::on_custom_draw(hwnd, message, wParam, lParam);
		}
	};
}
