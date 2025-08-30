#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはマテリアルです。
	//
	struct Material
	{
		//
		// パレットです。
		//
		Palette palette;

		//
		// マテリアルのコレクションです。
		//
		inline static std::vector<Material*> materials;

		//
		// コンストラクタです。
		//
		Material()
		{
			// このマテリアルをコレクションに追加します。
			materials.emplace_back(this);
		}

		//
		// すべてのマテリアルの初期化処理を実行します。
		//
		inline static void init_materials()
		{
			for (auto material : materials) material->on_init_material();
		}

		//
		// すべてのマテリアルの後始末処理を実行します。
		//
		inline static void exit_materials()
		{
			for (auto material : materials) material->on_exit_material();
		}

		//
		// コンストラクタのタイミングでは
		// まだスタイルの初期化が終わっていないので、
		// この仮想関数でマテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() {}

		//
		// この仮想関数でマテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() {}

		//
		// このクラスはスタイルの配色です。
		//
		struct StyleColor {
			COLORREF color = CLR_NONE;
			StyleColor(Style::Color color_index) : color(style.get_COLORREF(color_index)) {}
			StyleColor(COLORREF color) : color(color) {}
			operator COLORREF() const { return color; }
		};

		//
		// ピグメントを作成して返します。
		//
		inline static Pigment create_pigment(const std::wstring& section, const std::wstring& sub_key,
			const StyleColor& background_color, const StyleColor& border_color, const StyleColor& text_color)
		{
			return {
				{ custom_style.get_background_color(section, sub_key, background_color), },
				{ custom_style.get_border_color(section, sub_key, border_color), 1, },
				{ custom_style.get_text_color(section, sub_key, text_color), },
			};
		}

		//
		// ピグメントを作成して返します。
		//
		inline static Pigment create_pigment(const std::wstring& section, const std::wstring& sub_key, const Pigment& default_pigment)
		{
			return create_pigment(
				section, sub_key,
				default_pigment.background.color,
				default_pigment.border.color,
				default_pigment.text.color);
		}

		//
		// 背景色のみのピグメントを作成して返します。
		//
		inline static Pigment create_background_pigment(const std::wstring& section, const std::wstring& key, const StyleColor& background_color)
		{
			return {
				{ custom_style.get_color(section, key, background_color), },
				{ CLR_NONE },
				{ CLR_NONE },
			};
		}
	};
}
