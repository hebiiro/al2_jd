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
			ColorEntry color_entry;
			mutable HBRUSH brush = {};

			//
			// (デフォルト)コンストラクタです。
			//
			Background(const ColorEntry& color_entry = {})
				: color_entry(color_entry)
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
				if (this != &rhs)
					exit(), color_entry = rhs.color_entry;
			}

			//
			// 後始末処理を実行します。
			//
			void exit()
			{
				if (brush)
				{
					// ブラシがストックオブジェクトではない場合は
					if (brush != (HBRUSH)::GetStockObject(NULL_BRUSH))
						::DeleteObject(brush); // ブラシを削除します。

					brush = nullptr;
				}
			}

			//
			// 背景が描画可能な場合はTRUEを返します。
			//
			BOOL is_valid() const
			{
				return color_entry.parts[0].is_valid();
			}

			//
			// 配色をwin32形式で返します。
			//
			COLORREF get_win32_color() const
			{
				return color_entry.parts[0].win32;
			}

			//
			// 背景を描画するためのブラシを返します。
			//
			HBRUSH get_brush() const
			{
				if (brush) return brush;

				if (is_valid())
					return brush = ::CreateSolidBrush(get_win32_color());
				else
					return brush = (HBRUSH)::GetStockObject(NULL_BRUSH);
			}
		} background;

		struct Border
		{
			ColorEntry color_entry;

			//
			// 縁が描画可能な場合はTRUEを返します。
			//
			BOOL is_valid() const
			{
				return color_entry.parts[0].is_valid();
			}

			//
			// 配色をwin32形式で返します。
			//
			COLORREF get_win32_color() const
			{
				return color_entry.parts[0].win32;
			}
		} border;

		struct Text
		{
			ColorEntry color_entry;

			//
			// テキストが描画可能な場合はTRUEを返します。
			//
			BOOL is_valid() const
			{
				return color_entry.parts[0].is_valid();
			}

			//
			// 配色をwin32形式で返します。
			//
			COLORREF get_win32_color() const
			{
				return color_entry.parts[0].win32;
			}
		} text;

		struct TextShadow
		{
			ColorEntry color_entry;

			//
			// テキストの影が描画可能な場合はTRUEを返します。
			//
			BOOL is_valid() const
			{
				return color_entry.parts[0].is_valid();
			}

			//
			// 配色をwin32形式で返します。
			//
			COLORREF get_win32_color() const
			{
				return color_entry.parts[0].win32;
			}
		} text_shadow;
	};
}
