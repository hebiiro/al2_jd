//----------------------------------------------------------------------------------
//	�X�N���v�g���W���[�� �w�b�_�[�t�@�C�� for AviUtl ExEdit2
//	By �j�d�m����
//----------------------------------------------------------------------------------

// �X�N���v�g���W���[�������\����
struct SCRIPT_MODULE_PARAM {
	// �����̐����擾����
	// �߂�l		: �����̐�
	int (*get_param_num)();

	// �����𐮐��Ŏ擾����
	// index		: �����̈ʒu(0�`)
	// �߂�l		: �����̒l (�擾�o���Ȃ��ꍇ��0)
	int (*get_param_int)(int index);

	// �����𕂓������_�Ŏ擾����
	// index		: �����̈ʒu(0�`)
	// �߂�l		: �����̒l (�擾�o���Ȃ��ꍇ��0)
	double (*get_param_double)(int index);

	// �����𕶎���(UTF-8)�Ŏ擾����
	// index		: �����̈ʒu(0�`)
	// �߂�l		: �����̒l (�擾�o���Ȃ��ꍇ��nullptr)
	LPCSTR (*get_param_string)(int index);

	// �������f�[�^�̃|�C���^�Ŏ擾����
	// index		: �����̈ʒu(0�`)
	// �߂�l		: �����̒l (�擾�o���Ȃ��ꍇ��nullptr)
	void* (*get_param_data)(int index);

	//--------------------------------

	// �����̘A�z�z��v�f�𐮐��Ŏ擾����
	// index		: �����̈ʒu(0�`)
	// key			: �L�[��(UTF-8)
	// �߂�l		: �����̒l (�擾�o���Ȃ��ꍇ��0)
	int (*get_param_table_int)(int index, LPCSTR key);

	// �����̘A�z�z��v�f�𕂓������_�Ŏ擾����
	// index		: �����̈ʒu(0�`)
	// key			: �L�[��(UTF-8)
	// �߂�l		: �����̒l (�擾�o���Ȃ��ꍇ��0)
	double (*get_param_table_double)(int index, LPCSTR key);

	// �����̘A�z�z��v�f�𕶎���(UTF-8)�Ŏ擾����
	// index		: �����̈ʒu(0�`)
	// key			: �L�[��(UTF-8)
	// �߂�l		: �����̒l (�擾�o���Ȃ��ꍇ��nullptr)
	LPCSTR (*get_param_table_string)(int index, LPCSTR key);

	//--------------------------------

	// �����̔z��v�f�̐����擾����
	// index		: �����̈ʒu(0�`)
	// �߂�l		: �z��v�f�̐�
	int (*get_param_array_num)(int index);

	// �����̔z��v�f�𐮐��Ŏ擾����
	// index		: �����̈ʒu(0�`)
	// key			: �z��̃C���f�b�N�X(0�`)
	// �߂�l		: �����̒l (�擾�o���Ȃ��ꍇ��0)
	int (*get_param_array_int)(int index, int key);

	// �����̔z��v�f�𕂓������_�Ŏ擾����
	// index		: �����̈ʒu(0�`)
	// key			: �z��̃C���f�b�N�X(0�`)
	// �߂�l		: �����̒l (�擾�o���Ȃ��ꍇ��0)
	double (*get_param_array_double)(int index, int key);

	// �����̔z��v�f�𕶎���(UTF-8)�Ŏ擾����
	// index		: �����̈ʒu(0�`)
	// key			: �z��̃C���f�b�N�X(0�`)
	// �߂�l		: �����̒l (�擾�o���Ȃ��ꍇ��nullptr)
	LPCSTR (*get_param_array_string)(int index, int key);

	//--------------------------------

	// �����̖߂�l��ǉ�����
	// value		: �߂�l
	void (*push_result_int)(int value);

	// ���������_�̖߂�l��ǉ�����
	// value		: �߂�l
	void (*push_result_double)(double value);

	// ������(UTF-8)�̖߂�l��ǉ�����
	// value		: �߂�l
	void (*push_result_string)(LPCSTR value);

	// �f�[�^�̃|�C���^�̖߂�l��ǉ�����
	// value		: �߂�l
	void (*push_result_data)(void* value);

	//--------------------------------

	// �����A�z�z��̖߂�l��ǉ�����
	// key			: �L�[��(UTF-8)�̔z��
	// value		: �߂�l�̔z��
	// num			: �z��̗v�f��
	void (*push_result_table_int)(LPCSTR* key, int* value, int num);

	// ���������_�A�z�z��̖߂�l��ǉ�����
	// key			: �L�[��(UTF-8)�̔z��
	// value		: �߂�l�̔z��
	// num			: �z��̗v�f��
	void (*push_result_table_double)(LPCSTR* key, double* value, int num);

	// ������(UTF-8)�A�z�z��̖߂�l��ǉ�����
	// key			: �L�[��(UTF-8)�̔z��
	// value		: �߂�l�̔z��
	// num			: �z��̗v�f��
	void (*push_result_table_string)(LPCSTR* key, LPCSTR* value, int num);

	//--------------------------------

	// �����z��̖߂�l��ǉ�����
	// value		: �߂�l�̔z��
	// num			: �z��̗v�f��
	void (*push_result_array_int)(int* value, int num);

	// ���������_�z��̖߂�l��ǉ�����
	// value		: �߂�l�̔z��
	// num			: �z��̗v�f��
	void (*push_result_array_double)(double* value, int num);

	// ������(UTF-8)�z��̖߂�l��ǉ�����
	// value		: �߂�l�̔z��
	// num			: �z��̗v�f��
	void (*push_result_array_string)(LPCSTR *value, int num);

	//--------------------------------

	// �G���[���b�Z�[�W��ݒ肷��
	// �Ăяo���ꂽ�֐����G���[�I������ꍇ�ɐݒ肵�܂�
	// message		: �G���[���b�Z�[�W(UTF-8)
	void (*set_error)(LPCSTR message);
};

//----------------------------------------------------------------------------------

// �X�N���v�g���W���[���֐���`�\����
struct SCRIPT_MODULE_FUNCTION {
	LPCWSTR name;						// �֐���
	void (*func)(SCRIPT_MODULE_PARAM*);	// �֐��ւ̃|�C���^
};

// �X�N���v�g���W���[���\����
struct SCRIPT_MODULE_TABLE {
	LPCWSTR information;				// �X�N���v�g���W���[���̏��
	SCRIPT_MODULE_FUNCTION* functions;	// �o�^����֐��̈ꗗ (SCRIPT_MODULE_FUNCTION��񋓂��Ċ֐�����null�̗v�f�ŏI�[�������X�g�ւ̃|�C���^)
};

