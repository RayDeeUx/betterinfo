#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BetterInfoStats : public GManager {
	inline static BetterInfoStats* m_instance = nullptr;
	BetterInfoStats();


public:
	Ref<cocos2d::CCDictionary> m_normalDict = CCDictionary::create();
	Ref<cocos2d::CCDictionary> m_practiceDict = CCDictionary::create();
	Ref<cocos2d::CCDictionary> m_firstPlayedDict = CCDictionary::create();
	Ref<cocos2d::CCDictionary> m_lastPlayedDict = CCDictionary::create();
	Ref<cocos2d::CCDictionary> m_normalAttemptDict = CCDictionary::create();
	Ref<cocos2d::CCDictionary> m_practiceAttemptDict = CCDictionary::create();

	bool init();
	void migrateSaveData();
	void migrationPopup(float dt);
	void encodeDataTo(DS_Dictionary* data);
    void dataLoaded(DS_Dictionary* data);
    void firstLoad();
	static BetterInfoStats* sharedState(){
	    if(m_instance == nullptr){
	        m_instance = new BetterInfoStats;
	        m_instance->init();
	    }
	    return m_instance;
	}
	void logCompletion(GJGameLevel* level, bool practice);
	void logCompletion(GJGameLevel* level, bool practice, time_t timestamp);
	time_t getCompletion(GJGameLevel* level, bool practice);
	void logPlay(GJGameLevel* level);
	time_t getPlay(GJGameLevel* level, bool last);
	std::string keyForLevel(GJGameLevel* level);
	void logAttempt(GJGameLevel* level, bool practice);
	int getAttempts(GJGameLevel* level, bool practice);
};