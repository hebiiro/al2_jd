//----------------------------------------------------------------------------------
//	汎用プラグイン ヘッダーファイル for AviUtl ExEdit2
//	By ＫＥＮくん
//----------------------------------------------------------------------------------
struct INPUT_PLUGIN_TABLE;
struct OUTPUT_PLUGIN_TABLE;
struct FILTER_PLUGIN_TABLE;
struct SCRIPT_MODULE_TABLE;

// オブジェクトハンドル
typedef void* OBJECT_HANDLE;

// レイヤー・フレーム情報構造体
// フレーム番号、レイヤー番号が0からの番号になります ※UI表示と異なります
struct OBJECT_LAYER_FRAME {
	int layer;	// レイヤー番号
	int start;	// 開始フレーム番号
	int end;	// 終了フレーム番号
};

// 冗長なので後で廃止します
struct DEPRECATED_OBJECT_FRAME_INFO {
	int start;
	int end;
};

//----------------------------------------------------------------------------------

// 編集情報構造体
// フレーム番号、レイヤー番号が0からの番号になります ※UI表示と異なります
struct EDIT_INFO {
	int width, height;	// シーンの解像度
	int rate, scale;	// シーンのフレームレート
	int sample_rate;	// シーンのサンプリングレート
	int frame;			// 現在のカーソルのフレーム番号
	int layer;			// 現在のレイヤーの表示開始番号
	int frame_max;		// オブジェクトが存在する最大のフレーム番号
	int layer_max;		// オブジェクトが存在する最大のレイヤー番号
};

// 編集セクション構造体
// メニュー選択やプロジェクト編集のコールバック関数内で利用出来ます
// フレーム番号、レイヤー番号が0からの番号になります ※UI表示と異なります
struct EDIT_SECTION {
	// 編集情報
	EDIT_INFO* info;

	// 指定の位置にオブジェクトエイリアスを作成します
	// alias	: オブジェクトエイリアスデータ(UTF-8)へのポインタ
	//			  オブジェクトエイリアスファイルと同じフォーマットになります
	// layer	: 作成するレイヤー番号
	// frame	: 作成するフレーム番号
	// length	: オブジェクトのフレーム数 ※エイリアスデータにフレーム情報が無い場合に利用します
	// 戻り値	: 作成したオブジェクトのハンドル (失敗した場合はnullptrを返却)
	//			  既に存在するオブジェクトに重なったり、エイリアスデータが不正な場合に失敗します
	OBJECT_HANDLE (*create_object_from_alias)(LPCSTR alias, int layer, int frame, int length);

	// 指定のフレーム番号以降にあるオブジェクトを検索します
	// layer	: 検索対象のレイヤー番号
	// frame	: 検索を開始するフレーム番号
	// 戻り値	: 検索したオブジェクトのハンドル (見つからない場合はnullptrを返却)
	OBJECT_HANDLE (*find_object)(int layer, int frame);

	// 冗長なので後で廃止します
	DEPRECATED_OBJECT_FRAME_INFO (*deprecated_get_object_frame_info)(OBJECT_HANDLE object);

	// オブジェクトのレイヤー・フレーム情報を取得します
	// object	: オブジェクトのハンドル
	// 戻り値	: オブジェクトのレイヤー・フレーム情報
	OBJECT_LAYER_FRAME (*get_object_layer_frame)(OBJECT_HANDLE object);

	// オブジェクトのエイリアスデータを取得します
	// object	: オブジェクトのハンドル
	// 戻り値	: オブジェクトエイリアスデータ(UTF-8)へのポインタ (取得出来ない場合はnullptrを返却)
	// 			  オブジェクトエイリアスファイルと同じフォーマットになります
	//			  ※次に文字列返却の関数を使うかコールバック処理の終了まで有効
	LPCSTR (*get_object_alias)(OBJECT_HANDLE object);

	// オブジェクトの設定項目の値を文字列で取得します
	// object	: オブジェクトのハンドル
	// effect	: 対象のエフェクト名 (エイリアスファイルのeffect.nameの値)
	//			  同じエフェクトが複数ある場合は":n"のサフィックスでインデックス指定出来ます (nは0からの番号)
	//			  get_object_item_value(object, L"ぼかし:1", L"範囲"); // 2個目のぼかしを対象とする
	// item		: 対象の設定項目の名称 (エイリアスファイルのキーの名称)
	// 戻り値	: 取得した設定値(UTF8)へのポインタ (取得出来ない場合はnullptrを返却)
	//			  エイリアスファイルの設定値と同じフォーマットになります
	//			  ※次に文字列返却の関数を使うかコールバック処理の終了まで有効
	LPCSTR (*get_object_item_value)(OBJECT_HANDLE object, LPCWSTR effect, LPCWSTR item);

	// オブジェクトの設定項目の値を文字列で設定します
	// object	: オブジェクトのハンドル
	// effect	: 対象のエフェクト名 (エイリアスファイルのeffect.nameの値)
	//			  同じエフェクトが複数ある場合は":n"のサフィックスでインデックス指定出来ます (nは0からの番号)
	//			  set_object_item_value(object, L"ぼかし:1", L"範囲", "1"); // 2個目のぼかしを対象とする
	// item		: 対象の設定項目の名称 (エイリアスファイルのキーの名称)
	// value	: 設定値(UTF8)
	//			  エイリアスファイルの設定値と同じフォーマットになります
	// 戻り値	: 設定出来た場合はtrue (対象が見つからない場合は失敗します)
	bool (*set_object_item_value)(OBJECT_HANDLE object, LPCWSTR effect, LPCWSTR item, LPCSTR value);

