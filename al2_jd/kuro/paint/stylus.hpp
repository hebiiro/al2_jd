#pragma once

namespace apn::dark::kuro::paint
{
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
	// このクラスはGDIオブジェクトの属性をDCにセットします。
	//
	template <typename T, auto init>
	struct GdiObjectAttribute
	{
		//
		// DCのハンドルです。
		//
		HDC dc = {};

		//
		// GDIオブジェクトのハンドルです。
		//
		T handle = {};

		//
		// 以前のGDIオブジェクトのハンドルです。
		//
		T old_handle = {};

		//
		// 削除可能かどうかのフラグです。
		//
		BOOL is_deletable = {};

		//
		// コンストラクタです。
		//
		GdiObjectAttribute(HDC dc, const Pigment* pigment)
			: dc(dc)
		{
			init(this, pigment);

			old_handle = (T)::SelectObject(dc, handle);
		}

		//
		// デストラクタです。
		//
		~GdiObjectAttribute()
		{
			::SelectObject(dc, old_handle);

			if (is_deletable)
				::DeleteObject(handle);
		}

		//
		// GDIオブジェクトのハンドルを返します。
		//
		operator T() const { return handle; }
	};

	//
	// このクラスはペンの属性をDCにセットします。
	//
	using PenAttribute = GdiObjectAttribute<HPEN,
	[](auto p, const Pigment* pigment) {
		if (pigment->border.is_valid())
		{
			p->handle = ::CreatePen(PS_SOLID, pigment->border.width, pigment->border.color);
			p->is_deletable = TRUE;
		}
		else
		{
			p->handle = (HPEN)::GetStockObject(NULL_PEN);
			p->is_deletable = FALSE;
		}
	}>;

	//
	// このクラスはブラシの属性をDCにセットします。
	//
	using BrushAttribute = GdiObjectAttribute<HBRUSH,
	[](auto p, const Pigment* pigment) {
		if (pigment->background.is_valid())
		{
			p->handle = ::CreateSolidBrush(pigment->background.color);
			p->is_deletable = TRUE;
		}
		else
		{
			p->handle = (HBRUSH)::GetStockObject(NULL_BRUSH);
			p->is_deletable = FALSE;
		}
	}>;

	//
	// このクラスはテキストの属性をDCにセットします。
	//
	struct TextAttribute
	{
		//
		// DCのハンドルです。
		//
		HDC dc = {};

		//
		// 以前の背景モードです。
		//
		int old_bk_mode = {};

		//
		// 以前のテキストの色です。
		//
		COLORREF old_text_color = {};

		//
		// コンストラクタです。
		//
		TextAttribute(HDC dc, const Pigment* pigment)
			: dc(dc)
		{
			old_bk_mode = ::SetBkMode(dc, TRANSPARENT);
			old_text_color = ::SetTextColor(dc, pigment->text.color);
		}

		//
		// デストラクタです。
		//
		~TextAttribute()
		{
			::SetTextColor(dc, old_text_color);
			::SetBkMode(dc, old_bk_mode);
		}
	};

	//
	// このクラスはスタイラスです。
	// 主にピグメントを使用して各種図形などを描画します。
	//
	inline struct Stylus
	{
		//
		// ピグメントを使用して矩形を描画します。
		//
		BOOL draw_rect(HDC dc, LPCRECT rc, const Pigment* pigment)
		{
			if (pigment->border.is_valid())
			{
				PenAttribute pen_attribute(dc, pigment);
				BrushAttribute brush_attribute(dc, pigment);

				return hive.orig.Rectangle(dc, rc->left, rc->top, rc->right, rc->bottom);
			}
			else
			{
				my::gdi::unique_ptr<HBRUSH> brush(
					::CreateSolidBrush(pigment->background.color));

				return hive.orig.FillRect(dc, rc, brush.get());
			}
		}

		//
		// ピグメントを使用して文字列を描画します。
		//
		BOOL ext_text_out(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx, const Pigment* pigment)
		{
			ExtTextOutLocker locker;
			TextAttribute text_attribute(dc, pigment);

			return hive.orig.ExtTextOutW(dc, x, y, options, rc, text, c, dx);
		}

		//
		// ピグメントを使用して文字列を描画します。
		//
		BOOL draw_text(HDC dc, LPCRECT rc, LPCWSTR text, int c, DWORD text_flags, const Pigment* pigment)
		{
			ExtTextOutLocker locker;
			TextAttribute text_attribute(dc, pigment);

			return !!::DrawTextW(dc, text, c, (LPRECT)rc, text_flags);
		}

		//
		// ピグメントを使用して絵文字を描画します。
		//
		BOOL draw_icon(HDC dc, LPCRECT rc, const Pigment* pigment, LPCWSTR font_name, WCHAR char_code)
		{
			ExtTextOutLocker locker;
			TextAttribute text_attribute(dc, pigment);

			auto font_height = my::get_height(*rc);
			my::gdi::unique_ptr<HFONT> font(::CreateFontW(font_height, 0, 0, 0, 0,
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, font_name));
			my::gdi::selector font_selector(dc, font.get());

			return !!::DrawTextW(dc, &char_code, 1,
				(LPRECT)rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	} stylus;
}
