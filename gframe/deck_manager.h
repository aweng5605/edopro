#ifndef DECKMANAGER_H
#define DECKMANAGER_H

#include <unordered_map>
#include <vector>
#include "network.h"
#include "text_types.h"
#include "data_manager.h"

namespace ygo {

using banlist_content_t = std::unordered_map<uint32_t, int>;
using cardlist_type = std::vector<uint32_t>;

struct LFList {
	uint32_t hash;
	std::wstring listName;
	banlist_content_t content;
	bool whitelist;
};
struct Deck {
	std::vector<CardDataC*> main;
	std::vector<CardDataC*> extra;
	std::vector<CardDataC*> side;
	Deck() {}
	Deck(const Deck& ndeck) {
		main = ndeck.main;
		extra = ndeck.extra;
		side = ndeck.side;
	}
	void clear() {
		main.clear();
		extra.clear();
		side.clear();
	}
};
enum class DuelAllowedCards {
	ALLOWED_CARDS_OCG_ONLY,
	ALLOWED_CARDS_TCG_ONLY,
	ALLOWED_CARDS_OCG_TCG,
	ALLOWED_CARDS_WITH_PRERELEASE,
	ALLOWED_CARDS_ANY
};
class DeckManager {
private:
	int null_lflist_index = -1;
public:
	Deck current_deck;
	Deck pre_deck;
	std::vector<LFList> _lfList;

	bool LoadLFListSingle(const path_string& path);
	bool LoadLFListFolder(path_string path);
	void LoadLFList();
	void RefreshLFList();
	LFList* GetLFList(uint32_t lfhash);
	epro_wstringview GetLFListName(uint32_t lfhash);
	DeckError CheckDeck(Deck& deck, uint32_t lfhash, DuelAllowedCards allowedCards, bool doubled, uint32_t forbiddentypes = 0);
	int TypeCount(std::vector<CardDataC*> cards, uint32_t type);
	uint32_t LoadDeck(Deck& deck, uint32_t* dbuf, uint32_t mainc, uint32_t sidec, uint32_t mainc2 = 0, uint32_t sidec2 = 0);
	uint32_t LoadDeck(Deck& deck, cardlist_type mainlist, cardlist_type sidelist);
	bool LoadSide(Deck& deck, uint32_t* dbuf, uint32_t mainc, uint32_t sidec);
	bool LoadDeck(const path_string& file, Deck* deck = nullptr);
	bool LoadDeckDouble(const path_string& file, const path_string& file2, Deck* deck = nullptr);
	bool SaveDeck(Deck& deck, const path_string& name);
	bool SaveDeck(const path_string& name, cardlist_type mainlist, cardlist_type extralist, cardlist_type sidelist);
	const wchar_t* ExportDeckBase64(Deck& deck);
	const wchar_t* ExportDeckCardNames(Deck deck);
	void ImportDeckBase64(Deck& deck, const wchar_t* buffer);
	bool DeleteDeck(Deck& deck, const path_string& name);
	bool RenameDeck(const path_string& oldname, const path_string& newname);
};

extern DeckManager* gdeckManager;

}

#endif //DECKMANAGER_H
