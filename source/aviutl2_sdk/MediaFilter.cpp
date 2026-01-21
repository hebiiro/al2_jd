//----------------------------------------------------------------------------------
//	サンプルフィルタプラグイン(フィルタ効果) for AviUtl ExEdit2
//----------------------------------------------------------------------------------
#include <windows.h>
#include <memory>
#include <algorithm>

#include "filter2.h"

bool func_proc_video(FILTER_PROC_VIDEO* video);
bool func_proc_audio(FILTER_PROC_AUDIO* audio);

//---------------------------------------------------------------------
//	フィルタ設定項目定義
//---------------------------------------------------------------------
auto luminance = FILTER_ITEM_TRACK(L"明るさ", 1.0, 0.0, 2.0, 0.01);
FILTER_ITEM_SELECT::ITEM component_list[] = { { L"R成分のみ", 1 }, { L"G成分のみ", 2 }, { L"B成分のみ", 4 }, { L"RGB成分", 7 }, { nullptr } };
auto component = FILTER_ITEM_SELECT(L"対象", 7, component_list);
auto volume = FILTER_ITEM_TRACK(L"音量", 1.0, 0.0, 2.0, 0.01);
auto mono = FILTER_ITEM_CHECK(L"モノラル化", false);
void* items[] = { &luminance, &component, &volume, &mono, nullptr };

//---------------------------------------------------------------------
//	フィルタプラグイン構造体定義
//---------------------------------------------------------------------
FILTER_PLUGIN_TABLE filter_plugin_table = {
	FILTER_PLUGIN_TABLE::FLAG_VIDEO | FILTER_PLUGIN_TABLE::FLAG_AUDIO, //	フラグ
	L"メディアフィルタ(sample)",					// プラグインの名前
	L"サンプル",									// ラベルの初期値 (nullptrならデフォルトのラベルになります)
	L"Sample MediaFilter version 2.00 By ＫＥＮくん",	// プラグインの情報
	items,											// 設定項目の定義 (FILTER_ITEM_XXXポインタを列挙してnull終端したリストへのポインタ)
	func_proc_video,								// 画像フィルタ処理関数へのポインタ (FLAG_VIDEOが有効の時のみ呼ばれます)
	func_proc_audio									// 音声フィルタ処理関数へのポインタ (FLAG_AUDIOが有効の時のみ呼ばれます)
};

//---------------------------------------------------------------------
//	プラグインDLL初期化関数 (未定義なら呼ばれません)
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) bool InitializePlugin(DWORD version) { // versionは本体のバージョン番号
	return true;
}

//---------------------------------------------------------------------
//	プラグインDLL解放関数 (未定義なら呼ばれません)
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) void UninitializePlugin() {
}

//---------------------------------------------------------------------
//	フィルタ構造体のポインタを渡す関数
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) FILTER_PLUGIN_TABLE* GetFilterPluginTable(void) {
	return &filter_plugin_table;
}

//---------------------------------------------------------------------
//	画像フィルタ処理
//---------------------------------------------------------------------
bool func_proc_video(FILTER_PROC_VIDEO* video) {
	auto w = video->object->width;
	auto h = video->object->height;
	auto buffer = std::make_unique<PIXEL_RGBA[]>(w * h);
	video->get_image_data(buffer.get());

	// 指定のRGB成分の明るさを調整
	auto r = (component.value & 1) ? luminance.value : 1.0;
	auto g = (component.value & 2) ? luminance.value : 1.0;
	auto b = (component.value & 4) ? luminance.value : 1.0;
	auto p = buffer.get();
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			p->r = (unsigned char)std::clamp(p->r * r, 0.0, 255.0);
			p->g = (unsigned char)std::clamp(p->g * g, 0.0, 255.0);
			p->b = (unsigned char)std::clamp(p->b * b, 0.0, 255.0);
			p++;
		}
	}

	video->set_image_data(buffer.get(), w, h);
	return true;
}

//---------------------------------------------------------------------
//	音声フィルタ処理
//---------------------------------------------------------------------
bool func_proc_audio(FILTER_PROC_AUDIO* audio) {
	auto num = audio->object->sample_num;
	auto buffer0 = std::make_unique<float[]>(num);
	auto buffer1 = std::make_unique<float[]>(num);
	audio->get_sample_data(buffer0.get(), 0);
	audio->get_sample_data(buffer1.get(), 1);

	// 音量を調整
	auto v = (float)volume.value;
	auto p0 = buffer0.get();
	auto p1 = buffer1.get();
	for (int i = 0; i < num; i++) {
		*p0++ *= v;
		*p1++ *= v;
	}

	// モノラル化
	p0 = buffer0.get();
	p1 = buffer1.get();
	if (mono.value) {
		for (int i = 0; i < num; i++) {
			*p0++ += *p1++;
		}
		audio->set_sample_data(buffer0.get(), 0);
		audio->set_sample_data(buffer0.get(), 1);
	} else {
		audio->set_sample_data(buffer0.get(), 0);
		audio->set_sample_data(buffer1.get(), 1);
	}
	return true;
}
