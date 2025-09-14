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
		// ::DrawTextW()形式で文字列を描画します。
		//
		int draw_text(HDC dc, LPCWSTR text, int c, LPRECT rc, UINT flags)
		{
			MY_TRACE_FUNC("{/hex}, {/}, ({/}), {/hex}",
				dc, safe_string(text, c), safe_string(rc), flags);

			// 引数が無効の場合は何もしません。
			if (!text || !rc) return 0;

			// 描画の準備をします。
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

			// ブラシを取得します。
			ComPtr<ID2D1SolidColorBrush> text_brush;
			{
				// win32形式の文字色を取得します。
				auto win32_text_color = ::GetTextColor(dc);

				// D2D形式の文字色を取得します。
				auto d2d_text_color = D2D1::ColorF(win32_text_color, 1.0f);

				// ブラシを作成します。
				render_target->CreateSolidColorBrush(d2d_text_color, &text_brush);
				if (!text_brush) return 0;
			}

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
