#pragma once

namespace apn::dark::kuro
{
	//
	// このクラスはcustom_style.confの変数を保持します。
	//
	inline struct CustomStyle
	{
		//
		// カスタムカラーのコレクションです。
		//
		std::unordered_map<std::pair<std::wstring, std::wstring>, COLORREF> colors;

		//
		// 指定されたセクションとキーに対応するカスタムカラーを返します。
		//
		COLORREF get_custom_color(const std::wstring& section, const std::wstring& key) const
		{
			auto it = colors.find({ section, key });
			if (it == colors.end()) return CLR_NONE;
			return it->second;
		}

		//
		// 指定された条件のカラーを返します。
		//
		inline COLORREF get_color(const std::wstring& section, const std::wstring& key, COLORREF default_color)
		{
			// カスタムカラーが有効の場合は
			if (auto color = get_custom_color(section, key); color != CLR_NONE)
			{
				// カスタムカラーを返します。
				return color;
			}
			// カスタムカラーが無効の場合は
			else
			{
				// デフォルトカラーを返します。
				return default_color;
			}
		}

		//
		// バックグランドの配色を返します。
		//
		inline COLORREF get_background_color(const std::wstring& section, const std::wstring& sub_key, COLORREF default_color)
		{
			return get_color(section, sub_key + L"Fill", default_color);
		}

		//
		// ボーダーの配色を返します。
		//
		inline COLORREF get_border_color(const std::wstring& section, const std::wstring& sub_key, COLORREF default_color)
		{
			return get_color(section, sub_key + L"Edge", default_color);
		}

		//
		// テキストの配色を返します。
		//
		inline COLORREF get_text_color(const std::wstring& section, const std::wstring& sub_key, COLORREF default_color)
		{
			return get_color(section, sub_key + L"Text", default_color);
		}

		//
		// 文字列をCOLORREFに変換して返します。
		//
		inline static COLORREF to_COLORREF(const std::wstring& str)
		{
			switch (str.length())
			{
			case 6: // rrggbb形式の場合の処理です。
				{
					// 文字列を16進数に変換します。
					auto temp = wcstoul(str.data(), nullptr, 16);

					auto r = (uint8_t)((temp & 0x00FF0000) >> 16);
					auto g = (uint8_t)((temp & 0x0000FF00) >> 8);
					auto b = (uint8_t)((temp & 0x000000FF) >> 0);
					auto a = (uint8_t)(0xff);

					return RGB(r, g, b);
				}
			case 8: // rrggbbaa形式の場合の処理です。
				{
					// 文字列を16進数に変換します。
					auto temp = wcstoul(str.data(), nullptr, 16);

					auto r = (uint8_t)((temp & 0xFF000000) >> 24);
					auto g = (uint8_t)((temp & 0x00FF0000) >> 16);
					auto b = (uint8_t)((temp & 0x0000FF00) >> 8);
					auto a = (uint8_t)((temp & 0x000000FF) >> 0);

					return RGB(r, g, b);
				}
			}

			return CLR_NONE;
		}

		//
		// カスタムカラーファイルを読み込みます。
		//
		BOOL read_custom_color_file(const std::wstring& custom_color_path)
		{
			MY_TRACE_FUNC("{/}", custom_color_path);

			// 現在のセクション名です。
			auto section_name = std::wstring {};

			// ファイルストリームを開きます。UTF-8です。
			std::ifstream stream(custom_color_path);

			// 一行ずつ読み込みます。
			std::string utf8_line;
			while (std::getline(stream, utf8_line))
			{
				// ワイド文字列に変換します。
				auto line = my::cp_to_wide(utf8_line, CP_UTF8);

				// 前後の空白を削除します。
				line = trim(line);

				// 空行は無視します。
				if (line.empty()) continue;

				// コメント行の場合は無視します。
				if (line.starts_with(L';')) continue;

				// セクション行の場合は
				if (line.starts_with(L'[') && line.ends_with(L']'))
				{
					// セクション名を更新します。
					section_name = line.substr(1, line.size() - 2);

					// ループを続けます。
					continue;
				}

				// セパレータの位置を取得します。
				auto separator_pos = line.find_first_of(L'=');

				// セパレータの位置が無効の場合は無視します。
				if (separator_pos == line.npos) continue;

				// キーを取得します。
				auto key = trim(line.substr(0, separator_pos));

				// 値を取得します。
				auto value = trim(line.substr(separator_pos + 1));

				// コレクションに追加します。
				colors[{ section_name, key }] = { to_COLORREF(value) };
			}

			return TRUE;
		}

		//
		// 初期化処理を実行します。
		//
		BOOL init()
		{
			MY_TRACE_FUNC("");

			try
			{
				// プラグインフォルダのパスを取得します。
				auto plugin_folder_path = my::get_module_file_name(hive.instance).parent_path();

				// カスタムカラーファイルのパスを取得します。
				hive.jd.custom_color_file_name = plugin_folder_path / L"al2/config/custom_color.conf";

				// カスタムカラーファイルを読み込みます。
				read_custom_color_file(hive.jd.custom_color_file_name);
			}
			// 例外が発生した場合は
			catch (std::exception& error)
			{
				// メッセージボックスでユーザーに通知します。
				hive.message_box(my::ws(error.what()));
			}

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
	} custom_style;
}
