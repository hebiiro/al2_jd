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

			auto track_normal = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBody), },
				{ style.get_COLORREF(Style::Color::WindowBorder), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto button_normal = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBody), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto button_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyDisable), },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			auto button_hot = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
				{ style.get_COLORREF(Style::Color::BorderFocus), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto button_pressed = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodyPress), },
				{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto button_focused = button_pressed;

			auto thumb_normal = button_normal;
			auto thumb_disabled = button_disabled;
			auto thumb_hot = button_hot;
			auto thumb_pressed = button_pressed;
			auto thumb_focused = button_focused;

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
