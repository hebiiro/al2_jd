#pragma once

namespace apn::dark::kuro::paint
{
	//
	// シンボル描画用のフォント名です。
	//
	inline static constexpr auto c_symbol = L"Segoe UI Symbol";

	//
	// 疑似シンボル描画用のフォント名です。
	//
	inline static constexpr auto c_pseudo = L"Consolas";

	//
	// 疑似シンボル描画用の縦書きフォント名です。
	//
	inline static constexpr auto c_pseudo_vert = L"@Consolas";

	//
	// 縁の幅を整数で返します。
	//
	inline int get_border_width_as_int()
	{
		return hive.border.width / 10;
	}

	//
	// 縁の幅を実数で返します。
	//
	inline float get_border_width_as_float()
	{
		return hive.border.width / 10.0f;
	}

	//
	// 丸みを整数で返します。
	//
	inline int get_round_as_int(int base_round_size)
	{
		return ::MulDiv(base_round_size, hive.round.size, 100);
	}

	//
	// 丸みを実数で返します。
	//
	inline float get_round_as_float(float base_round_size)
	{
		return base_round_size * hive.round.size / 100.0f;
	}

	//
	// グラデーション終了色のアルファを実数で返します。
	//
	inline float get_gradient_end_alpha()
	{
		return hive.gradient.end_alpha / 100.0f;
	}

	//
	// 影のオフセットを整数で返します。
	//
	inline POINT get_shadow_offset_as_int()
	{
		return { hive.shadow.offset.x / 10, hive.shadow.offset.y / 10 };
	}

	//
	// 影のサイズを整数で返します。
	//
	inline int get_shadow_size_as_int()
	{
		return hive.shadow.size / 10;
	}

	//
	// 指定されたペンの色を返します。
	//
	inline COLORREF get_pen_color(HPEN pen)
	{
		LOGPEN lp = {};
		if (::GetObject(pen, sizeof(lp), &lp) != sizeof(lp)) return CLR_NONE;
		return lp.lopnColor;
	}

	//
	// 指定されたブラシの色を返します。
	//
	inline COLORREF get_brush_color(HBRUSH brush)
	{
		LOGBRUSH lb = {};
		if (::GetObject(brush, sizeof(lb), &lb) != sizeof(lb)) return CLR_NONE;
		if (lb.lbStyle != BS_SOLID) return CLR_NONE;
		return lb.lbColor;
	}

	//
	// 指定された矩形とモードで描画領域をクリップします。
	//
	inline void clip_rect(HDC dc, LPCRECT rc, int mode = RGN_COPY)
	{
		my::gdi::unique_ptr<HRGN> rgn(::CreateRectRgnIndirect(rc));

		::ExtSelectClipRgn(dc, rgn.get(), mode);
	}

	//
	// 指定された色で指定された矩形を塗りつぶします。
	//
	inline void fill_rect(HDC dc, LPCRECT rc, COLORREF fill_color)
	{
		auto w = my::get_width(*rc);
		auto h = my::get_height(*rc);

		my::gdi::unique_ptr<HBRUSH> brush(::CreateSolidBrush(fill_color));
		my::gdi::selector brush_selector(dc, brush.get());

		hive.orig.PatBlt(dc, rc->left, rc->top, w, h, PATCOPY);
	}

	//
	// 指定された色と幅で指定された矩形の縁を描画します。
	//
	inline void frame_rect(HDC dc, LPCRECT rc, COLORREF edge_color, int edge_width)
	{
		auto w = my::get_width(*rc);
		auto h = my::get_height(*rc);

		my::gdi::unique_ptr<HBRUSH> brush(::CreateSolidBrush(edge_color));
		my::gdi::selector brush_selector(dc, brush.get());

		hive.orig.PatBlt(dc, rc->left, rc->top, w, edge_width, PATCOPY);
		hive.orig.PatBlt(dc, rc->left, rc->top, edge_width, h, PATCOPY);
		hive.orig.PatBlt(dc, rc->left, rc->bottom - edge_width, w, edge_width, PATCOPY);
		hive.orig.PatBlt(dc, rc->right - edge_width, rc->top, edge_width, h, PATCOPY);
	}

	//
	// 矩形を描画します。
	//
	inline void draw_rect(HDC dc, LPCRECT rc, COLORREF fill_color, COLORREF edge_color, int edge_width)
	{
		auto w = my::get_width(*rc);
		auto h = my::get_height(*rc);

		if (fill_color != CLR_NONE)
		{
			my::gdi::unique_ptr<HBRUSH> brush(::CreateSolidBrush(fill_color));
			my::gdi::selector brush_selector(dc, brush.get());

			hive.orig.PatBlt(dc, rc->left, rc->top, w, h, PATCOPY);
		}

		if (edge_color != CLR_NONE && edge_width > 0)
		{
			my::gdi::unique_ptr<HBRUSH> brush(::CreateSolidBrush(edge_color));
			my::gdi::selector brush_selector(dc, brush.get());

			hive.orig.PatBlt(dc, rc->left, rc->top, w, edge_width, PATCOPY);
			hive.orig.PatBlt(dc, rc->left, rc->bottom - edge_width, w, edge_width, PATCOPY);
			hive.orig.PatBlt(dc, rc->left, rc->top, edge_width, h, PATCOPY);
			hive.orig.PatBlt(dc, rc->right - edge_width, rc->top, edge_width, h, PATCOPY);
		}
	}

	//
	// 軽量エッジを描画します。
	//
	inline void draw_single_edge(HDC dc, LPCRECT rc, COLORREF near_color, COLORREF far_color)
	{
		const auto edge_width = 1;

		auto w = my::get_width(*rc);
		auto h = my::get_height(*rc);

		{
			my::gdi::unique_ptr<HBRUSH> brush(::CreateSolidBrush(near_color));
			my::gdi::selector brush_selector(dc, brush.get());

			hive.orig.PatBlt(dc, rc->left, rc->top, w, edge_width, PATCOPY);
			hive.orig.PatBlt(dc, rc->left, rc->top, edge_width, h, PATCOPY);
		}

		{
			my::gdi::unique_ptr<HBRUSH> brush(::CreateSolidBrush(far_color));
			my::gdi::selector brush_selector(dc, brush.get());

			hive.orig.PatBlt(dc, rc->left, rc->bottom - edge_width, w, edge_width, PATCOPY);
			hive.orig.PatBlt(dc, rc->right - edge_width, rc->top, edge_width, h, PATCOPY);
		}
	}

	//
	// エッジを描画します。
	//
	inline void draw_edge(HDC dc, LPCRECT rc,
		COLORREF outer_near_color, COLORREF outer_far_color,
		COLORREF inner_near_color, COLORREF inner_far_color)
	{
		auto rc_temp = *rc;
		draw_single_edge(dc, &rc_temp, outer_near_color, outer_far_color);
		::InflateRect(&rc_temp, -1, -1);
		draw_single_edge(dc, &rc_temp, inner_near_color, inner_far_color);
	}
}
