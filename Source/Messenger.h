#pragma once
#include <functional>
#include <string>
#include <map>

//**************************************
//
// ���b�Z���W���[�N���X
//
//**************************************
class Messenger
{
public:
	using Function = void(void*);
	using Receiver = std::function<Function>;

	static constexpr const uint64_t invalid_key = 0xFFFFFFFFFFFFFFFF;
private:
	Messenger(){}
	~Messenger(){}

public:
	// �C���X�^���X�擾
	static Messenger& Instance()
	{
		static Messenger instance;
		return instance;
	}

	// ��M�֐��o�^����
	void Clear();

	// �f�[�^���M
	void SendData(const std::string& identifier, void* data);

	// �֐��o�^(����֐��𕡐��o�^�ł���̂Œ���)// TODO
	uint64_t AddReceiver(const std::string& identifier, Receiver receiver);
	
	// �֐��o�^����(�o�^���̃L�[���K�v)
	void RemoveReceiver(uint64_t key);
private:
	struct Data
	{
		uint64_t	key;
		Receiver	func;
		Data(uint64_t key, Receiver func) : key(key), func(func) {}
		bool operator==(Data& r)
		{
			return key == r.key;
		}
	};
	std::multimap<std::string, Data> receivers;
	uint64_t increment_key = 0;
};