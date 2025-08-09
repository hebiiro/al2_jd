#pragma once

namespace apn::dark::kuro::paint
{
	//
	// このクラスはメニューのマテリアルです。
	//
	inline struct MenuMaterial : Material
	{
		//
		// マテリアルの初期化処理を実行します。
		//
		virtual void on_init_material() override
		{
			MY_TRACE_FUNC("");

			auto background = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto border = Pigment {
				{ style.get_COLORREF(Style::Color::WindowBorder), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto gutter = Pigment {
				{ style.get_COLORREF(Style::Color::TitleHeader), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto separator = Pigment {
				{ style.get_COLORREF(Style::Color::TitleHeader), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto item_normal = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto item_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			auto item_hot = Pigment {
				{ style.get_COLORREF(Style::Color::ButtonBodySelect), },
//				{ style.get_COLORREF(Style::Color::BorderSelect), 1, },
				{ style.get_COLORREF(Style::Color::Border), 1, },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto mc_normal = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			};

			auto mc_disabled = Pigment {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			};

			palette.set(MENU_BARBACKGROUND, MB_ACTIVE, {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::Text), },
			});

			palette.set(MENU_BARBACKGROUND, MB_INACTIVE, {
				{ style.get_COLORREF(Style::Color::Background), },
				{ CLR_NONE },
				{ style.get_COLORREF(Style::Color::TextDisable), },
			});

			palette.set(MENU_BARITEM, MBI_NORMAL, item_normal);
			palette.set(MENU_BARITEM, MBI_DISABLED, item_disabled);
			palette.set(MENU_BARITEM, MBI_HOT, item_hot);
			palette.set(MENU_BARITEM, MBI_DISABLEDHOT, item_disabled);
			palette.set(MENU_BARITEM, MBI_PUSHED, item_hot);
			palette.set(MENU_BARITEM, MBI_DISABLEDPUSHED, item_disabled);

			// Menu
			palette.set(MENU_POPUPBACKGROUND, 0, background);
			palette.set(MENU_POPUPBORDERS, 0, border);
			palette.set(MENU_POPUPGUTTER, 0, gutter);
			palette.set(MENU_POPUPSEPARATOR, 0, separator);

			// Menu (comdlg32)
			palette.set(MENU_POPUPBACKGROUND, 1, background);
			palette.set(MENU_POPUPBORDERS, 1, border);
			palette.set(MENU_POPUPGUTTER, 1, gutter);
			palette.set(MENU_POPUPSEPARATOR, 1, separator);

			// ImmersiveStart::Menu
			palette.set(MENU_POPUPBACKGROUND, 3, background);
			palette.set(MENU_POPUPBORDERS, 3, border);
			palette.set(MENU_POPUPGUTTER, 3, gutter);
			palette.set(MENU_POPUPSEPARATOR, 3, separator);

			palette.set(MENU_POPUPITEM, MPI_NORMAL, item_normal);
			palette.set(MENU_POPUPITEM, MPI_DISABLED, item_disabled);
			palette.set(MENU_POPUPITEM, MPI_HOT, item_hot);
			palette.set(MENU_POPUPITEM, MPI_DISABLEDHOT, item_disabled);

			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_NORMAL, item_normal);
			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_DISABLED, item_disabled);
			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_HOT, item_hot);
			palette.set(MENU_POPUPITEMFOCUSABLE, MPI_DISABLEDHOT, item_disabled);

			palette.set(MENU_POPUPCHECK, MC_CHECKMARKNORMAL, mc_normal);
			palette.set(MENU_POPUPCHECK, MC_CHECKMARKDISABLED, mc_disabled);
			palette.set(MENU_POPUPCHECK, MC_BULLETNORMAL, mc_normal);
			palette.set(MENU_POPUPCHECK, MC_BULLETDISABLED, mc_disabled);

			palette.set(MENU_POPUPCHECKBACKGROUND, MCB_DISABLED, mc_disabled);
			palette.set(MENU_POPUPCHECKBACKGROUND, MCB_NORMAL, mc_normal);
			palette.set(MENU_POPUPCHECKBACKGROUND, MCB_BITMAP, mc_normal);

			palette.set(MENU_POPUPSUBMENU, MSM_NORMAL, mc_normal);
			palette.set(MENU_POPUPSUBMENU, MSM_DISABLED, mc_disabled);

			palette.set(MENU_SYSTEMCLOSE, MSYSC_NORMAL, mc_normal);
			palette.set(MENU_SYSTEMCLOSE, MSYSC_DISABLED, mc_disabled);

			palette.set(MENU_SYSTEMMAXIMIZE, MSYSMX_NORMAL, mc_normal);
			palette.set(MENU_SYSTEMMAXIMIZE, MSYSMX_DISABLED, mc_disabled);

			palette.set(MENU_SYSTEMMINIMIZE, MSYSMN_NORMAL, mc_normal);
			palette.set(MENU_SYSTEMMINIMIZE, MSYSMN_DISABLED, mc_disabled);

			palette.set(MENU_SYSTEMRESTORE, MSYSR_NORMAL, mc_normal);
			palette.set(MENU_SYSTEMRESTORE, MSYSR_DISABLED, mc_disabled);
		}

		//
		// マテリアルの後始末処理を実行します。
		//
		virtual void on_exit_material() override
		{
			MY_TRACE_FUNC("");
		}
	} menu_material;
}
