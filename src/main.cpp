#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include "ShareCRPPopup.hpp"

using namespace geode::prelude;

class $modify(ShareCRPProfilePage, ProfilePage) {
    bool init(int accountID, bool isOwnProfile) {
        if (!ProfilePage::init(accountID, isOwnProfile)) return false;

        // Añadir el botón solo en perfiles de otros usuarios
        if (!isOwnProfile) {
            auto menu = this->getChildByID("left-menu");
            if (!menu) menu = m_mainLayer;

            auto btnSprite = CircleButtonSprite::createWithSpriteFrameName("geode.loader/geode-logo-outline.png"); // O tu ícono personalizado
            
            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(ShareCRPProfilePage::onShareCRP)
            );
            btn->setID("share-cp-button"_spr);

            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onShareCRP(CCObject* sender) {
        ShareCRPPopup::create(m_accountID)->show();
    }
};
