#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはシステムカラーのマテリアルです。
	//
	inline struct SysColorMaterial : Material
	{
		Pigment window;
		Pigment window_text;
		Pigment highlight;
		Pigment highlight_text;
		Pigment btn_face;
		Pigment btn_text;
		Pigment btn_highlight;
		Pigment gray_text;

		Pigment single_border;

		struct SingleEdge {
			Pigment top_left;
			Pigment bottom_right;
		};

		struct Edge {
			SingleEdge outer;
			SingleEdge inner;
		};

		SingleEdge single_raised_edge;
		SingleEdge single_sunken_edge;
		Edge raised_edge;
		Edge sunken_edge;
		Edge bump_edge;
		Edge etched_edge;

		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			window = create_background_pigment(L"SysColor", L"Window", Style::Color::Background);
			window_text = create_background_pigment(L"SysColor", L"WindowText", Style::Color::Text);
			highlight = create_background_pigment(L"SysColor", L"Highlight", Style::Color::ButtonBodySelect);
			highlight_text = create_background_pigment(L"SysColor", L"HighlightText", Style::Color::Text);
			btn_face = create_background_pigment(L"SysColor", L"BtnFace", Style::Color::ButtonBody);
			btn_text = create_background_pigment(L"SysColor", L"BtnText", Style::Color::Text);
			btn_highlight = create_background_pigment(L"SysColor", L"BtnHighlight", Style::Color::ButtonBody);
			gray_text = create_background_pigment(L"SysColor", L"GrayText", Style::Color::TextDisable);

			single_border = create_background_pigment(L"SysColor", L"SingleBorder", Style::Color::Border);

			single_raised_edge.top_left = create_background_pigment(L"SysColor", L"SingleRaisedEdgeTopLeft", Style::Color::Border);
			single_raised_edge.bottom_right = create_background_pigment(L"SysColor", L"SingleRaisedEdgeBottomRight", Style::Color::Border);

			single_sunken_edge.top_left = create_background_pigment(L"SysColor", L"SingleSunkenEdgeTopLeft", Style::Color::Border);
			single_sunken_edge.bottom_right = create_background_pigment(L"SysColor", L"SingleSunkenEdgeBottomRight", Style::Color::Border);

			raised_edge.outer.top_left = create_background_pigment(L"SysColor", L"RaisedEdgeOuterTopLeft", Style::Color::Border);
			raised_edge.outer.bottom_right = create_background_pigment(L"SysColor", L"RaisedEdgeOuterBottomRight", Style::Color::Border);
			raised_edge.inner.top_left = create_background_pigment(L"SysColor", L"RaisedEdgeInnerTopLeft", Style::Color::ButtonBody);
			raised_edge.inner.bottom_right = create_background_pigment(L"SysColor", L"RaisedEdgeInnerBottomRight", Style::Color::ButtonBody);

			sunken_edge.outer.top_left = create_background_pigment(L"SysColor", L"SunkenEdgeOuterTopLeft", Style::Color::ButtonBody);
			sunken_edge.outer.bottom_right = create_background_pigment(L"SysColor", L"SunkenEdgeOuterBottomRight", Style::Color::ButtonBody);
			sunken_edge.inner.top_left = create_background_pigment(L"SysColor", L"SunkenEdgeInnerTopLeft", Style::Color::Border);
			sunken_edge.inner.bottom_right = create_background_pigment(L"SysColor", L"SunkenEdgeInnerBottomRight", Style::Color::Border);

			bump_edge.outer.top_left = create_background_pigment(L"SysColor", L"BumpEdgeOuterTopLeft", Style::Color::ButtonBodyHover);
			bump_edge.outer.bottom_right = create_background_pigment(L"SysColor", L"BumpEdgeOuterBottomRight", Style::Color::ButtonBody);
			bump_edge.inner.top_left = create_background_pigment(L"SysColor", L"BumpEdgeInnerTopLeft", Style::Color::ButtonBody);
			bump_edge.inner.bottom_right = create_background_pigment(L"SysColor", L"BumpEdgeInnerBottomRight", Style::Color::ButtonBodyHover);

			etched_edge.outer.top_left = create_background_pigment(L"SysColor", L"EtchedEdgeOuterTopLeft", Style::Color::ButtonBody);
			etched_edge.outer.bottom_right = create_background_pigment(L"SysColor", L"EtchedEdgeOuterBottomRight", Style::Color::ButtonBodyHover);
			etched_edge.inner.top_left = create_background_pigment(L"SysColor", L"EtchedEdgeInnerTopLeft", Style::Color::ButtonBodyHover);
			etched_edge.inner.bottom_right = create_background_pigment(L"SysColor", L"EtchedEdgeInnerBottomRight", Style::Color::ButtonBody);

			palette.set(COLOR_WINDOW, 0, window);
			palette.set(COLOR_WINDOWTEXT, 0, window_text);
			palette.set(COLOR_HIGHLIGHT, 0, highlight);
			palette.set(COLOR_HIGHLIGHTTEXT, 0, highlight_text);
			palette.set(COLOR_BTNFACE, 0, btn_face);
			palette.set(COLOR_BTNTEXT, 0, btn_text);
			palette.set(COLOR_BTNHIGHLIGHT, 0, btn_highlight);
			palette.set(COLOR_GRAYTEXT, 0, gray_text);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}

		//
		// 軽量エッジを描画します。
		//
		inline static void draw_single_edge(HDC dc, LPCRECT rc, SingleEdge& edge)
		{
			return paint::draw_single_edge(dc, rc,
				edge.top_left.background.get_win32_color(),
				edge.bottom_right.background.get_win32_color());
		}

		//
		// エッジを描画します。
		//
		inline static void draw_edge(HDC dc, LPCRECT rc, Edge& edge)
		{
			return paint::draw_edge(dc, rc,
				edge.outer.top_left.background.get_win32_color(),
				edge.outer.bottom_right.background.get_win32_color(),
				edge.inner.top_left.background.get_win32_color(),
				edge.inner.bottom_right.background.get_win32_color());
		}
	} sys_color_material;

	//
	// 軽量ボーダーを描画します。
	//
	inline void draw_single_border(HDC dc, LPCRECT rc)
	{
		return frame_rect(dc, rc, sys_color_material.single_border.background.get_win32_color(), 1);
	}

	//
	// 浮き上がり軽量エッジを描画します。
	//
	inline void draw_single_raised_edge(HDC dc, LPCRECT rc)
	{
		return sys_color_material.draw_single_edge(dc, rc, sys_color_material.single_raised_edge);
	}

	//
	// 沈み込み軽量エッジを描画します。
	//
	inline void draw_single_sunken_edge(HDC dc, LPCRECT rc)
	{
		return sys_color_material.draw_single_edge(dc, rc, sys_color_material.single_sunken_edge);
	}

	//
	// 浮き上がりエッジを描画します。
	//
	inline void draw_raised_edge(HDC dc, LPCRECT rc)
	{
		return sys_color_material.draw_edge(dc, rc, sys_color_material.raised_edge);
	}

	//
	// 沈み込みエッジを描画します。
	//
	inline void draw_sunken_edge(HDC dc, LPCRECT rc)
	{
		return sys_color_material.draw_edge(dc, rc, sys_color_material.sunken_edge);
	}

	//
	// 凸エッジを描画します。
	//
	inline void draw_bump_edge(HDC dc, LPCRECT rc)
	{
		return sys_color_material.draw_edge(dc, rc, sys_color_material.bump_edge);
	}

	//
	// 凹エッジを描画します。
	//
	inline void draw_etched_edge(HDC dc, LPCRECT rc)
	{
		return sys_color_material.draw_edge(dc, rc, sys_color_material.etched_edge);
	}
}
