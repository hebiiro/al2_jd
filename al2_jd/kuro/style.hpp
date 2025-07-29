#pragma once

namespace apn::dark::kuro
{
	//
	// このクラスは配色を保持します。
	//
	struct ColorValue
	{
		//
		// このクラスはRGBAを保持します。
		//
		struct RGBA { uint8_t r, g, b, a; };

		//
		// 配色の値です。複数の場合もあります。
		//
		RGBA values[2] = {};

		//
		// win32用の配色の値です。複数の場合もあります。
		//
		COLORREF win32_values[2] = {};

		//
		// 配色の名前です。
		//
		LPCWSTR name;

		//
		// コンストラクタです。
		//
		ColorValue(LPCWSTR name)
			: name(name)
		{
			for (auto& value : values)
				value = { 0xff, 0xff, 0xff, 0xff };
		}

		//
		// 配色を設定します。
		//
		void set(size_t i, const std::wstring& str)
		{
			switch (str.length())
			{
			case 6: // rrggbb形式の場合の処理です。
				{
					// 文字列を16進数に変換します。
					auto temp = wcstoul(str.data(), nullptr, 16);

					values[i].r = (uint8_t)((temp & 0x00FF0000) >> 16);
					values[i].g = (uint8_t)((temp & 0x0000FF00) >> 8);
					values[i].b = (uint8_t)((temp & 0x000000FF) >> 0);
					values[i].a = (uint8_t)(0xff);

					win32_values[i] = RGB(values[i].r, values[i].g, values[i].b);

					break;
				}
			case 8: // rrggbbaa形式の場合の処理です。
				{
					// 文字列を16進数に変換します。
					auto temp = wcstoul(str.data(), nullptr, 16);

					values[i].r = (uint8_t)((temp & 0xFF000000) >> 24);
					values[i].g = (uint8_t)((temp & 0x00FF0000) >> 16);
					values[i].b = (uint8_t)((temp & 0x0000FF00) >> 8);
					values[i].a = (uint8_t)((temp & 0x000000FF) >> 0);

					win32_values[i] = RGB(values[i].r, values[i].g, values[i].b);

					break;
				}
			}
		}
	};

	//
	// このクラスはaviutl2のstyle.confの変数を保持します。
	//
	inline struct Style
	{
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

		ColorValue colors[(size_t)Color::MaxSize] = {
			L"Background",
			L"WindowBorder",
			L"WindowSeparator",
			L"Footer",
			L"FooterProgress",
			L"Grouping",
			L"GroupingHover",
			L"GroupingSelect",
			L"TitleHeader",
			L"BorderSelect",
			L"Border",
			L"BorderFocus",
			L"Text",
			L"TextDisable",
			L"TextSelect",
			L"ButtonBody",
			L"ButtonBodyHover",
			L"ButtonBodyPress",
			L"ButtonBodyDisable",
			L"ButtonBodySelect",
			L"SliderCursor",
			L"TrackBarRange",
			L"ZoomGauge",
			L"ZoomGaugeHover",
			L"ZoomGaugeOff",
			L"ZoomGaugeOffHover",
			L"FrameCursor",
			L"FrameCursorWide",
			L"PlayerCursor",
			L"GuideLine",
			L"Layer",
			L"LayerHeader",
			L"LayerHover",
			L"LayerDisable",
			L"LayerRange",
			L"LayerRangeFrame",
			L"ObjectVideo",
			L"ObjectVideoSelect",
			L"ObjectAudio",
			L"ObjectAudioSelect",
			L"ObjectControl",
			L"ObjectControlSelect",
			L"ObjectVideoFilter",
			L"ObjectVideoFilterSelect",
			L"ObjectAudioFilter",
			L"ObjectAudioFilterSelect",
			L"ObjectHover",
			L"ObjectFocus",
			L"ObjectSection",
			L"ClippingObject",
			L"ClippingObjectMask",
			L"Anchor",
			L"AnchorLine",
			L"AnchorIn",
			L"AnchorOut",
			L"AnchorHover",
			L"AnchorSelect",
			L"AnchorEdge",
			L"CenterGroup",
			L"HandleX",
			L"HandleY",
			L"HandleZ",
			L"HandleXHover",
			L"HandleYHover",
			L"HandleZHover",
			L"OutsideDisplay",
		};

		//
		// 配色をwin32のCOLORREF形式で返します。
		//
		COLORREF get_COLORREF(Color color_index, size_t value_index = 0) const
		{
			return colors[(size_t)color_index].win32_values[value_index];
		}

		//
		// スタイルファイルのカラー項目を読み込みます。
		//
		BOOL read_color_style(LPCWSTR ini_file_name)
		{
			MY_TRACE_FUNC("{/}", ini_file_name);

			//
			// この関数はスタイルファイル内のカラー項目を文字列として返します。
			//
			const auto get_color_string = [&](LPCWSTR key_name)
			{
				std::wstring buffer(MAX_PATH, L'\0');
				::GetPrivateProfileStringW(L"Color", key_name,
					L"", buffer.data(), (DWORD)buffer.size(), ini_file_name);
				buffer.resize(wcslen(buffer.data()));
				return buffer;
			};

			//
			// この関数はスタイルファイル内のカラー項目を読み込みます。
			//
			const auto read_color = [&](ColorValue& color)
			{
				// カラー項目を文字列として取得します。
				auto str = get_color_string(color.name);

				// 文字列を配列に分割します。
				auto vec = split(str, L',');

				// 配列が取得できなかった場合は何もしません。
				if (vec.empty()) return;

				// 最大数を取得します。
				auto c = std::min(std::size(color.values), vec.size());

				// 配色をセットします。
				for (size_t i = 0; i < c; i++) color.set(i, vec[i]);
			};

			// スタイルファイル内のカラー項目を読み込みます。
			for (auto& color : colors) read_color(color);

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
				auto ini_file_name = module_folder_name / L"data" / L"style.conf";

				// スタイルファイルが存在しない場合は
				if (!std::filesystem::exists(ini_file_name))
				{
					// プログラムデータ内のスタイルファイルのパスを取得します。
					ini_file_name = std::filesystem::path(LR"***(C:\ProgramData\aviutl2\style.conf)***");
				}

				// スタイルファイルが存在しない場合は
				if (!std::filesystem::exists(ini_file_name))
				{
					// aviutl2フォルダ内のスタイルファイルのパスを取得します。
					ini_file_name = module_folder_name / L"style.conf";
				}

				// カラー項目を読み込みます。
				read_color_style(ini_file_name.c_str());
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
