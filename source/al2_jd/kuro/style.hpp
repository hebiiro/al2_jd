#pragma once

namespace apn::dark::kuro
{
	//
	// このクラスはaviutl2のstyle.confの変数を保持します。
	//
	inline struct Style
	{
		//
		// 配色のインデックスです。
		//
		enum class Color : size_t {
			Background,
			WindowBorder,
			WindowSeparator,
			Footer,
			FooterProgress,
			Grouping,
			GroupingHover,
			GroupingSelect,
			TitleHeader,
			BorderSelect,
			Border,
			BorderFocus,
			Text,
			TextDisable,
			TextSelect,
			ButtonBody,
			ButtonBodyHover,
			ButtonBodyPress,
			ButtonBodyDisable,
			ButtonBodySelect,
			SliderCursor,
			TrackBarRange,
			ZoomGauge,
			ZoomGaugeHover,
			ZoomGaugeOff,
			ZoomGaugeOffHover,
			FrameCursor,
			FrameCursorWide,
			PlayerCursor,
			GuideLine,
			Layer,
			LayerHeader,
			LayerHover,
			LayerDisable,
			LayerRange,
			LayerRangeFrame,
			ObjectVideo,
			ObjectVideoSelect,
			ObjectAudio,
			ObjectAudioSelect,
			ObjectControl,
			ObjectControlSelect,
			ObjectVideoFilter,
			ObjectVideoFilterSelect,
			ObjectAudioFilter,
			ObjectAudioFilterSelect,
			ObjectHover,
			ObjectFocus,
			ObjectSection,
			ClippingObject,
			ClippingObjectMask,
			Anchor,
			AnchorLine,
			AnchorIn,
			AnchorOut,
			AnchorHover,
			AnchorSelect,
			AnchorEdge,
			CenterGroup,
			HandleX,
			HandleY,
			HandleZ,
			HandleXHover,
			HandleYHover,
			HandleZHover,
			OutsideDisplay,
			MaxSize,
		};

		//
		// 配色インデックスのマップです。
		// キーは配色名です。
		//
		const std::unordered_map<std::wstring, Color> index_map = {
			{ L"Background", Color::Background },
			{ L"WindowBorder", Color::WindowBorder },
			{ L"WindowSeparator", Color::WindowSeparator },
			{ L"Footer", Color::Footer },
			{ L"FooterProgress", Color::FooterProgress },
			{ L"Grouping", Color::Grouping },
			{ L"GroupingHover", Color::GroupingHover },
			{ L"GroupingSelect", Color::GroupingSelect },
			{ L"TitleHeader", Color::TitleHeader },
			{ L"BorderSelect", Color::BorderSelect },
			{ L"Border", Color::Border },
			{ L"BorderFocus", Color::BorderFocus },
			{ L"Text", Color::Text },
			{ L"TextDisable", Color::TextDisable },
			{ L"TextSelect", Color::TextSelect },
			{ L"ButtonBody", Color::ButtonBody },
			{ L"ButtonBodyHover", Color::ButtonBodyHover },
			{ L"ButtonBodyPress", Color::ButtonBodyPress },
			{ L"ButtonBodyDisable", Color::ButtonBodyDisable },
			{ L"ButtonBodySelect", Color::ButtonBodySelect },
			{ L"SliderCursor", Color::SliderCursor },
			{ L"TrackBarRange", Color::TrackBarRange },
			{ L"ZoomGauge", Color::ZoomGauge },
			{ L"ZoomGaugeHover", Color::ZoomGaugeHover },
			{ L"ZoomGaugeOff", Color::ZoomGaugeOff },
			{ L"ZoomGaugeOffHover", Color::ZoomGaugeOffHover },
			{ L"FrameCursor", Color::FrameCursor },
			{ L"FrameCursorWide", Color::FrameCursorWide },
			{ L"PlayerCursor", Color::PlayerCursor },
			{ L"GuideLine", Color::GuideLine },
			{ L"Layer", Color::Layer },
			{ L"LayerHeader", Color::LayerHeader },
			{ L"LayerHover", Color::LayerHover },
			{ L"LayerDisable", Color::LayerDisable },
			{ L"LayerRange", Color::LayerRange },
			{ L"LayerRangeFrame", Color::LayerRangeFrame },
			{ L"ObjectVideo", Color::ObjectVideo },
			{ L"ObjectVideoSelect", Color::ObjectVideoSelect },
			{ L"ObjectAudio", Color::ObjectAudio },
			{ L"ObjectAudioSelect", Color::ObjectAudioSelect },
			{ L"ObjectControl", Color::ObjectControl },
			{ L"ObjectControlSelect", Color::ObjectControlSelect },
			{ L"ObjectVideoFilter", Color::ObjectVideoFilter },
			{ L"ObjectVideoFilterSelect", Color::ObjectVideoFilterSelect },
			{ L"ObjectAudioFilter", Color::ObjectAudioFilter },
			{ L"ObjectAudioFilterSelect", Color::ObjectAudioFilterSelect },
			{ L"ObjectHover", Color::ObjectHover },
			{ L"ObjectFocus", Color::ObjectFocus },
			{ L"ObjectSection", Color::ObjectSection },
			{ L"ClippingObject", Color::ClippingObject },
			{ L"ClippingObjectMask", Color::ClippingObjectMask },
			{ L"Anchor", Color::Anchor },
			{ L"AnchorLine", Color::AnchorLine },
			{ L"AnchorIn", Color::AnchorIn },
			{ L"AnchorOut", Color::AnchorOut },
			{ L"AnchorHover", Color::AnchorHover },
			{ L"AnchorSelect", Color::AnchorSelect },
			{ L"AnchorEdge", Color::AnchorEdge },
			{ L"CenterGroup", Color::CenterGroup },
			{ L"HandleX", Color::HandleX },
			{ L"HandleY", Color::HandleY },
			{ L"HandleZ", Color::HandleZ },
			{ L"HandleXHover", Color::HandleXHover },
			{ L"HandleYHover", Color::HandleYHover },
			{ L"HandleZHover", Color::HandleZHover },
			{ L"OutsideDisplay", Color::OutsideDisplay },
		};

		//
		// カラーエントリの配列です。
		//
		ColorEntry color_entries[(size_t)Color::MaxSize] = {};

		//
		// カラーエントリを返します。
		//
		const ColorEntry& get_color_entry(Color color_index) const
		{
			return color_entries[(size_t)color_index];
		}

		//
		// 値をカラーパーツに変換してコレクションに追加します。
		//
		void add_color_parts(const std::wstring& section, const std::wstring& key, const std::wstring& value)
		{
			// キーに対応するイテレータを取得します。
			auto it = index_map.find(key);
			if (it == index_map.end()) return;

			// 配色インデックスを取得します。
			auto index = (size_t)it->second;
			if (index >= std::size(color_entries)) return;

			// 値を配列に分割します。
			auto vec = split(value, L',');

			// 最大数を取得します。
			auto c = std::min(ColorEntry::c_max_size, vec.size());

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

				// カラーパーツをセットします。
				color_entries[index].parts[i] = { rgba };
			}
		}

