#include "ShareCRPPopup.hpp"

ShareCRPPopup* ShareCRPPopup::create(int accountID) {
    auto ret = new ShareCRPPopup();
    if (ret && ret->initAnchored(360.f, 180.f, accountID)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ShareCRPPopup::setup(int accountID) {
    m_accountID = accountID;
    setTitle("Share Creator Points");

    m_inputField = TextInput::create(280.f, "Amount of CPs", "chatFont.fnt");
    
    auto winSize = m_mainLayer->getContentSize();
    m_inputField->setPosition({winSize.width / 2, winSize.height / 2 + 10.f});
    m_mainLayer->addChild(m_inputField);

    auto cancelBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Cancel"),
        this,
        menu_selector(ShareCRPPopup::onClose)
    );

    auto submitBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Submit"),
        this,
        menu_selector(ShareCRPPopup::onSubmit)
    );

    auto menu = CCMenu::create();
    menu->addChild(cancelBtn);
    menu->addChild(submitBtn);
    menu->alignItemsHorizontallyWithPadding(20.f);
    menu->setPosition({winSize.width / 2, 35.f});
    m_mainLayer->addChild(menu);

    // Listener para recibir la respuesta del servidor
    m_webListener.bind([this](web::WebTask::Event* e) {
        if (auto res = e->getValue()) {
            if (res->ok()) {
                FLAlertLayer::create("Success", "Creator Points granted successfully!", "OK")->show();
                this->onClose(nullptr);
            } else {
                FLAlertLayer::create("Error", "Failed to grant CPs. Check permissions/backend.", "OK")->show();
            }
        }
    });

    return true;
}

void ShareCRPPopup::onSubmit(CCObject*) {
    auto password = Mod::get()->getSettingValue<std::string>("password");
    
    // DETECCIÓN AUTOMÁTICA DEL GDPS ACTUAL:
    std::string gdpsBase = GJAccountManager::sharedState()->m_serverURL;
    if (gdpsBase.empty()) {
        gdpsBase = "http://www.boomlings.com/database"; // Fallback por defecto
    }
    
    // Aseguramos que no termine con diagonal antes de concatenar
    if (gdpsBase.back() == '/') {
        gdpsBase.pop_back();
    }
    
    std::string fullURL = gdpsBase + "/addcp";

    auto req = web::WebRequest();
    req.header("Authorization", password);
    req.param("accountID", std::to_string(m_accountID));
    req.param("cp", m_inputField->getString());
    
    m_webListener.setFilter(req.post(fullURL));
}
