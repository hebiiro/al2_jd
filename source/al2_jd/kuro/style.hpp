#pragma once

namespace apn::dark::kuro
{
	//
	// このクラスはaviutl2のstyle.confの変数を保持します。
	//
	inline struct style_t
	{
		//
		// 配色のインデックスです。
		//
		enum class color_e : size_t {
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
		const std::unordered_map<std::wstring, color_e> index_map = {
			{ L"Background", color_e::Background },
			{ L"WindowBorder", color_e::WindowBorder },
			{ L"WindowSeparator", color_e::WindowSeparator },
			{ L"Footer", color_e::Footer },
			{ L"FooterProgress", color_e::FooterProgress },
			{ L"Grouping", color_e::Grouping },
			{ L"GroupingHover", color_e::GroupingHover },
			{ L"GroupingSelect", color_e::GroupingSelect },
			{ L"TitleHeader", color_e::TitleHeader },
			{ L"BorderSelect", color_e::BorderSelect },
			{ L"Border", color_e::Border },
			{ L"BorderFocus", color_e::BorderFocus },
			{ L"Text", color_e::Text },
			{ L"TextDisable", color_e::TextDisable },
			{ L"TextSelect", color_e::TextSelect },
			{ L"ButtonBody", color_e::ButtonBody },
			{ L"ButtonBodyHover", color_e::ButtonBodyHover },
			{ L"ButtonBodyPress", color_e::ButtonBodyPress },
			{ L"ButtonBodyDisable", color_e::ButtonBodyDisable },
			{ L"ButtonBodySelect", color_e::ButtonBodySelect },
			{ L"SliderCursor", color_e::SliderCursor },
			{ L"TrackBarRange", color_e::TrackBarRange },
			{ L"ZoomGauge", color_e::ZoomGauge },
			{ L"ZoomGaugeHover", color_e::ZoomGaugeHover },
			{ L"ZoomGaugeOff", color_e::ZoomGaugeOff },
			{ L"ZoomGaugeOffHover", color_e::ZoomGaugeOffHover },
			{ L"FrameCursor", color_e::FrameCursor },
			{ L"FrameCursorWide", color_e::FrameCursorWide },
			{ L"PlayerCursor", color_e::PlayerCursor },
			{ L"GuideLine", color_e::GuideLine },
			{ L"Layer", color_e::Layer },
			{ L"LayerHeader", color_e::LayerHeader },
			{ L"LayerHover", color_e::LayerHover },
			{ L"LayerDisable", color_e::LayerDisable },
			{ L"LayerRange", color_e::LayerRange },
			{ L"LayerRangeFrame", color_e::LayerRangeFrame },
			{ L"ObjectVideo", color_e::ObjectVideo },
			{ L"ObjectVideoSelect", color_e::ObjectVideoSelect },
			{ L"ObjectAudio", color_e::ObjectAudio },
			{ L"ObjectAudioSelect", color_e::ObjectAudioSelect },
			{ L"ObjectControl", color_e::ObjectControl },
			{ L"ObjectControlSelect", color_e::ObjectControlSelect },
			{ L"ObjectVideoFilter", color_e::ObjectVideoFilter },
			{ L"ObjectVideoFilterSelect", color_e::ObjectVideoFilterSelect },
			{ L"ObjectAudioFilter", color_e::ObjectAudioFilter },
			{ L"ObjectAudioFilterSelect", color_e::ObjectAudioFilterSelect },
			{ L"ObjectHover", color_e::ObjectHover },
			{ L"ObjectFocus", color_e::ObjectFocus },
			{ L"ObjectSection", color_e::ObjectSection },
			{ L"ClippingObject", color_e::ClippingObject },
			{ L"ClippingObjectMask", color_e::ClippingObjectMask },
			{ L"Anchor", color_e::Anchor },
			{ L"AnchorLine", color_e::AnchorLine },
			{ L"AnchorIn", color_e::AnchorIn },
			{ L"AnchorOut", color_e::AnchorOut },
			{ L"AnchorHover", color_e::AnchorHover },
			{ L"AnchorSelect", color_e::AnchorSelect },
			{ L"AnchorEdge", color_e::AnchorEdge },
			{ L"CenterGroup", color_e::CenterGroup },
			{ L"HandleX", color_e::HandleX },
			{ L"HandleY", color_e::HandleY },
			{ L"HandleZ", color_e::HandleZ },
			{ L"HandleXHover", color_e::HandleXHover },
			{ L"HandleYHover", color_e::HandleYHover },
			{ L"HandleZHover", color_e::HandleZHover },
			{ L"OutsideDisplay", color_e::OutsideDisplay },
		};

		//
		// カラーエントリの配列です。
		//
		color_entry_t color_entries[(size_t)color_e::MaxSize] = {};

		//
		// カラーエントリを返します。
		//
		const color_entry_t& get_color_entry(color_e color_index) const
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
			auto c = std::min(color_entry_t::c_max_size, vec.size());

			// 配列を走査します。
			for (size_t i = 0; i < c; i++)
			{
				// 文字列を取得します。
				const auto& str = vec[i];

				// 文字列が空の場合は何もしません。
				if (str.empty()) continue;

				// 取得予定のRGBAです。
				auto rgba = rgba_t {};

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
