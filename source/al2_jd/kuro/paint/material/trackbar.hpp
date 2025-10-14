#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはトラックバーのマテリアルです。
	//
	inline struct TrackBarMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			auto track_normal = create_pigment(L"TrackBar", L"Normal",
				Style::Color::ButtonBody,
				Style::Color::WindowBorder,
				Style::Color::Text);

			auto thumb_normal = create_pigment(L"TrackBarThumb", L"Normal",
				Style::Color::ButtonBody,
				Style::Color::Border,
				Style::Color::Text);

			auto thumb_disabled = create_pigment(L"TrackBarThumb", L"Disable",
				Style::Color::ButtonBodyDisable,
				Style::Color::Border,
				Style::Color::TextDisable);

			auto thumb_hot = create_pigment(L"TrackBarThumb", L"Hot",
				Style::Color::ButtonBodySelect,
				Style::Color::BorderFocus,
				Style::Color::Text);

			auto thumb_pressed = create_pigment(L"TrackBarThumb", L"Press",
				Style::Color::ButtonBodyPress,
				Style::Color::BorderSelect,
				Style::Color::Text);

			auto thumb_focused = thumb_pressed;

			palette.set(TKP_TRACK, TRS_NORMAL, track_normal);
			palette.set(TKP_THUMB, TUS_NORMAL, thumb_normal);
			palette.set(TKP_THUMB, TUS_DISABLED, thumb_disabled);
			palette.set(TKP_THUMB, TUS_HOT, thumb_hot);
			palette.set(TKP_THUMB, TUS_PRESSED, thumb_pressed);
			palette.set(TKP_THUMB, TUS_FOCUSED, thumb_focused);
			palette.set(TKP_THUMBBOTTOM, TUS_NORMAL, thumb_normal);
			palette.set(TKP_THUMBBOTTOM, TUS_DISABLED, thumb_disabled);
			palette.set(TKP_THUMBBOTTOM, TUS_HOT, thumb_hot);
			palette.set(TKP_THUMBBOTTOM, TUS_PRESSED, thumb_pressed);
			palette.set(TKP_THUMBBOTTOM, TUS_FOCUSED, thumb_focused);
			palette.set(TKP_THUMBTOP, TUS_NORMAL, thumb_normal);
			palette.set(TKP_THUMBTOP, TUS_DISABLED, thumb_disabled);
			palette.set(TKP_THUMBTOP, TUS_HOT, thumb_hot);
			palette.set(TKP_THUMBTOP, TUS_PRESSED, thumb_pressed);
			palette.set(TKP_THUMBTOP, TUS_FOCUSED, thumb_focused);

			palette.set(TKP_TRACKVERT, TRS_NORMAL, track_normal);
			palette.set(TKP_THUMBVERT, TUS_NORMAL, thumb_normal);
			palette.set(TKP_THUMBVERT, TUS_DISABLED, thumb_disabled);
			palette.set(TKP_THUMBVERT, TUS_HOT, thumb_hot);
			palette.set(TKP_THUMBVERT, TUS_PRESSED, thumb_pressed);
			palette.set(TKP_THUMBVERT, TUS_FOCUSED, thumb_focused);
			palette.set(TKP_THUMBLEFT, TUS_NORMAL, thumb_normal);
			palette.set(TKP_THUMBLEFT, TUS_DISABLED, thumb_disabled);
			palette.set(TKP_THUMBLEFT, TUS_HOT, thumb_hot);
			palette.set(TKP_THUMBLEFT, TUS_PRESSED, thumb_pressed);
			palette.set(TKP_THUMBLEFT, TUS_FOCUSED, thumb_focused);
			palette.set(TKP_THUMBRIGHT, TUS_NORMAL, thumb_normal);
			palette.set(TKP_THUMBRIGHT, TUS_DISABLED, thumb_disabled);
			palette.set(TKP_THUMBRIGHT, TUS_HOT, thumb_hot);
			palette.set(TKP_THUMBRIGHT, TUS_PRESSED, thumb_pressed);
			palette.set(TKP_THUMBRIGHT, TUS_FOCUSED, thumb_focused);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} trackbar_material;
}
