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
			const ColorEntry& entry;
			StyleColor(const ColorEntry& entry = {}) : entry(entry) {}
			StyleColor(Style::Color color_index) : entry(style.get_entry(color_index)) {}
			operator const ColorEntry&() const { return entry; }
		};

		//
		// ピグメントを作成して返します。
		//
		inline static Pigment create_pigment(const std::wstring& section, const std::wstring& sub_key,
			const StyleColor& background_color, const StyleColor& border_color, const StyleColor& text_color)
		{
			return {
				{ custom_style.get_background_color(section, sub_key, background_color), },
				{ custom_style.get_border_color(section, sub_key, border_color), },
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
				default_pigment.background.entry,
				default_pigment.border.entry,
				default_pigment.text.entry);
		}

		//
		// 背景色のみのピグメントを作成して返します。
		//
		inline static Pigment create_background_pigment(const std::wstring& section, const std::wstring& key, const StyleColor& background_color)
		{
			return {
				{ custom_style.get_color(section, key, background_color), },
				{},
				{},
			};
		}
	};
}
