//----------------------------------------------------------------------------------
//	フィルタプラグイン ヘッダーファイル for AviUtl ExEdit2
//	By ＫＥＮくん
//----------------------------------------------------------------------------------

// トラックバー項目構造体
struct FILTER_ITEM_TRACK {
	FILTER_ITEM_TRACK(LPCWSTR name, double value, double s, double e, double step = 1.0) : name(name), value(value), s(s), e(e), step(step) {}
	LPCWSTR type = L"track";	// 設定の種別
	LPCWSTR name;				// 設定名
	double value;				// 設定値 (フィルタ処理の呼び出し時に現在の値に更新されます)
	const double s, e;			// 設定値の最小、最大
	const double step;			// 設定値の単位( 1.0 / 0.1 / 0.01 / 0.001 )
};

// チェックボックス項目構造体
struct FILTER_ITEM_CHECK {
	FILTER_ITEM_CHECK(LPCWSTR name, bool value) : name(name), value(value) {}
	LPCWSTR type = L"check";	// 設定の種別
	LPCWSTR name;				// 設定名
	bool value;					// 設定値 (フィルタ処理の呼び出し時に現在の値に更新されます)
};

// 色選択項目構造体
struct FILTER_ITEM_COLOR {
	union VALUE { // 設定値の色
		int code;
		struct { unsigned char b, g, r, x; };
	};
	FILTER_ITEM_COLOR(LPCWSTR name, int code) : name(name) { value.code = code; }
	LPCWSTR type = L"color";	// 設定の種別
	LPCWSTR name;				// 設定名
	VALUE value;				// 設定値 (フィルタ処理の呼び出し時に現在の値に更新されます)
};

// 選択リスト項目構造体
struct FILTER_ITEM_SELECT {
	struct ITEM {		// 選択肢項目
		LPCWSTR name;	// 選択肢の名前
		int value;		// 選択肢の値
	};
	FILTER_ITEM_SELECT(LPCWSTR name, int value, ITEM* list) : name(name), value(value), list(list) {}
	LPCWSTR type = L"select";	// 設定の種別
	LPCWSTR name;				// 設定名
	int value;					// 設定値 (フィルタ処理の呼び出し時に現在の値に更新されます)
	ITEM* list;					// 選択肢リスト (ITEMを列挙して名前がnullのITEMで終端したリストへのポインタ)
};

// ファイル選択項目構造体
struct FILTER_ITEM_FILE {
	FILTER_ITEM_FILE(LPCWSTR name, LPCWSTR value, LPCWSTR filefilter) : name(name), value(value), filefilter(filefilter) {}
	LPCWSTR type = L"file";		// 設定の種別
	LPCWSTR name;				// 設定名
	LPCWSTR value;				// 設定値 (フィルタ処理の呼び出し時に現在の値に更新されます)
	LPCWSTR filefilter;			// ファイルフィルタ
};

//----------------------------------------------------------------------------------

// RGBA32bit構造体
struct PIXEL_RGBA {
	unsigned char r, g, b, a;
};

// シーン情報構造体
struct SCENE_INFO {
	int width, height;		// シーンの解像度
	int rate, scale;		// シーンのフレームレート
	int sample_rate;		// シーンのサンプリングレート
};

// オブジェクト情報構造体
struct OBJECT_INFO {
	int64_t id;				// オブジェクトのID (アプリ起動毎の固有ID)
	int frame;				// オブジェクトの現在のフレーム番号
	int frame_total;		// オブジェクトの総フレーム数
	double time;			// オブジェクトの現在の時間(秒)
	double time_total;		// オブジェクトの総時間(秒)
	int width, height;		// オブジェクトの現在の画像サイズ (画像フィルタのみ)
	int64_t sample_index;	// オブジェクトの現在の音声サンプル位置 (音声フィルタのみ)
	int64_t sample_total;	// オブジェクトの総サンプル数 (音声フィルタのみ)
	int sample_num;			// オブジェクトの現在の音声サンプル数 (音声フィルタのみ)
	int channel_num;		// オブジェクトの現在の音声チャンネル数 (音声フィルタのみ) ※通常2になります
	int64_t effect_id;		// オブジェクトの内の対象エフェクトのID (アプリ起動毎の固有ID)
							// ※処理対象のフィルタ効果、オブジェクト入出力の固有ID
};

//----------------------------------------------------------------------------------

// 画像フィルタ処理用構造体
struct FILTER_PROC_VIDEO {
	// シーン情報
	const SCENE_INFO* scene;

	// オブジェクト情報
	const OBJECT_INFO* object;

	// 現在の画像のデータを取得する (VRAMからデータを取得します) 
	// buffer		: 画像データの格納先へのポインタ
	void (*get_image_data)(PIXEL_RGBA* buffer);

	// 現在の画像のデータを設定します (VRAMへデータを書き込みます) 
	// buffer		: 画像データへのポインタ
	// width,height	: 画像サイズ
	void (*set_image_data)(PIXEL_RGBA* buffer, int width, int height);

};

// 音声フィルタ処理用構造体
struct FILTER_PROC_AUDIO {
	// シーン情報
	const SCENE_INFO* scene;

	// オブジェクト情報
	const OBJECT_INFO* object;

	// 現在の音声のデータを取得する
	// buffer		: 音声データの格納先へのポインタ ※音声データはPCM(float)32bit
	// channel		: 音声データのチャンネル ( 0 = 左チャンネル / 1 = 右チャンネル )
	void (*get_sample_data)(float* buffer, int channel);

	// 現在の音声のデータを設定する
	// buffer		: 音声データへのポインタ ※音声データはPCM(float)32bit
	// channel		: 音声データのチャンネル ( 0 = 左チャンネル / 1 = 右チャンネル )
	void (*set_sample_data)(float* buffer, int channel);

};

//----------------------------------------------------------------------------------

// フィルタプラグイン構造体
struct FILTER_PLUGIN_TABLE {
	int flag;								// フラグ
	static constexpr int FLAG_VIDEO = 1;	// 画像フィルタをサポートする
	static constexpr int FLAG_AUDIO = 2;	// 音声フィルタをサポートする
											// 画像と音声のフィルタ処理は別々のスレッドで処理されます
	static constexpr int FLAG_INPUT = 4;	// オブジェクトの初期入力をする (メディアオブジェクトにする場合)
	LPCWSTR name;				// プラグインの名前
	LPCWSTR label;				// ラベルの初期値 (nullptrならデフォルトのラベルになります)
	LPCWSTR information;		// プラグインの情報
	void** items;				// 設定項目の定義 (FILTER_ITEM_XXXポインタを列挙してnull終端したリストへのポインタ)

	// 画像フィルタ処理関数へのポインタ (FLAG_VIDEOが有効の時のみ呼ばれます)
	bool (*func_proc_video)(FILTER_PROC_VIDEO* video);

	// 音声フィルタ処理関数へのポインタ (FLAG_AUDIOが有効の時のみ呼ばれます)
	bool (*func_proc_audio)(FILTER_PROC_AUDIO* audio);

};
