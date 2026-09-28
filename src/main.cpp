#include <Geode/Geode.hpp>
#include "ShareCRPPopup.hpp"

using namespace geode::prelude;

#include <Geode/modify/LevelInfoLayer.hpp>
class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1))
            return false;

        auto myButton = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("CRP", "goldFont.fnt", "btn_small_01.png", 0.8f),
            this,
            menu_selector(MyLevelInfoLayer::onShareCRP)
        );

        auto menu = this->getChildByID("right-side-menu");
        if (menu) {
            menu->addChild(myButton);
            menu->updateLayout();
        }

        return true;
    }

    void onShareCRP(CCObject*) {
        std::string serverURL = GameManager::sharedState()->m_gameServerURL;
        
        // Detect if the user is on RobTop's official servers
        if (serverURL.empty() || serverURL.find("boomlings.com") != std::string::npos) {
            FLAlertLayer::create(
                "ShareCRP", 
                "ShareCRP Button is <cr>INACTIVE</c> Because You're not on a GDPS.", 
                "OK"
            )->show();
            return;
        }

        int creatorAccountID = m_level->m_accountID;
        if (creatorAccountID <= 0) {
            FLAlertLayer::create("Error", "Could not retrieve the creator's account ID.", "OK")->show();
            return;
        }

        // Open the popup passing the creator's account ID
        ShareCRPPopup::create(creatorAccountID)->show();
    }
};
