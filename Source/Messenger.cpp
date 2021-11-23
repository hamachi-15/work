#include "Messenger.h"
//-----------------------------
// ��M�֐��o�^����
//-----------------------------
void Messenger::Clear()
{
	receivers.clear();
}

//-----------------------------
// �f�[�^���M
//-----------------------------
void Messenger::SendData(const std::string& identifier, void* data)
{
	// �w��̃L�[�̊J�n�m�[�h�I���m�[�h���擾
	auto it_range = receivers.equal_range(identifier);
	for (decltype(it_range.first) it = it_range.first; it != it_range.second; ++it)
	{
		it->second.func(data);
	}
}

//-----------------------------
// �֐��o�^(����֐��𕡐��o�^�ł���̂Œ���)
//-----------------------------
uint64_t Messenger::AddReceiver(const std::string& identifier, Receiver receiver)
{
	receivers.insert(std::make_pair(identifier, Data(increment_key, receiver)));
	return increment_key++;
}

//-----------------------------
// �֐��o�^����(�o�^���̃L�[���K�v)
//-----------------------------
void Messenger::RemoveReceiver(uint64_t key)
{
	auto it = receivers.begin();
	auto itend = receivers.end();
	while (it != itend)
	{
		if (it->second.key == key)
		{
			receivers.erase(it);
			break;
		}
		it++;
	}
}
