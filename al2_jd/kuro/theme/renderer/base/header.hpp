#pragma once

namespace apn::dark::kuro::theme::base
{
	//
	// このクラスはヘッダーコントロールレンダラーのベースクラスです。
	//
	struct HeaderRenderer : Renderer
	{
		//
		// ドロップダウンボタンを描画します。
		//
		HRESULT draw_drop_down_button(HDC dc, LPCRECT rc, const paint::Pigment* pigment)
		{
			auto rc2 = *rc;
			auto width = my::get_width(rc2);
			rc2.top = (rc2.top + rc2.bottom - width) / 2;
			rc2.bottom = rc2.top + width;

			return draw_icon(dc, &rc2, pigment, L"メイリオ", 0xE015);
		}

		//
		// ソートボタンを描画します。
		//
		HRESULT draw_sort_button(HDC dc, LPCRECT rc, const paint::Pigment* pigment, LPCWSTR font_name, WCHAR char_code)
		{
			auto dpi = ::GetDpiForSystem();

			auto rc2 = *rc;
			rc2.top -= ::MulDiv(2, dpi, USER_DEFAULT_SCREEN_DPI);
			rc2.bottom += ::MulDiv(6, dpi, USER_DEFAULT_SCREEN_DPI);

			return draw_icon(dc, &rc2, pigment, font_name, char_code);
		}
	};
}
