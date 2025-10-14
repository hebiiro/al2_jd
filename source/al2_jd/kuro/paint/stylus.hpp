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
		if (pigment->border.is_valid() && pigment->border.is_opaque())
		{
			p->handle = ::CreatePen(PS_INSIDEFRAME, get_border_width_as_int(), pigment->border.get_win32_color());
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
		if (pigment->background.is_valid() && pigment->background.is_opaque())
		{
			p->handle = ::CreateSolidBrush(pigment->background.get_win32_color());
			p->is_deletable = TRUE;
		}
		else
		{
			p->handle = (HBRUSH)::GetStockObject(NULL_BRUSH);
			p->is_deletable = FALSE;
		}
	}>;

	//
	// このクラスはテキスト属性のベースクラスです。
	//
	struct TextAttributeBase
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
		TextAttributeBase(HDC dc, COLORREF background_color, COLORREF text_color, BOOL opaque)
			: dc(dc)
			, old_bk_mode(::GetBkMode(dc))
			, old_bk_color(::GetBkColor(dc))
			, old_text_color(::GetTextColor(dc))
		{
			if (background_color != CLR_NONE && opaque)
				::SetBkColor(dc, background_color);
			else
				::SetBkMode(dc, TRANSPARENT);

			if (text_color != CLR_NONE)
				::SetTextColor(dc, text_color);
		}

		//
		// デストラクタです。
		//
		~TextAttributeBase()
		{
			::SetTextColor(dc, old_text_color);
			::SetBkColor(dc, old_bk_color);
			::SetBkMode(dc, old_bk_mode);
		}
	};

	//
	// このクラスはテキストの属性をDCにセットします。
	//
	struct TextAttribute : TextAttributeBase
	{
		TextAttribute(HDC dc, const Pigment* pigment, BOOL opaque = TRUE) : TextAttributeBase
			(dc, pigment->background.get_win32_color(), pigment->text.get_win32_color(), opaque) {}
	};

	//
	// このクラスは影の属性をDCにセットします。
	//
	struct TextShadowAttribute : TextAttributeBase
	{
		TextShadowAttribute(HDC dc, const Pigment* pigment, BOOL opaque = TRUE) : TextAttributeBase
			(dc, pigment->background.get_win32_color(), pigment->text_shadow.get_win32_color(), opaque) {}
	};

	//
	// このクラスはアイコンの属性をDCにセットします。
	//
	struct IconAttribute
	{
		my::gdi::unique_ptr<HFONT> font;
		my::gdi::selector font_selector;

		IconAttribute(HDC dc, LPCRECT rc, LPCWSTR font_name, int font_weight)
			: font(::CreateFontW(
				my::get_height(*rc), 0, 0, 0, font_weight,
				FALSE, FALSE, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, font_name))
			, font_selector(dc, font.get())
		{
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
					::CreateSolidBrush(pigment->background.get_win32_color()));

				return hive.orig.FillRect(dc, rc, brush.get());
			}
		}

		//
		// ピグメントを使用して丸角矩形を描画します。
		//
		BOOL draw_round_rect(HDC dc, LPCRECT arg_rc, const Pigment* pigment)
		{
			if (!hive.round.flag_use) return draw_rect(dc, arg_rc, pigment);

			auto rc = *arg_rc;
#if 0
			rc.right -= 1;
			rc.bottom -= 1;
#endif
			auto w = my::get_width(rc);
			auto h = my::get_height(rc);
			auto r = std::min(w, h);

			if (hive.jd.use_d2d)
			{
				auto radius = get_round_as_float(r / 2.0f);

				if (auto result = d2d::Recter(dc, &rc, pigment).draw_round_rect(radius))
					return result;
			}

			auto round = get_round_as_int(r);

			PenAttribute pen_attribute(dc, pigment);
			BrushAttribute brush_attribute(dc, pigment);

			return ::RoundRect(dc, rc.left, rc.top, rc.right, rc.bottom, round, round);
		}

		//
		// ピグメントを使用して文字列を描画します。
		//
		BOOL ext_text_out(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx, const Pigment* pigment, BOOL opaque = TRUE)
		{
			// このスコープ内では::ExtTextOutW()をフックしないようにします。
			Locker locker(&ext_text_out_lock);

			// 影を描画する場合は
			if (hive.shadow.flag_use && pigment->text_shadow.is_valid())
			{
				auto offset = get_shadow_offset_as_int();

				auto fill_color = pigment->background.get_win32_color();
				auto text_color = pigment->text.get_win32_color();
				auto text_shadow_color = pigment->text_shadow.get_win32_color();

				auto old_bk_mode = ::GetBkMode(dc);
				auto old_bk_color = ::GetBkColor(dc);
				auto old_text_color = ::GetTextColor(dc);

				if (options & ETO_OPAQUE)
				{
					// 背景を塗りつぶします。

					if (fill_color != CLR_NONE)
						::SetBkColor(dc, fill_color);

					hive.orig.ExtTextOutW(dc, x, y, options, rc, nullptr, 0, dx);
				}

				if (text && c)
				{
					// 文字列を描画します。

					::SetBkMode(dc, TRANSPARENT);

					if (text_shadow_color != CLR_NONE)
					{
						auto align = ::GetTextAlign(dc);
						::SetTextAlign(dc, align & ~TA_UPDATECP);
						::SetTextColor(dc, text_shadow_color);

						x += offset.x, y += offset.y;
						hive.orig.ExtTextOutW(dc, x, y, options & ~ETO_OPAQUE, rc, text, c, dx);
						x -= offset.x, y -= offset.y;

						::SetTextAlign(dc, align);
					}

					::SetTextColor(dc, text_color);

					hive.orig.ExtTextOutW(dc, x, y, options & ~ETO_OPAQUE, rc, text, c, dx);
				}

				::SetBkMode(dc, old_bk_mode);
				::SetBkColor(dc, old_bk_color);
				::SetTextColor(dc, old_text_color);

				return TRUE;
			}
			// 影を描画しない場合は
			else
			{
				// テキストを描画します。
				{
					TextAttribute text_attribute(dc, pigment, opaque);

					return hive.orig.ExtTextOutW(dc, x, y, options, rc, text, c, dx);
				}
			}
		}

		//
		// ピグメントを使用して文字列を描画します。
		//
		BOOL draw_text(HDC dc, LPCRECT rc, LPCWSTR text, int c, DWORD text_flags, const Pigment* pigment, BOOL opaque = TRUE)
		{
			// このスコープ内では::ExtTextOutW()をフックしないようにします。
			Locker locker(&ext_text_out_lock);
#if 0 // テスト用コードです。
			my::theme::unique_ptr<> theme(::OpenThemeData(nullptr, L"Window"));

			DTTOPTS opts = { sizeof(DTTOPTS) };
			opts.dwFlags = DTT_TEXTCOLOR | DTT_SHADOWCOLOR | DTT_SHADOWTYPE | DTT_SHADOWOFFSET;
//			opts.dwFlags = DTT_TEXTCOLOR | DTT_SHADOWCOLOR | DTT_SHADOWTYPE | DTT_SHADOWOFFSET | DTT_GLOWSIZE;
//			opts.dwFlags = DTT_COMPOSITED | DTT_TEXTCOLOR | DTT_SHADOWCOLOR | DTT_SHADOWTYPE | DTT_SHADOWOFFSET | DTT_GLOWSIZE;
			opts.crText = RGB(0, 255, 0);
			opts.crShadow = RGB(255, 0, 0);
			opts.iTextShadowType = TST_CONTINUOUS;
			opts.ptShadowOffset = get_shadow_offset_as_int();
			opts.iGlowSize = get_shadow_size_as_int();

			return S_OK == hive.orig.DrawThemeTextEx(theme.get(), dc, 0, 0, text, c, text_flags, (LPRECT)rc, &opts);
#else
			// 影を描画する場合は
			if (hive.shadow.flag_use && pigment->text_shadow.is_valid())
			{
				// テキストの影を描画します。
				TextShadowAttribute text_attribute(dc, pigment, opaque);

				auto offset = get_shadow_offset_as_int();

				auto rc2 = *rc;
				::OffsetRect(&rc2, offset.x, offset.y);

				hive.orig.DrawTextW(dc, text, c, &rc2, text_flags);
			}

			// テキストを描画します。

			TextAttribute text_attribute(dc, pigment, FALSE);

			return !!hive.orig.DrawTextW(dc, text, c, (LPRECT)rc, text_flags);
#endif
		}

		//
		// ピグメントを使用して絵文字を描画します。
		//
		BOOL draw_icon(HDC dc, LPCRECT rc, const Pigment* pigment, LPCWSTR font_name, WCHAR char_code, int font_weight = 0)
		{
			// アイコンの属性をセットします。
			IconAttribute icon_attribute(dc, rc, font_name, font_weight);

			// アイコンを描画します。
			return draw_text(dc, rc, &char_code, 1,
				DT_CENTER | DT_VCENTER | DT_SINGLELINE, pigment, FALSE);
		}

		//
		// ピグメントを使用して文字列を描画します。
		//
		BOOL d2d_draw_text(HDC dc, LPCRECT rc, LPCWSTR text, int c, DWORD text_flags, const Pigment* pigment, BOOL opaque = TRUE)
		{
			if (hive.jd.use_d2d)
			{
				TextAttribute text_attribute(dc, pigment, opaque);

				if (auto result = d2d::Texter(dc, text, c, rc, text_flags, pigment).draw_text())
					return !!result;
			}

			return draw_text(dc, rc, text, c, text_flags, pigment, opaque);
		}

		//
		// ピグメントを使用して絵文字を描画します。
		//
		BOOL d2d_draw_icon(HDC dc, LPCRECT rc, const Pigment* pigment, LPCWSTR font_name, WCHAR char_code, int font_weight = 0)
		{
			if (hive.jd.use_d2d)
			{
				IconAttribute icon_attribute(dc, rc, font_name, font_weight);

				auto text_flags = UINT { DT_CENTER | DT_VCENTER | DT_SINGLELINE };

				if (auto result = d2d::Texter(dc, &char_code, 1, rc, text_flags, pigment).draw_text())
					return !!result;
			}

			return draw_icon(dc, rc, pigment, font_name, char_code, font_weight);
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
