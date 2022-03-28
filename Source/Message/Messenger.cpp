#include "Messenger.h"
//-----------------------------
// óMŠÖ”“o˜^‰ğœ
//-----------------------------
void Messenger::Clear()
{
	receivers.clear();
}

//-----------------------------
// ƒf[ƒ^‘—M
//-----------------------------
void Messenger::SendData(const std::string& identifier, void* data)
{
	// w’è‚ÌƒL[‚ÌŠJnƒm[ƒhI—¹ƒm[ƒh‚ğæ“¾
	auto it_range = receivers.equal_range(identifier);
	for (decltype(it_range.first) it = it_range.first; it != it_range.second; ++it)
	{
		it->second.func(data);
	}
}

//-----------------------------
// ŠÖ”“o˜^(“¯ˆêŠÖ”‚ğ•¡”“o˜^‚Å‚«‚é‚Ì‚Å’ˆÓ)
//-----------------------------
uint64_t Messenger::AddReceiver(const std::string& identifier, Receiver receiver)
{
	receivers.insert(std::make_pair(identifier, Data(increment_key, receiver)));
	return increment_key++;
}

//-----------------------------
// ŠÖ”“o˜^‰ğœ(“o˜^‚ÌƒL[‚ª•K—v)
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
