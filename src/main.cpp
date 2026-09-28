#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include "ShareCRPPopup.hpp"

using namespace geode::prelude;

class $modify(MyProfilePage, ProfilePage) {
    bool init(int accountID, bool p1) {
        // Llamamos al init original del juego
        if (!ProfilePage::init(accountID, p1)) {
            return false;
        }

        // Creamos el botón con texto "CRP"
        auto shareCrpBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("CRP", "goldFont.fnt", "GJ_button_01.png"),
            this,
            menu_selector(MyProfilePage::onOpenShareCRP)
        );
        shareCrpBtn->setID("share-crp-profile-button"_spr);

        // Buscamos el menú de la parte inferior del perfil (donde están bloquear, mensaje, etc.)
        // Intentamos obtener el menú por sus IDs estándar de Geode
        CCMenu* targetMenu = nullptr;

        if (auto menu = m_mainLayer->getChildByID("user-menu")) {
            targetMenu = static_cast<CCMenu*>(menu);
        } else if (auto menu = m_mainLayer->getChildByID("player-menu")) {
            targetMenu = static_cast<CCMenu*>(menu);
        } else if (auto menu = m_mainLayer->getChildByID("bottom-menu")) {
            targetMenu = static_cast<CCMenu*>(menu);
        } else if (auto menu = m_mainLayer->getChildByID("left-menu")) {
            targetMenu = static_cast<CCMenu*>(menu);
        }

        // Si encontramos el menú, agregamos el botón y reacomodamos el diseño
        if (targetMenu) {
            targetMenu->addChild(shareCrpBtn);
            targetMenu->updateLayout();
        }

        return true;
    }

    void onOpenShareCRP(CCObject*) {
        // Abrimos el popup pasándole la ID de la cuenta que estamos viendo
        ShareCRPPopup::create(m_accountID)->show();
    }
};
