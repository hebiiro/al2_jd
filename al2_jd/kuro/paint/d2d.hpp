#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはD2Dによる描画を担当します。
	//
	inline struct D2D
	{
		//
		// このクラスはレンダーターゲットとデバイスコンテキストをバインドします。
		// 内部的に使用されます。
		//
		struct Binder
		{
			//
			// コンストラクタです。
			//
			Binder(HDC dc, LPCRECT rc)
			{
				// デバイスコンテキストにバインドします。
				d2d.render_target->BindDC(dc, rc);

				// 描画を開始します。
				d2d.render_target->BeginDraw();
			}

			//
			// デストラクタです。
			//
			~Binder()
			{
				// 描画を終了します。
				if (d2d.render_target->EndDraw() == D2DERR_RECREATE_TARGET)
					d2d.create_render_target();
			}
		};

		ComPtr<IDWriteFactory> dw_factory;
		ComPtr<ID2D1Factory> d2d_factory;
		ComPtr<ID2D1DCRenderTarget> render_target;

		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// 後始末処理を実行します。
		//
		BOOL exit()
		{
			MY_TRACE_FUNC("");

			return TRUE;
		}

		//
		// グラデーションストップコレクションを作成して返します。
		//
		HRESULT create_gradient_stop_collection(
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
		HRESULT create_gradient_brush(
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
		HRESULT create_gradient_brush(
			const D2D1_COLOR_F& start_color,
			const D2D1_COLOR_F& end_color,
			const D2D1_POINT_2F& start_point,
			const D2D1_POINT_2F& end_point,
			ID2D1LinearGradientBrush** ppv)
		{
			ComPtr<ID2D1GradientStopCollection> stop_collection;
			auto hr = create_gradient_stop_collection(start_color, end_color, &stop_collection);
			if (!stop_collection) return hr;

			return create_gradient_brush(start_point, end_point, stop_collection.Get(), ppv);
		}

		//
		// 丸角矩形を描画します。
		//
		BOOL draw_round_rect(HDC dc, LPCRECT rc, float radius, const Pigment* pigment)
		{
			MY_TRACE_FUNC("{/hex}, ({/}), {/}", dc, safe_string(rc), radius);

			// 描画の準備に失敗した場合は何もしません。
			if (!prepare()) return FALSE;

			auto iw = my::get_width(*rc);
			auto ih = my::get_height(*rc);

			// 描画矩形が無効の場合は何もしません。
			if (iw < 0 || ih < 0) return FALSE;

			// レンダーターゲットとデバイスコンテキストをバインドします。
			Binder binder(dc, rc);

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
				if (hive.jd.as_gradient)
				{
					// 終了色のインデックスを取得します。
					auto end_color_index = color_entry.colors[1].is_valid() ? 1 : 0;

					// グラデーションブラシで背景を描画します。
					ComPtr<ID2D1LinearGradientBrush> gradient_brush;
					create_gradient_brush(
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
#if 0
			// 模様を描画します。
			{
				// 丸角矩形のジオメトリを作成します。
				ComPtr<ID2D1RoundedRectangleGeometry> geometry;
				d2d_factory->CreateRoundedRectangleGeometry(rrc, &geometry);
				if (!geometry) return FALSE;

				// レイヤーを作成します。
				ComPtr<ID2D1Layer> layer;
				render_target->CreateLayer(&layer);
				if (!layer) return FALSE;

				// ジオメトリでクリップします。
				auto layer_params = D2D1::LayerParameters();
				layer_params.geometricMask = geometry.Get();
				render_target->PushLayer(layer_params, layer.Get());

				// 下部に矩形を描画します。
				ComPtr<ID2D1SolidColorBrush> brush;
				render_target->CreateSolidColorBrush(make_color(RGB(255, 0, 0), 0.5f), &brush);
				if (!brush) return FALSE;
				auto under_rc = D2D1::RectF(rrc.rect.left, rrc.rect.bottom - 1.5f, rrc.rect.right, rrc.rect.bottom);
				render_target->FillRectangle(under_rc, brush.Get());

				// クリップを解除します。
				render_target->PopLayer();
			}
#endif
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
					blend(color_entry.colors[0], color_entry_for_blend, 0),
					blend(color_entry.colors[end_color_index], color_entry_for_blend, 1),
					stop_point.first, stop_point.second,
					&gradient_brush);
				if (!gradient_brush) return FALSE;
				render_target->DrawRoundedRectangle(rrc, gradient_brush.Get(), border_width);
			}

			return TRUE;
		}

		//
		// プレフィックスを処理します。
		// 内部的に使用されます。
		//
		inline static std::wstring process_prefix(const std::wstring& str, std::vector<DWRITE_TEXT_RANGE>& underlines)
		{
			// 出力する文字列です。
			std::wstring out;

			// プレフィックス付きフラグです。
			bool prefixed = false;

			// すべての文字を走査します。
			for (auto ch : str)
			{
				// プレフィックス付きの場合は
				if (prefixed)
				{
					// 文字を出力に追加します。
					out.push_back(ch);

					// 文字がプレフィックスではない場合は
					if (ch != L'&')
					{
						// 最後に追加した文字をアンダーライン対象にします。
						underlines.emplace_back((UINT32)out.length() - 1, 1);
					}

					// プレフィックス付きフラグをリセットします。
					prefixed = false;
				}
				// 文字がプレフィックスの場合は
				else if (ch == L'&')
				{
					// プレフィックス付きフラグをセットします。
					prefixed = true;
				}
				// それ以外の場合は
				else
				{
					// 文字を出力に追加します。
					out.push_back(ch);
				}
			}

			// プレフィックス付きの場合は
			if (prefixed)
			{
				// 最後のプレフィックスはそのまま出力します。
				out.push_back(L'&');
			}

			return out;
		}

		//
		// ::DrawTextW()方式で文字列を描画します。
		//
		int draw_text(HDC dc, LPCWSTR text, int c, LPRECT rc, UINT flags, const Pigment* pigment)
		{
			MY_TRACE_FUNC("{/hex}, {/}, ({/}), {/hex}",
				dc, safe_string(text, c), safe_string(rc), flags);

			// 引数が無効の場合は何もしません。
			if (!text || !rc) return 0;

			// ピグメントが無効の場合は何もしません。
			if (!pigment->text.is_valid()) return 0;

			// 描画の準備に失敗した場合は何もしません。
			if (!prepare()) return 0;

			// レンダーターゲットとデバイスコンテキストをバインドします。
			Binder binder(dc, rc);

			// 文字列を取得します。
			auto s = (c < 0) ? std::wstring(text) : std::wstring(text, c);

			// アンダーライン対象を取得します。
			auto underlines = std::vector<DWRITE_TEXT_RANGE> {};
			if (!(flags & DT_NOPREFIX)) s = process_prefix(s, underlines);

			// 描画寸法を取得します。
			auto x = (float)rc->left;
			auto y = (float)rc->top;
			auto w = (float)my::get_width(*rc);
			auto h = (float)my::get_height(*rc);

			// テキスト用のカラーエントリを取得します。
			const auto& color_entry = pigment->text.entry;

			// テキストフォーマットを取得します。
			ComPtr<IDWriteTextFormat> text_format;
			{
				// フォントを取得します。
				auto font = (HFONT)::GetCurrentObject(dc, OBJ_FONT);

				// 論理フォントを取得します。
				LOGFONTW lf = {}; ::GetObjectW(font, sizeof(lf), &lf);

				// フォントの寸法を取得します。
				TEXTMETRICW tm = {};
				::GetTextMetricsW(dc, &tm);

				// フォントサイズを取得します。
				auto font_size = (float)(tm.tmHeight - tm.tmInternalLeading) - 0.5f;

				// テキストフォーマットを作成します。
				dw_factory->CreateTextFormat(
					lf.lfFaceName,
					nullptr,
					(lf.lfWeight >= FW_BOLD) ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
					(lf.lfItalic) ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					font_size,
					L"",
					&text_format);
				if (!text_format) return 0;

				// 横方向のアラインを設定します。
				{
					auto text_alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

					if (flags & DT_CENTER) text_alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
					else if (flags & DT_RIGHT) text_alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;

					text_format->SetTextAlignment(text_alignment);
				}

				// 縦方向のアラインを設定します。
				{
					auto paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;

					if (flags & DT_VCENTER) paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
					else if (flags & DT_BOTTOM) paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_FAR;

					text_format->SetParagraphAlignment(paragraph_alignment);
				}
			}

			// テキストレイアウトを取得します。
			ComPtr<IDWriteTextLayout> text_layout;
			{
				// テキストレイアウトを作成します。
				dw_factory->CreateTextLayout(s.c_str(), (UINT32)s.length(), text_format.Get(), w, h, &text_layout);
				if (!text_layout) return 0;

				// 折り返さないように設定します。
				text_layout->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

				// クリップされるように設定します。
				text_layout->SetMaxWidth(w);
				text_layout->SetMaxHeight(h);

				// アンダーラインを適用します。
				for (const auto& range : underlines)
					text_layout->SetUnderline(TRUE, range);
			}
#if 0
			// テスト用コードです。
			// 縦グラデーションで文字列を描画します。

			// グラデーションブラシを取得します。
			ComPtr<ID2D1LinearGradientBrush> text_brush;
			create_gradient_brush(
				to_d2d_color(color_entry.colors[0]),
				D2D1::ColorF(1.0f, 0.8f, 0.8f, 1.0f),
				D2D1::Point2F(0.0f, 0.0f),
				D2D1::Point2F(0.0f, h),
				&text_brush);
			if (!text_brush) return 0;
#else
			// ブラシを取得します。
			ComPtr<ID2D1SolidColorBrush> text_brush;
			{
				// 文字の色をD2D形式で取得します。
				auto d2d_text_color = to_d2d_color(color_entry.colors[0]);

				// ブラシを作成します。
				render_target->CreateSolidColorBrush(d2d_text_color, &text_brush);
				if (!text_brush) return 0;
			}
#endif
			// 文字列を描画します。
			render_target->DrawTextLayout(
				D2D1::Point2F(0.0f, 0.0f),
				text_layout.Get(),
				text_brush.Get(),
				D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);

			// 文字の高さを返します。
			DWRITE_TEXT_METRICS text_metrics = {};
			text_layout->GetMetrics(&text_metrics);
			return (int)text_metrics.height;
		}

		//
		// 各種ファクトリを作成します。
		//
		HRESULT create_factory()
		{
			auto hr = ::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory), (IUnknown**)dw_factory.GetAddressOf());
			if (!dw_factory) return hr;

			hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2d_factory.GetAddressOf());
			if (!d2d_factory) return hr;

			return hr;
		}

		//
		// レンダーターゲットを作成します。
		//
		HRESULT create_render_target()
		{
			if (!d2d_factory) return E_FAIL;

			auto props = D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(
					DXGI_FORMAT_B8G8R8A8_UNORM,
					D2D1_ALPHA_MODE_PREMULTIPLIED),
//					D2D1_ALPHA_MODE_IGNORE),
				0, 0,
				D2D1_RENDER_TARGET_USAGE_NONE,
				D2D1_FEATURE_LEVEL_DEFAULT);

			auto hr = d2d_factory->CreateDCRenderTarget(&props, render_target.ReleaseAndGetAddressOf());
			if (!render_target) return hr;

			return hr;
		}

		//
		// 描画の準備をします。
		//
		BOOL prepare()
		{
			MY_TRACE_FUNC("");

			if (!d2d_factory)
				if (FAILED(create_factory())) return FALSE;

			if (!render_target)
				if (FAILED(create_render_target())) return FALSE;

			return TRUE;
		}
	} d2d;
}