		//
		// スタイルファイルを読み込みます。
		//
		BOOL read_file(const std::wstring& ini_file_name)
		{
			MY_TRACE_FUNC("{/}", ini_file_name);

			// 現在のセクションです。
			auto section = std::wstring {};

			// ファイルストリームを開きます。UTF-8です。
			std::ifstream stream(ini_file_name);

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
				add_color_parts(section, key, value);
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
				// メインモジュールがあるフォルダのパスを取得します。
				auto module_folder_name = my::get_module_file_name(nullptr).parent_path();

				// aviutl2/dataフォルダ内のスタイルファイルのパスを取得します。
				hive.jd.style_file_name = module_folder_name / L"data" / L"style.conf";

				// スタイルファイルが存在しない場合は
				if (!std::filesystem::exists(hive.jd.style_file_name))
				{
					// プログラムデータ内のスタイルファイルのパスを取得します。
					hive.jd.style_file_name = std::filesystem::path(LR"***(C:\ProgramData\aviutl2\style.conf)***");
				}

				// スタイルファイルが存在しない場合は
				if (!std::filesystem::exists(hive.jd.style_file_name))
				{
					// aviutl2フォルダ内のスタイルファイルのパスを取得します。
					hive.jd.style_file_name = module_folder_name / L"style.conf";
				}

				// スタイルファイルを読み込みます。
				read_file(hive.jd.style_file_name);
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
	} style;
}
