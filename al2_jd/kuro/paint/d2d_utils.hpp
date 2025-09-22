#pragma once

namespace apn::dark::kuro::paint
{
	//
	// 2つの平行線abとcdに対して、aを通る垂直線と、cdの交点を返します。
	//
	inline D2D1_POINT_2F perpendicular(
		const D2D1_POINT_2F& a, const D2D1_POINT_2F& b,
		const D2D1_POINT_2F& c, const D2D1_POINT_2F& d)
	{
		// 方向ベクトル v = b - a
		auto vx = b.x - a.x;
		auto vy = b.y - a.y;
		auto vnorm2 = vx * vx + vy * vy;
		if (vnorm2 == 0.0f) return {};

		// t = ((a - c) . v) / (v . v)
		auto acx = a.x - c.x;
		auto acy = a.y - c.y;
		auto t = (acx * vx + acy * vy) / vnorm2;

		// f = c + t * v
		return {
			c.x + t * vx,
			c.y + t * vy,
		};
	}

	//
	// 配色をD2D形式に変換して返します。
	//
	inline auto to_d2d_color(const Color& color)
	{
		return D2D1::ColorF(color.rgba.value >> 8, color.alpha());
	}

	//
	// 配色をD2D形式に変換して返します。
	//
	inline auto to_d2d_color(const Color& color, float alpha)
	{
		return D2D1::ColorF(color.rgba.value >> 8, alpha);
	}

	//
	// 背景の終了色のアルファを実数で返します。
	//
	inline float get_background_end_alpha(float base_alpha)
	{
		if (hive.jd.use_gradient_end_alpha)
			return base_alpha * get_gradient_end_alpha();
		else
			return base_alpha;
	}

	//
	// 背景の終了色をD2D形式で返します。
	//
	inline auto get_background_end_color(const Color& end_color)
	{
		auto end_alpha = get_background_end_alpha(end_color.alpha());

		return to_d2d_color(end_color, end_alpha);
	}

	//
	// 背景の終了色のアルファを実数で返します。
	//
	inline const ColorEntry* get_3d_edge_entry()
	{
		if (hive.jd.as_3d_edge)
			return custom_style.get_color(L"3DEdge", L"Raised");
		else
			return {};
	}

	//
	// 基本色に補助色をブレンドしてD2D形式で返します。
	//
	inline auto blend(const Color& base_color, const Color& sub_color)
	{
		if (!sub_color.is_valid()) return to_d2d_color(base_color);

		auto t = sub_color.alpha();
		auto s = 1.0f - t;

		auto r = (base_color.red() * s) + (sub_color.red() * t);
		auto g = (base_color.green() * s) + (sub_color.green() * t);
		auto b = (base_color.blue() * s) + (sub_color.blue() * t);
		auto a = base_color.alpha();

		return D2D1::ColorF(r, g, b, a);
	}

	//
	// 基本色に補助色をブレンドしてD2D形式で返します。
	//
	inline auto blend(const Color& base_color, const ColorEntry* entry, size_t color_index)
	{
		if (!entry) return to_d2d_color(base_color);

		return blend(base_color, entry->colors[color_index]);
	}

	//
	// 矩形を収縮して返します。
	//
	inline D2D1_RECT_F deflate(const D2D1_RECT_F& rc, float x, float y)
	{
		D2D1_RECT_F out_rc = rc;
		out_rc.left += x;
		out_rc.top += y;
		out_rc.right -= x;
		out_rc.bottom -= y;
		return out_rc;
	}

	//
	// 矩形を収縮して返します。
	//
	inline D2D1_ROUNDED_RECT deflate(const D2D1_ROUNDED_RECT& rc, float x, float y)
	{
		D2D1_ROUNDED_RECT out_rc = rc;
		out_rc.rect.left += x;
		out_rc.rect.top += y;
		out_rc.rect.right -= x;
		out_rc.rect.bottom -= y;
		return out_rc;
	}
}
