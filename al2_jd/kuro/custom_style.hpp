#pragma once

namespace apn::dark::kuro
{
	//
	// 指定されたカラーエントリを結合します。
	//
	inline void operator+=(ColorEntry& lhs, const ColorEntry* rhs)
	{
		// 右辺のカラーエントリが無効の場合は何もしません。
		if (!rhs) return;

		// 右辺のカラーパーツを走査します。
		for (size_t i = 0; i < rhs->c_max_size; i++)
		{
			// 右辺のカラーパーツが有効の場合は
			if (rhs->colors[i].is_valid())
			{
				// 左辺のカラーパーツを上書きします。
				lhs.colors[i] = rhs->colors[i];
			}
		}
	}

	//
	// このクラスはcustom_color.confの変数を保持します。
	//
	inline struct CustomStyle
	{
		//
		// カスタムカラーのコレクションです。
		//
		std::unordered_map<std::pair<std::wstring, std::wstring>, ColorEntry> entries;

		//
		// 指定されたセクションとキーに対応するカスタムカラーを返します。
		//
		const ColorEntry* get_color(const std::wstring& section, const std::wstring& key) const
		{
			auto it = entries.find({ section, key });
			if (it == entries.end()) return {};
			return &it->second;
		}

		//
		// 指定された条件のカスタムカラーを返します。
		//
		inline ColorEntry get_color(const std::wstring& section, const std::wstring& key, const ColorEntry& default_color)
		{
			// 一旦デフォルトカラーをセットします。
			auto out_entry = default_color;

			// カスタムカラーを結合します
			out_entry += get_color(section, key);

			// 出力カラーエントリを返します。
			return out_entry;
		}

		//
		// バックグランドのカスタムカラーを返します。
		//
		inline ColorEntry get_background_color(const std::wstring& section, const std::wstring& sub_key, const ColorEntry& default_color)
		{
			return get_color(section, sub_key + L"Fill", default_color);
		}

		//
		// ボーダーのカスタムカラーを返します。
		//
		inline ColorEntry get_border_color(const std::wstring& section, const std::wstring& sub_key, const ColorEntry& default_color)
		{
			return get_color(section, sub_key + L"Edge", default_color);
		}

		//
		// テキストのカスタムカラーを返します。
		//
		inline ColorEntry get_text_color(const std::wstring& section, const std::wstring& sub_key, const ColorEntry& default_color)
		{
			return get_color(section, sub_key + L"Text", default_color);
		}

		//
		// テキストの影のカスタムカラーを返します。
		//
		inline ColorEntry get_text_shadow_color(const std::wstring& section, const std::wstring& sub_key)
		{
			// 出力カラーエントリです。
			auto out_entry = ColorEntry {};

			// 各カラーエントリを出力カラーエントリに結合します。
			out_entry += get_color(L"Common", L"TextShadow");
			out_entry += get_color(section, sub_key + L"TextShadow");

			// 出力カラーエントリを返します。
			return out_entry;
		}

		//
		// 文字列を配色データに変換してコレクションに追加します。
		//
		void add(const std::wstring& section, const std::wstring& key, const std::wstring& value)
		{
			// 引数が無効の場合は何もしません。
			if (section.empty() || key.empty() || value.empty()) return;

			// 値を配列に分割します。
			auto vec = split(value, L',');

			// 最大数を取得します。
			auto c = std::min(ColorEntry::c_max_size, vec.size());

			// コレクションに追加予定のカスタムカラーです。
			auto entry = ColorEntry {};

			// 配列を走査します。
			for (size_t i = 0; i < c; i++)
			{
				// 文字列を取得します。
				const auto& str = vec[i];

				// 文字列が空の場合は何もしません。
				if (str.empty()) continue;

				// 取得予定のRGBAです。
				auto rgba = RGBA {};

				// 文字列の長さで分岐します。
				switch (str.length())
				{
				// rrggbb形式の場合は
				case 6:
					{
						// 文字列をRGBAに変換します。
						rgba.value = wcstoul(str.data(), nullptr, 16) << 8;
						rgba.a = 0xff;

						break;
					}
				// rrggbbaa形式の場合は
				case 8:
					{
						// 文字列をRGBAに変換します。
						rgba.value = wcstoul(str.data(), nullptr, 16);

						break;
					}
				// それ以外の場合は
				default:
					{
						// 何もしません。
						continue;
					}
				}

				// RGBAをカスタムカラーに追加します。
				entry.colors[i] = { rgba };
			}

			// カスタムカラーをコレクションに追加します。
			entries[{ section, key }] = entry;
		}

		//
		// カスタムカラーファイルを読み込みます。
		//
		BOOL read_custom_color_file(const std::wstring& custom_color_path)
		{
			MY_TRACE_FUNC("{/}", custom_color_path);

			// 既存のエントリを消去します。
			entries.clear();

			// 現在のセクションです。
			auto section = std::wstring {};

			// ファイルストリームを開きます。UTF-8です。
			std::ifstream stream(custom_color_path);

			// 一行ずつ読み込みます。
			auto utf8_line = std::string {};
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
					// セクションを更新します。
					section = line.substr(1, line.length() - 2);

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
				add(section, key, value);
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
				std::filesystem::path paths[] = {
					plugin_folder_path / L"al2/assets/custom_color.conf",
					plugin_folder_path / L"al2/config/custom_color.conf",
				};

				// カスタムカラーファイルのパスの配列を走査します。
				for (auto path : paths)
				{
					// カスタムカラーファイルを読み込みます。
					read_custom_color_file(path);

					// ハイブにセットします。
					hive.jd.custom_color_file_name = path;
				}
			}
			// 例外が発生した場合は
			catch (const std::exception& error)
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
