#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "ShareCRPPopup.hpp"

using namespace geode::prelude;

bool isMyGDPS() {
    return true; 
}

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }
        return true;
    }
};

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) {
            return false;
        }

        if (!isMyGDPS()) {
            return true; 
        }

        auto bgCircle = CCSprite::createWithSpriteFrameName("GJ_square01.png");
        bgCircle->setScale(0.7f);

        auto crpIcon = CCSprite::createWithSpriteFrameName("GJ_creatorIcon_001.png");
        if (!crpIcon) {
            crpIcon = CCSprite::createWithSpriteFrameName("difficulty_06_btn_001.png");
        }
        
        if (crpIcon) {
            crpIcon->setPosition(bgCircle->getContentSize() / 2);
            bgCircle->addChild(crpIcon);
        }

        auto shareCrpBtn = CCMenuItemSpriteExtra::create(
            bgCircle,
            this,
            menu_selector(MyLevelInfoLayer::onOpenShareCRP)
        );
        shareCrpBtn->setID("level-share-crp-button"_spr);

        if (auto sideMenu = this->getChildByID("right-side-menu")) {
            sideMenu->addChild(shareCrpBtn);
            sideMenu->updateLayout();
        }

        return true;
    }

    void onOpenShareCRP(CCObject*) {
        int currentLevelID = m_level->m_levelID;
        ShareCRPPopup::create(currentLevelID)->show();
    }
};
