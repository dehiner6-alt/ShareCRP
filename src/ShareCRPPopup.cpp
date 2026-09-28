#include "ShareCRPPopup.hpp"

ShareCRPPopup* ShareCRPPopup::create(int accountID) {
    auto ret = new ShareCRPPopup();
    if (ret && ret->init(360.f, 200.f, "GJ_square01.png")) {
        ret->m_targetAccountID = accountID;
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ShareCRPPopup::setup(int accountID) {
    auto winSize = m_size;

    // Título de la ventana
    auto title = CCLabelBMFont::create("Share Creator Points", "goldFont.fnt");
    title->setPosition({winSize.width / 2, winSize.height - 25.f});
    title->setScale(0.7f);
    m_mainLayer->addChild(title);

    // Campo de texto para los puntos
    m_inputField = TextInput::create(220.f, "Amount", "chatFont.fnt");
    m_inputField->setPosition({winSize.width / 2, winSize.height / 2 + 10.f});
    m_mainLayer->addChild(m_inputField);

    // Botón de enviar
    auto submitBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Submit", "goldButton_01.png"),
        this,
        menu_selector(ShareCRPPopup::onsubmitButton)
    );
    
    auto menu = CCMenu::create();
    menu->addChild(submitBtn);
    menu->setPosition({winSize.width / 2, 45.f});
    m_mainLayer->addChild(menu);

    return true;
}

void ShareCRPPopup::onsubmitButton(CCObject* sender) {
    auto password = Mod::get()->getSettingValue<std::string>("password");
    
    // URL del servidor (puedes ajustarla según el endpoint de tu GDPS)
    std::string serverURL = "http://localhost/addcp"; 

    auto req = web::WebRequest();
    req.header("Authorization", password);
    req.param("accountID", std::to_string(m_targetAccountID));
    req.param("cp", m_inputField->getString());

    m_listener.bind([this](web::WebTask::Event* e) {
        if (auto res = e->getValue()) {
            if (res->ok()) {
                FLAlertLayer::create("Success", "Creator points granted!", "OK")->show();
                this->onClose(nullptr);
            } else {
                FLAlertLayer::create("Error", "Request failed or unauthorized.", "OK")->show();
            }
        }
    });

    m_listener.setFilter(req.post(serverURL));
}
