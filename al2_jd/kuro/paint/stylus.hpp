#pragma once

namespace apn::dark::kuro::paint
{
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
	using PenAttribute = GdiObjectAttribute<HPEN, [](auto p, const Pigment* pigment)
	{
		if (pigment->border.is_valid())
		{
			p->handle = ::CreatePen(PS_INSIDEFRAME, hive.jd.border_width, pigment->border.color);
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
	using BrushAttribute = GdiObjectAttribute<HBRUSH, [](auto p, const Pigment* pigment)
	{
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
		// 以前の背景の色です。
		//
		COLORREF old_bk_color = {};

		//
		// 以前のテキストの色です。
		//
		COLORREF old_text_color = {};

		//
		// コンストラクタです。
		//
		TextAttribute(HDC dc, const Pigment* pigment, BOOL opaque = TRUE)
			: dc(dc)
			, old_bk_mode(::GetBkMode(dc))
			, old_bk_color(::GetBkColor(dc))
			, old_text_color(::GetTextColor(dc))
		{
			if (pigment->background.is_valid() && opaque)
				::SetBkColor(dc, pigment->background.color);
			else
				::SetBkMode(dc, TRANSPARENT);

			if (pigment->text.is_valid())
				::SetTextColor(dc, pigment->text.color);
		}

		//
		// デストラクタです。
		//
		~TextAttribute()
		{
			::SetTextColor(dc, old_text_color);
			::SetBkColor(dc, old_bk_color);
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
		// ピグメントを使用して丸角矩形を描画します。
		//
		BOOL draw_round_rect(HDC dc, LPCRECT rc, const Pigment* pigment)
		{
			if (!hive.jd.as_round) return draw_rect(dc, rc, pigment);

			PenAttribute pen_attribute(dc, pigment);
			BrushAttribute brush_attribute(dc, pigment);

			auto w = my::get_width(*rc);
			auto h = my::get_height(*rc);

			auto round = ::MulDiv(std::min(w, h), hive.jd.round, 100);

			return ::RoundRect(dc, rc->left, rc->top, rc->right - 1, rc->bottom - 1, round, round);
		}

		//
		// ピグメントを使用して文字列を描画します。
		//
		BOOL ext_text_out(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx, const Pigment* pigment, BOOL opaque = TRUE)
		{
			ExtTextOutLocker locker;
			TextAttribute text_attribute(dc, pigment, opaque);

			return hive.orig.ExtTextOutW(dc, x, y, options, rc, text, c, dx);
		}

		//
		// ピグメントを使用して文字列を描画します。
		//
		BOOL draw_text(HDC dc, LPCRECT rc, LPCWSTR text, int c, DWORD text_flags, const Pigment* pigment, BOOL opaque = TRUE)
		{
			ExtTextOutLocker locker;
			TextAttribute text_attribute(dc, pigment, opaque);

			return !!hive.orig.DrawTextW(dc, text, c, (LPRECT)rc, text_flags);
		}

		//
		// ピグメントを使用して絵文字を描画します。
		//
		BOOL draw_icon(HDC dc, LPCRECT rc, const Pigment* pigment, LPCWSTR font_name, WCHAR char_code, int font_weight = 0)
		{
			ExtTextOutLocker locker;
			TextAttribute text_attribute(dc, pigment, FALSE); // 背景は塗りつぶしません。

			auto font_height = my::get_height(*rc);
			my::gdi::unique_ptr<HFONT> font(::CreateFontW(
				font_height, 0, 0, 0, 0,
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, font_name));
			my::gdi::selector font_selector(dc, font.get());

			return !!hive.orig.DrawTextW(dc, &char_code, 1,
				(LPRECT)rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

		//
		// ピグメントを使用して文字列を描画します。
		//
		BOOL d2d_draw_text(HDC dc, LPCRECT rc, LPCWSTR text, int c, DWORD text_flags, const Pigment* pigment, BOOL opaque = TRUE)
		{
			if (!hive.jd.use_d2d) return draw_text(dc, rc, text, c, text_flags, pigment, opaque);

			ExtTextOutLocker locker;
			TextAttribute text_attribute(dc, pigment, opaque);

			return !!d2d.draw_text(dc, text, c, (LPRECT)rc, text_flags);
		}

		//
		// ピグメントを使用して絵文字を描画します。
		//
		BOOL d2d_draw_icon(HDC dc, LPCRECT rc, const Pigment* pigment, LPCWSTR font_name, WCHAR char_code, int font_weight = 0)
		{
			if (!hive.jd.use_d2d) return draw_icon(dc, rc, pigment, font_name, char_code, font_weight);

			ExtTextOutLocker locker;
			TextAttribute text_attribute(dc, pigment, FALSE); // 背景は塗りつぶしません。

			auto font_height = my::get_height(*rc);
			my::gdi::unique_ptr<HFONT> font(::CreateFontW(
				font_height, 0, 0, 0, 0,
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, font_name));
			my::gdi::selector font_selector(dc, font.get());

			return !!d2d.draw_text(dc, &char_code, 1,
				(LPRECT)rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

		//
		// パレットを使用して矩形を描画します。
		//
		inline BOOL draw_rect(HDC dc, LPCRECT rc,
			const paint::Palette& palette, int part_id, int state_id)
		{
			if (auto pigment = palette.get(part_id, state_id))
				return draw_rect(dc, rc, pigment);

			return FALSE;
		}

		//
		// パレットを使用して丸角矩形を描画します。
		//
		inline BOOL draw_round_rect(HDC dc, LPCRECT rc,
			const paint::Palette& palette, int part_id, int state_id)
		{
			if (auto pigment = palette.get(part_id, state_id))
				return draw_round_rect(dc, rc, pigment);

			return FALSE;
		}

		//
		// パレットを使用して文字列を描画します。
		//
		inline BOOL ext_text_out(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx,
			const paint::Palette& palette, int part_id, int state_id, BOOL opaque = TRUE)
		{
			if (auto pigment = palette.get(part_id, state_id))
				return ext_text_out(dc, x, y, options, rc, text, c, dx, pigment, opaque);

			return FALSE;
		}

		//
		// パレットを使用して文字列を描画します。
		//
		inline BOOL draw_text(HDC dc, LPCRECT rc, LPCWSTR text, int c, DWORD text_flags,
			const paint::Palette& palette, int part_id, int state_id, BOOL opaque = TRUE)
		{
			if (auto pigment = palette.get(part_id, state_id))
				return draw_text(dc, rc, text, c, text_flags, pigment, opaque);

			return FALSE;
		}

		//
		// パレットを使用して絵文字を描画します。
		//
		inline BOOL draw_icon(HDC dc, LPCRECT rc,
			const paint::Palette& palette, int part_id, int state_id,
			LPCWSTR font_name, WCHAR char_code, int font_weight = 0)
		{
			if (auto pigment = palette.get(part_id, state_id))
				return draw_icon(dc, rc, pigment, font_name, char_code, font_weight);

			return FALSE;
		}

		//
		// パレットを使用して文字列を描画します。
		//
		inline BOOL d2d_draw_text(HDC dc, LPCRECT rc, LPCWSTR text, int c, DWORD text_flags,
			const paint::Palette& palette, int part_id, int state_id, BOOL opaque = TRUE)
		{
			if (auto pigment = palette.get(part_id, state_id))
				return d2d_draw_text(dc, rc, text, c, text_flags, pigment, opaque);

			return FALSE;
		}

		//
		// パレットを使用して絵文字を描画します。
		//
		inline BOOL d2d_draw_icon(HDC dc, LPCRECT rc,
			const paint::Palette& palette, int part_id, int state_id,
			LPCWSTR font_name, WCHAR char_code, int font_weight = 0)
		{
			if (auto pigment = palette.get(part_id, state_id))
				return d2d_draw_icon(dc, rc, pigment, font_name, char_code, font_weight);

			return FALSE;
		}
	} stylus;
}
