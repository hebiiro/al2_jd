//----------------------------------------------------------------------------------
//	�T���v���X�N���v�g���W���[���v���O�C�� for AviUtl ExEdit2
//----------------------------------------------------------------------------------
#include <windows.h>
#include <algorithm>

#include "module2.h"
#include "filter2.h" // PIXEL_RGBA��`�p 

//---------------------------------------------------------------------
//	�v���O�C��DLL�������֐� (����`�Ȃ�Ă΂�܂���)
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) bool InitializePlugin(DWORD version) {
	return true;
}

//---------------------------------------------------------------------
//	�v���O�C��DLL����֐� (����`�Ȃ�Ă΂�܂���)
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) void UninitializePlugin() {
}

//---------------------------------------------------------------------
//	���v���v�Z����T���v���֐�
//---------------------------------------------------------------------
void sum(SCRIPT_MODULE_PARAM* param) {
	// �����̍��v���v�Z
	double total = 0.0;
	auto num = param->get_param_num();
	for (int i = 0; i < num; i++) {
		total += param->get_param_double(i);
	}
	param->push_result_double(total);

}

//---------------------------------------------------------------------
//	���邳�𒲐�����T���v���֐�
//---------------------------------------------------------------------
void luminance(SCRIPT_MODULE_PARAM* param) {
	// �������擾
	auto n = param->get_param_num();
	if (n != 4) {
		param->set_error(u8"�����̐�������������܂���");
		return;
	}
	auto p = (PIXEL_RGBA*)param->get_param_data(0);
	auto w = param->get_param_int(1);
	auto h = param->get_param_int(2);
	auto v = param->get_param_double(3);
	if (!p || w <= 0 || h <= 0) {
		param->set_error(u8"�����̒l������������܂���");
		return;
	}

	// ���邳�𒲐�
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			p->r = (unsigned char)std::clamp(p->r * v, 0.0, 255.0);
			p->g = (unsigned char)std::clamp(p->g * v, 0.0, 255.0);
			p->b = (unsigned char)std::clamp(p->b * v, 0.0, 255.0);
			p++;
		}
	}
}

//---------------------------------------------------------------------
//	�X�N���v�g���W���[���֐����X�g��`
//---------------------------------------------------------------------
SCRIPT_MODULE_FUNCTION functions[] = {
	{ L"sum", sum },
	{ L"luminance", luminance },
	{ nullptr }
};

//---------------------------------------------------------------------
//	�X�N���v�g���W���[���\���̒�`
//---------------------------------------------------------------------
SCRIPT_MODULE_TABLE script_module_table = {
	L"Sample ScriptModule version 2.00 By �j�d�m����",	// ���W���[���̏��
	functions
};

//---------------------------------------------------------------------
//	�X�N���v�g���W���[���\���̂̃|�C���^��n���֐�
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) SCRIPT_MODULE_TABLE* GetScriptModuleTable(void) {
	return &script_module_table;
}