	// オブジェクトを移動します
	// object	: オブジェクトのハンドル
	// layer	: 移動先のレイヤー番号
	// frame	: 移動先のフレーム番号
	// 戻り値	: 移動した場合はtrue (移動先にオブジェクトが存在する場合は失敗します)
	bool (*move_object)(OBJECT_HANDLE object, int layer, int frame);

	// オブジェクトを削除します
	// object	: オブジェクトのハンドル
	void (*delete_object)(OBJECT_HANDLE object);

	// オブジェクト設定ウィンドウで選択されているオブジェクトのハンドルを取得します
	// 戻り値	: オブジェクトのハンドル (未選択の場合はnullptrを返却)　
	OBJECT_HANDLE(*get_focus_object)();

	// オブジェクト設定ウィンドウで選択するオブジェクトを設定します (コールバック処理の終了時に設定されます)
	// object	: オブジェクトのハンドル
	void (*set_focus_object)(OBJECT_HANDLE object);

	// アプリケーションのログを出力します
	// message	: ログメッセージ
	void (*output_log)(LPCWSTR message);

};

// 編集ハンドル構造体
struct EDIT_HANDLE {
	// プロジェクトデータの編集をする為のコールバック関数(func_proc_edit)を呼び出します
	// 編集情報を排他制御する為にコールバック関数内で編集処理をする形になります
	// コールバック関数内で編集したオブジェクトは纏めてUndoに登録されます
	// コールバック関数はメインスレッドから呼ばれます
	// func_proc_edit	: 編集処理のコールバック関数
	// 戻り値			: trueなら成功
	//					  編集が出来ない場合(出力中等)に失敗します
	bool (*call_edit_section)(void (*func_proc_edit)(EDIT_SECTION* edit));

};

//----------------------------------------------------------------------------------

// プロジェクトファイル構造体
// プロジェクトファイルのロード、セーブ時のコールバック関数内で利用出来ます
// プロジェクトの保存データはプラグイン毎のデータ領域になります
struct PROJECT_FILE {
	// プロジェクトに保存されている文字列(UTF-8)を取得します
	// key		: キー名(UTF-8)
	// 戻り値	: 取得した文字列へのポインタ (未設定の場合はnullptr)
	LPCSTR (*get_param_string)(LPCSTR key);

	// プロジェクトに文字列(UTF-8)を保存します
	// key		: キー名(UTF-8)
	// value	: 保存する文字列(UTF-8)
	void (*set_param_string)(LPCSTR key, LPCSTR value);

	// プロジェクトに保存されているバイナリデータを取得します
	// key		: キー名(UTF-8)
	// data		: 取得するデータの格納先へのポインタ
	// size		: 取得するデータのサイズ (保存されているサイズと異なる場合は失敗します)
	// 戻り値	: 正しく取得出来た場合はtrue
	bool (*get_param_binary)(LPCSTR key, void* data, int size);

	// プロジェクトにバイナリデータを保存します
	// key		: キー名(UTF-8)
	// data		: 保存するデータへのポインタ
	// size		: 保存するデータのサイズ (4096バイト以下)
	void (*set_param_binary)(LPCSTR key, void* data, int size);

	// プロジェクトに保存されているデータを全て削除します
	void (*clear_params)();

};

//----------------------------------------------------------------------------------

// ホストアプリケーション構造体
struct HOST_APP_TABLE {
	// プラグインの情報を設定する
	// information	: プラグインの情報
	void (*set_plugin_information)(LPCWSTR information);

	// 入力プラグインを登録する
	// input_plugin_table	: 入力プラグイン構造体
	void (*register_input_plugin)(INPUT_PLUGIN_TABLE* input_plugin_table);

	// 出力プラグインを登録する
	// output_plugin_table	: 出力プラグイン構造体
	void (*register_output_plugin)(OUTPUT_PLUGIN_TABLE* output_plugin_table);

	// フィルタプラグインを登録する
	// filter_plugin_table	: フィルタプラグイン構造体
	void (*register_filter_plugin)(FILTER_PLUGIN_TABLE* filter_plugin_table);

	// スクリプトモジュールを登録する
	// script_module_table	: スクリプトモジュール構造体
	void (*register_script_module)(SCRIPT_MODULE_TABLE* script_module_table);

	// インポートメニューを登録する
	// name				: インポートメニューの名称
	// func_proc_import	: インポートメニュー選択時のコールバック関数
	void (*register_import_menu)(LPCWSTR name, void (*func_proc_import)(EDIT_SECTION* edit));

	// エクスポートメニューを登録する
	// name				: エクスポートメニューの名称
	// func_proc_export	: エクスポートメニュー選択時のコールバック関数
	void (*register_export_menu)(LPCWSTR name, void (*func_proc_export)(EDIT_SECTION* edit));

	// ウィンドウクライアントを登録する
	// name		: ウィンドウの名称
	// hwnd		: ウィンドウハンドル
	// ウィンドウにはWS_CHILDが追加され親ウィンドウが設定されます ※WS_POPUPは削除されます
	void (*register_window_client)(LPCWSTR name, HWND hwnd);

	// プロジェクトデータ編集用のハンドルを取得します
	// 戻り値	: 編集ハンドル
	EDIT_HANDLE* (*create_edit_handle)();

	// プロジェクトファイルをロードした直後に呼ばれる関数を登録する ※プロジェクトの初期化時にも呼ばれます
	// func_project_load	: プロジェクトファイルのロード時のコールバック関数
	void (*register_project_load_handler)(void (*func_project_load)(PROJECT_FILE* project));

	// プロジェクトファイルをセーブする直前に呼ばれる関数を登録する
	// func_project_save	: プロジェクトファイルのセーブ時のコールバック関数
	void (*register_project_save_handler)(void (*func_project_save)(PROJECT_FILE* project));

};
