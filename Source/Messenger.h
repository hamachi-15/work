#pragma once
#include <functional>
#include <string>
#include <map>

//**************************************
//
// メッセンジャークラス
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
	// インスタンス取得
	static Messenger& Instance()
	{
		static Messenger instance;
		return instance;
	}

	// 受信関数登録解除
	void Clear();

	// データ送信
	void SendData(const std::string& identifier, void* data);

	// 関数登録(同一関数を複数登録できるので注意)// TODO
	uint64_t AddReceiver(const std::string& identifier, Receiver receiver);
	
	// 関数登録解除(登録時のキーが必要)
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