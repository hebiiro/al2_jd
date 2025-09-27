#pragma once

namespace apn::dark::kuro::paint::d2d
{
	//
	// このクラスはD2Dを使用した矩形の描画を担当します。
	//
	struct Recter
	{
		//
		// グラデーションストップコレクションを作成して返します。
		//
		inline static HRESULT create_gradient_stop_collection(
			const auto& render_target,
			const D2D1_COLOR_F& start_color,
			const D2D1_COLOR_F& end_color,
			ID2D1GradientStopCollection** ppv)
		{
			D2D1_GRADIENT_STOP stops[2] = { { 0.0f, start_color }, { 1.0f, end_color } };

			return render_target->CreateGradientStopCollection(
				stops, std::size(stops), D2D1_GAMMA_1_0, D2D1_EXTEND_MODE_CLAMP, ppv);
		}

		//
		// グラデーションブラシを作成して返します。
		//
		inline static HRESULT create_gradient_brush(
			const auto& render_target,
			const D2D1_POINT_2F& start_point,
			const D2D1_POINT_2F& end_point,
			ID2D1GradientStopCollection* stop_collection,
			ID2D1LinearGradientBrush** ppv)
		{
			D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES props = { start_point, end_point };

			return render_target->CreateLinearGradientBrush(props, stop_collection, ppv);
		}

		//
		// グラデーションブラシを作成して返します。
		//
		inline static HRESULT create_gradient_brush(
			const auto& render_target,
			const D2D1_COLOR_F& start_color,
			const D2D1_COLOR_F& end_color,
			const D2D1_POINT_2F& start_point,
			const D2D1_POINT_2F& end_point,
			ID2D1LinearGradientBrush** ppv)
		{
			ComPtr<ID2D1GradientStopCollection> stop_collection;
			auto hr = create_gradient_stop_collection(
				render_target, start_color, end_color, &stop_collection);
			if (!stop_collection) return hr;

			return create_gradient_brush(render_target,
				start_point, end_point, stop_collection.Get(), ppv);
		}

		//
		// 丸角矩形を描画します。
		//
		BOOL draw_round_rect(HDC dc, LPCRECT rc, float radius, const Pigment* pigment)
		{
			MY_TRACE_FUNC("{/hex}, ({/}), {/}", dc, safe_string(rc), radius);

			// 描画の準備に失敗した場合は何もしません。
			if (!core.prepare()) return FALSE;

			auto iw = my::get_width(*rc);
			auto ih = my::get_height(*rc);

			// 描画矩形が無効の場合は何もしません。
			if (iw < 0 || ih < 0) return FALSE;

			// レンダーターゲットとデバイスコンテキストをバインドします。
			Core::Binder binder(dc, rc);

			// レンダーターゲットを取得します。
			const auto& render_target = core.render_target;

			// 縁の幅を取得します。
			auto border_width = get_border_width_as_float();
			auto half_border_width = border_width / 2.0f;

			// 全体の矩形を取得します。
			auto w = (float)iw;
			auto h = (float)ih;
			auto whole_rc = D2D1::RectF(0, 0, w, h);

			// 縁がはみ出さないように収縮した矩形を取得します。
			auto draw_rc = deflate(whole_rc, half_border_width, half_border_width);

			// 丸角矩形を取得します。
			auto rrc = D2D1::RoundedRect(draw_rc, radius, radius);

			// 背景の描画データが有効の場合は
			if (pigment->background.is_valid())
			{
				// 背景を描画します。

				// 背景用のカラーエントリを取得します。
				const auto& color_entry = pigment->background.entry;

				// グラデーションで描画する場合は
				if (hive.gradient.flag_use)
				{
					// 終了色のインデックスを取得します。
					auto end_color_index = color_entry.colors[1].is_valid() ? 1 : 0;

					// グラデーションブラシで背景を描画します。
					ComPtr<ID2D1LinearGradientBrush> gradient_brush;
					create_gradient_brush(
						render_target,
						to_d2d_color(color_entry.colors[0]),
						get_background_end_color(color_entry.colors[end_color_index]),
						D2D1::Point2F(draw_rc.left, draw_rc.top),
						D2D1::Point2F(draw_rc.right, draw_rc.bottom),
						&gradient_brush);
					if (!gradient_brush) return FALSE;
					render_target->FillRoundedRectangle(rrc, gradient_brush.Get());
				}
				// それ以外の場合は
				else
				{
					// カラーブラシで背景を描画します。
					ComPtr<ID2D1SolidColorBrush> brush;
					render_target->CreateSolidColorBrush(
						to_d2d_color(color_entry.colors[0]), &brush);
					if (!brush) return FALSE;
					render_target->FillRoundedRectangle(rrc, brush.Get());
				}
			}

			// 縁の描画データが有効の場合は
			if (pigment->border.is_valid() && border_width > 0.0f)
			{
				// 縁を描画します。

				// 縁用のカラーエントリを取得します。
				const auto& color_entry = pigment->background.entry;

				// グラデーションストップ座標を取得します。
				// radiusが0だと計算不可能なので最小値を指定しています。
				auto stop_point = [&, radius = std::max(radius, 0.1f)]()
				{
					auto a = D2D1::Point2F(draw_rc.left, draw_rc.bottom - radius);
					auto b = D2D1::Point2F(draw_rc.right - radius, draw_rc.top);
					auto c = D2D1::Point2F(draw_rc.left + radius, draw_rc.bottom);
					auto d = D2D1::Point2F(draw_rc.right, draw_rc.top + radius);

					return std::make_pair(a, perpendicular(a, b, c, d));
				} ();

				// 終了カラーを決定します。
				auto end_color_index = color_entry.colors[1].is_valid() ? 1 : 0;

				// ブレンド用のカラーエントリを取得します。
				auto color_entry_for_blend = get_3d_edge_entry();

				// グラデーションブラシで縁を描画します。
				ComPtr<ID2D1LinearGradientBrush> gradient_brush;
				create_gradient_brush(
					render_target,
					blend(color_entry.colors[0], color_entry_for_blend, 0),
					blend(color_entry.colors[end_color_index], color_entry_for_blend, 1),
					stop_point.first, stop_point.second,
					&gradient_brush);
				if (!gradient_brush) return FALSE;
				render_target->DrawRoundedRectangle(rrc, gradient_brush.Get(), border_width);
			}

			return TRUE;
		}
	};
}
