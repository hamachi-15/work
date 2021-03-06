#include "Messenger.h"
//-----------------------------
// 受信関数登録解除
//-----------------------------
void Messenger::Clear()
{
	receivers.clear();
}

//-----------------------------
// データ送信
//-----------------------------
void Messenger::SendData(const std::string& identifier, void* data)
{
	// 指定のキーの開始ノード終了ノードを取得
	auto it_range = receivers.equal_range(identifier);
	for (decltype(it_range.first) it = it_range.first; it != it_range.second; ++it)
	{
		it->second.func(data);
	}
}

//-----------------------------
// 関数登録(同一関数を複数登録できるので注意)
//-----------------------------
uint64_t Messenger::AddReceiver(const std::string& identifier, Receiver receiver)
{
	receivers.insert(std::make_pair(identifier, Data(increment_key, receiver)));
	return increment_key++;
}

//-----------------------------
// 関数登録解除(登録時のキーが必要)
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
