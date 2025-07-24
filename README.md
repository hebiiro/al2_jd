# 🎉ダークモード化プラグイン

* aviutl2のダークモード化されていない部分を補完することを目的としたプラグインです。
* 現時点では「入力プラグイン」形式になっています。
* `jd`は`just dark`の略です。

## 🚧注意事項

* まだ作りかけなので、以下の部分しかダークモード化できません。
	* メニュー (aviutl2ウィンドウのメニューバーとその他のポップアップメニュー)
	* スクロールバー (すべてのwin32ウィンドウのスクロールバー)
	* ツールチップ (aviutl2ウィンドウやエクスプローラなどでマウスホバーすると出てくるウィンドウ)
	* エクスプローラ (aviutl2のファイル管理ウィンドウ)

## 🚀インストール

* `C:\ProgramData\aviutl2\Plugin`フォルダに以下のファイルを入れてください。
	* `al2_jd.aui2`
	* (上記ファイルはzipファイル内の`Plugin`フォルダに入っています)

## 🔥アンインストール

* `C:\ProgramData\aviutl2\Plugin`フォルダから以下のファイルを削除してください。
	* `al2_jd.aui2`

## 🔧設定方法

* ✏️現時点では設定は変更できません。

## 📝スタイルファイル

* このプラグインは以下の流れでスタイルファイルを読み込みます。
	1. まず、`C:\ProgramData\aviutl2\style.conf`を読み込みます。
	1. 読み込めなかった場合は、代わりに`aviutl2フォルダ\style.conf`を読み込みます。

## 🔖更新履歴

* 🔖r1 🗓️2025.07.24
	* 🎉初版

## ⚗️動作確認

* AviUtl ExEdit2 beta3 https://spring-fragrance.mints.ne.jp/aviutl/

## 💳クレジット

* AviUtl ExEdit2 Plugin SDK https://spring-fragrance.mints.ne.jp/aviutl/
* Microsoft Research Detours Package https://github.com/microsoft/Detours
* JSON for Modern C++ https://github.com/nlohmann/json

## 👽️作成者情報
 
* 作成者 - 蛇色 (へびいろ)
* GitHub - https://github.com/hebiiro
* X - https://x.com/io_hebiiro

## 🚨免責事項

この作成物および同梱物を使用したことによって生じたすべての障害・損害・不具合等に関しては、私と私の関係者および私の所属するいかなる団体・組織とも、一切の責任を負いません。各自の責任においてご使用ください。
