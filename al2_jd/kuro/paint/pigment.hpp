#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはピグメントです。
	// GDIやテーマの描画に使用されます。
	//
	struct Pigment
	{
		struct Background
		{
			COLORREF color;
			mutable HBRUSH brush;

			//
			// 背景が描画可能な場合はTRUEを返します。
			//
			BOOL is_valid() const
			{
				return color != CLR_NONE;
			}

			//
			// 背景を描画するためのブラシを返します。
			//
			HBRUSH get_brush() const
			{
				if (brush) return brush;

				if (is_valid())
					return brush = ::CreateSolidBrush(color);
				else
					return brush = (HBRUSH)::GetStockObject(NULL_BRUSH);
			}
		} background;

		struct Border
		{
			COLORREF color;
			int width;

			//
			// 縁が描画可能な場合はTRUEを返します。
			//
			BOOL is_valid() const
			{
				return color != CLR_NONE && width > 0;
			}
		} border;

		struct Text
		{
			COLORREF color;
		} text;
	};
}
