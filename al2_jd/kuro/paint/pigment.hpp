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
			// (デフォルト)コンストラクタです。
			//
			Background(COLORREF color = CLR_NONE)
				: color(color)
				, brush(nullptr)
			{
			}

			//
			// デストラクタです。
			//
			~Background()
			{
				exit();
			}

			//
			// コピーコンストラクタです。
			//
			Background(const Background& rhs)
				: color(CLR_NONE)
				, brush(nullptr)
			{
				assign(rhs);
			}

			//
			// 代入演算子です。
			//
			Background& operator=(const Background& rhs)
			{
				return assign(rhs), *this;
			}

			//
			// 代入処理を実行します。
			//
			void assign(const Background& rhs)
			{
				if (color != rhs.color)
					exit(), color = rhs.color;
			}

			//
			// 後始末処理を実行します。
			//
			void exit()
			{
				if (brush)
					::DeleteObject(brush), brush = nullptr;
			}

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

			//
			// テキストが描画可能な場合はTRUEを返します。
			//
			BOOL is_valid() const
			{
				return color != CLR_NONE;
			}
		} text;
	};
}
