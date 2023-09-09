#pragma once

#include "BaseJsonManager.h"
#include "../delegates/UploadDateDelegate.h"

class BetterInfoCache : public BaseJsonManager, public OnlineListDelegate {
	std::set<int> m_attemptedUsernames;
	std::set<int> m_attemptedLevelDates;
	inline static BetterInfoCache* m_instance = nullptr;
	BetterInfoCache();

public:
	UploadDateDelegate* m_uploadDateDelegate = nullptr;

	bool init();

	void validateLoadedData();

	static BetterInfoCache* sharedState(){
	    if(m_instance == nullptr){
	        m_instance = new BetterInfoCache;
	        m_instance->init();
	        m_instance->retain();
	    }
	    return m_instance;
	}

	void checkDailies();
	void cacheLevel(GJGameLevel* level);
	void cacheLevels(std::set<int> toDownload);

	void storeUserName(int userID, const std::string& username);
	void storeUploadDate(int levelID, const std::string& date);

	std::string getLevelName(int levelID);
	std::string getUserName(int userID, bool download = true);
	int getCoinCount(int levelID);

	std::string getUploadDate(int levelID, UploadDateDelegate* delegate = nullptr);

	void loadListFinished(cocos2d::CCArray*, const char*);
    void loadListFailed(const char*);
    void setupPageInfo(std::string, const char*);
};