#include "MenuScene.h"
#include "ScoreScene.h"
#include "ParseClient.h"
#include "cocos-ext.h"
#include "HttpClient.h"
#include "GameClient.h"
#include "DataManager.h"
#include "CustomTableViewCell.h"
#include "MyMacro.h"


USING_NS_CC;
USING_NS_CC_EXT;

#define NUM_REQUEST_FRIENDS		5000

ScoreScene::ScoreScene()
{
	
}

ScoreScene::~ScoreScene()
{
	//CCHttpClient::getInstance()->destroyInstance();
	//MY_RELEASE(m_dictFriends);
	MY_RELEASE(m_arrName);
	MY_RELEASE(m_arrScore);
}

bool ScoreScene::init()
{
	GameClient::sharedGameClient()->submitScore();

	if (!CCLayer::init())
	{
		return false;
	}

	// pres ////////////////////////////////////

	this->setKeypadEnabled(true);

	//////////////////////////////////////////////////////////////////////////

	m_listSize = 0;
	m_isLoaded = false;

	m_friendList = NULL;

	m_arrName = new CCArray(m_listSize);
	m_arrScore = new CCArray(m_listSize);
	m_arrName->retain();
	m_arrScore->retain();


	//m_dictFriends = CCDictionary::create();
	//m_dictFriends->retain();

	
	CCSprite* bg = CCSprite::create("bg_stars.png");
	bg->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(bg, 0);

	CCSprite* score_top = CCSprite::create("setting_top.png");
	score_top->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT - 100));
	this->addChild(score_top, 0);

	//facebook avatar
	m_userSprite = CCSprite::create("fb-profile.png");
	m_userSprite->setPosition(ccp(120, G_DESIGN_HEIGHT-276));
	this->addChild(m_userSprite);

	std::string name = DataManager::sharedDataManager()->GetName();
	m_lbName = CCLabelTTF::create(name.c_str(), "Marker Felt.ttf", 50);
	m_lbName->setFontFillColor(ccc3(0, 0, 0));
	m_lbName->setColor(ccc3(0, 0, 0));
	m_lbName->setPosition(ccp(400, 1280-276));
	this->addChild(m_lbName);

	//
	CCMenuItemImage *backItem = CCMenuItemImage::create(
		"back.png",
		"back1.png",
		this,
		menu_selector(ScoreScene::menuCallback));
	backItem->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT-1164));

	//

	m_isLoggedIn = false;
	CCMenuItem* fbLogIn = CCMenuItemImage::create("facebook-icon.png", NULL, NULL);
	CCMenuItem* fbLogOut = CCMenuItemImage::create("facebook-icon_off.png", NULL, NULL);
	m_fbItem = CCMenuItemToggle::createWithTarget(this,  menu_selector(ScoreScene::fbCallback), fbLogIn, fbLogOut, NULL);
	m_fbItem->setSelectedIndex(0);
	m_fbItem->setPosition(ccp(680,  G_DESIGN_HEIGHT-276));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	EziSocialObject::sharedObject()->setFacebookDelegate(this);

	if(EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in state
	{
		//CCLOG("Is logged in, ------------------------ NO NEED TO LOG IN");
		m_isLoggedIn = true;

		//login button
		m_fbItem->setSelectedIndex(1); //log out button 

		//avartar
		EziSocialObject::sharedObject()->getProfilePicForID(this, DataManager::sharedDataManager()->GetProfileID().c_str(), // Profile ID of current user
			128.0f, 128.0f, // Size of the image
			false // force download it from server
		);

		//full name
		m_lbName->setString(DataManager::sharedDataManager()->GetName().c_str());

		//get friends
		EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ALL_FRIENDS, 0, NUM_REQUEST_FRIENDS);
	}
	else //logged out stated
	{
		m_isLoggedIn = false;
		m_fbItem->setSelectedIndex(0); //log in button
	}
	
#endif


	//

	CCMenu* pMenu = CCMenu::create(backItem, m_fbItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	// table view

	m_sprCell = CCSprite::create("tablecell.png");
	m_sprCell->retain();
	CCSize cellsize = m_sprCell->getContentSize();
	CCSize tableSize = CCSizeMake(cellsize.width, cellsize.height * 6.0f);

	//vertical
	m_tableView = CCTableView::create(this, tableSize);
	m_tableView->setDirection(kCCScrollViewDirectionVertical); // (kCCScrollViewDirectionHorizontal);
	m_tableView->setAnchorPoint(CCPointZero);
	m_tableView->setPosition(ccp(400 - tableSize.width/2, (1280-718) - tableSize.height/2));
	m_tableView->setDelegate(this);
	m_tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	this->addChild(m_tableView);
	m_tableView->reloadData();
	m_tableView->setTouchEnabled(false);

	m_lbWaiting = CCLabelBMFont::create("Waiting...", "Mia_64.fnt");
	m_lbWaiting->setColor(ccc3(0, 0, 0));
	//m_waiting->setScale(0.6f);
	m_lbWaiting->setAlignment(kCCTextAlignmentCenter); //cocos2d::CCTextAlignment::
	m_lbWaiting->setPosition(ccp(G_DESIGN_WIDTH/2, G_DESIGN_HEIGHT/2));
	this->addChild(m_lbWaiting);
	


	//pre process ////////////////////////////////////////////////////

	if (m_isLoggedIn == false)
	{
		//CCLOG("Call getLeaderboard");

		CCString* url = ParseClient::sharedParseClient()->getUrl("getLeaderboard");

		CCHttpRequest* request = new CCHttpRequest();
		request->setHeaders(ParseClient::sharedParseClient()->m_header);
		request->setUrl(url->getCString());
		request->setRequestType(CCHttpRequest::kHttpPost);
		request->setResponseCallback(this, httpresponse_selector(ScoreScene::onGetLeaderboardCompleted));
		request->setTag("getLeaderboard");
		CCString* json = CCString::createWithFormat("{}");
		request->setRequestData(json->getCString(), strlen(json->getCString()));
		CCHttpClient::getInstance()->send(request);
		request->release();
	}

	//pre process


	return true;
}

void ScoreScene::menuCallback(CCObject* pSender)
{
	CCScene *pScene = CCTransitionFade::create(0.5, MenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void ScoreScene::onGetLeaderboardCompleted(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response)
{
	if (!response)
	{
		return;
	}

	//Show info
	//CCLOG("-----------------BEGIN-------------------");
	//CCLOG("Request: [%s] completed", response->getHttpRequest()->getTag());
	//CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		//CCLOG("Request Failed: Error buffer: %s", response->getErrorBuffer());
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
		//CCLOG("Content: %s", str.c_str());
		processData(str);
		m_lbWaiting->setVisible(false);
		m_tableView->setTouchEnabled(true);
	}

	
	//CCLOG("-----------------END-------------------");
}

void ScoreScene::processData( std::string str )
{
	//m_arrName
	//m_arrScore

	json_t *root;
	json_error_t error;
	json_t *results;

	root = json_loads(str.c_str(), strlen(str.c_str()), &error);

	if(!root)
	{
		//CCLOG("Error: on line %d: %s", error.line, error.text);
	}
	else
	{
		results = json_object_get(root, "result");
		if(!json_is_array(results))
		{
			//CCLOG("Error: Result is not a array");
		}
		else
		{
			//add you at first

			m_arrName->addObject(CCString::create(DataManager::sharedDataManager()->GetName()));
			m_arrScore->addObject(CCString::createWithFormat("%d", DataManager::sharedDataManager()->GetHighScore()));

			int sizearr = (int)json_array_size(results);
			m_listSize = sizearr + 1;

			for(int i = 0; i < sizearr; i++)
			{
				json_t *user, *username, *name, *score;
				user = json_array_get(results, i);				
				username = json_object_get(user, "username");
				name = json_object_get(user, "name");
				score = json_object_get(user, "score");

				m_arrName->addObject(CCString::create(json_string_value(name)));
				m_arrScore->addObject(CCString::createWithFormat("%d", (int)json_integer_value(score)));

				//CCLOG("%s: %d", json_string_value(name), (int)json_integer_value(score));
			}
		}
	}

	m_isLoaded = true;
	m_tableView->reloadData();
}

void ScoreScene::keyBackClicked()
{
	menuCallback(NULL);
}

void ScoreScene::uploadFacebookId( std::string fbID )
{
	//CCLOG("Call upload facebook id");

	std::string username = DataManager::sharedDataManager()->GetUsername();

	CCString* url = ParseClient::sharedParseClient()->getUrl("setFacebookID");
	
	CCHttpRequest* request = new CCHttpRequest();
	request->setHeaders(ParseClient::sharedParseClient()->m_header);
	request->setUrl(url->getCString());
	request->setRequestType(CCHttpRequest::kHttpPost);
	request->setResponseCallback(this, httpresponse_selector(ScoreScene::onUploadFacebookIdCompleted));
	request->setTag("setFacebookID");
	CCString* json = CCString::createWithFormat("{\"username\":\"%s\",\"fbID\":\"%s\"}", username.c_str(), fbID.c_str());
	
	//CCLOG("Upload Facebook ID: %s", json->getCString());

	request->setRequestData(json->getCString(), strlen(json->getCString()));
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void ScoreScene::onUploadFacebookIdCompleted( cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response )
{
	if (!response)
	{
		return;
	}

	//Show info
	//CCLOG("-----------------BEGIN-------------------");
	//CCLOG("Request: [%s] completed", response->getHttpRequest()->getTag());
	//CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		//CCLOG("Request Failed: Error buffer: %s", response->getErrorBuffer());
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
		//CCLOG("Content: %s", str.c_str());
	}
	//CCLOG("-----------------END-------------------");
}

void ScoreScene::uploadName( std::string name )
{
	//CCLOG("Call upload name");

	std::string username = DataManager::sharedDataManager()->GetUsername();

	CCString* url = ParseClient::sharedParseClient()->getUrl("setName");

	CCHttpRequest* request = new CCHttpRequest();
	request->setHeaders(ParseClient::sharedParseClient()->m_header);
	request->setUrl(url->getCString());
	request->setRequestType(CCHttpRequest::kHttpPost);
	request->setResponseCallback(this, httpresponse_selector(ScoreScene::onUploadNameCompleted));
	request->setTag("setName");
	CCString* json = CCString::createWithFormat("{\"username\":\"%s\",\"name\":\"%s\"}", username.c_str(), name.c_str());

	//CCLOG("Upload Name: %s", json->getCString());

	request->setRequestData(json->getCString(), strlen(json->getCString()));
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void ScoreScene::onUploadNameCompleted( cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response )
{
	if (!response)
	{
		return;
	}

	//Show info
	//CCLOG("-----------------BEGIN-------------------");
	//CCLOG("Request: [%s] completed", response->getHttpRequest()->getTag());
	//CCLOG("Status: [%i]", response->getResponseCode());

	if (!response->isSucceed())
	{
		//CCLOG("Request Failed: Error buffer: %s", response->getErrorBuffer());
	}
	else
	{
		std::vector<char> *buffer = response->getResponseData();
		std::string str(buffer->begin(), buffer->end());
		//CCLOG("Content: %s", str.c_str());
	}
	//CCLOG("-----------------END-------------------");
}


//new delegate

void ScoreScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	//CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize ScoreScene::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return m_sprCell->getContentSize();
}

CCTableViewCell* ScoreScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCString *order = CCString::createWithFormat("%d", idx);
	if (idx == 0)
	{
		order = CCString::create("My");
	}

	CCString *score;
	CCString *name;

	if (m_isLoaded == false)
	{
		score = CCString::create("0");
		name = CCString::create("Name");
	}
	else
	{
		score = (CCString*)m_arrScore->objectAtIndex(idx);
		name = (CCString*)m_arrName->objectAtIndex(idx);
	}

	
	CCTableViewCell *cell = table->dequeueCell();
	if (!cell) {
		cell = new CustomTableViewCell();
		cell->autorelease();

		CCSprite *sprite = CCSprite::create("tablecell.png");
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(0, 0));
		cell->addChild(sprite);

		CCLabelBMFont *lbOrder = CCLabelBMFont::create(order->getCString(), "Mia_64.fnt");
		lbOrder->setScale(0.6f);
		lbOrder->setAlignment(kCCTextAlignmentLeft); //cocos2d::CCTextAlignment::
		lbOrder->setPosition(ccp(20, m_sprCell->getContentSize().height/2));
		lbOrder->setAnchorPoint(ccp(0.0f, 0.5f));
		lbOrder->setTag(100);
		cell->addChild(lbOrder);

		CCLabelTTF *lbName = CCLabelTTF::create(name->getCString(), "Marker Felt.ttf", 64);
		lbName->setFontFillColor(ccc3(0,0,0));
		lbName->setScale(0.6f);
		lbName->setHorizontalAlignment(kCCTextAlignmentRight);
		//lbName->setAlignment(kCCTextAlignmentRight); //cocos2d::CCTextAlignment::
		lbName->setPosition(ccp(m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));
		lbName->setAnchorPoint(ccp(1.0f, 0.5f));
		lbName->setTag(101);
		cell->addChild(lbName);

		CCLabelBMFont *lbScore = CCLabelBMFont::create(score->getCString(), "Mia_64.fnt");
		lbScore->setScale(0.6f);
		lbScore->setAlignment(kCCTextAlignmentCenter); //cocos2d::CCTextAlignment::
		lbScore->setPosition(ccp(m_sprCell->getContentSize().width/2, m_sprCell->getContentSize().height/2));
		lbScore->setAnchorPoint(ccp(0.5f, 0.5f));
		lbScore->setTag(102);
		cell->addChild(lbScore);
	}
	else
	{
		CCLabelBMFont *lbOrder = (CCLabelBMFont*)cell->getChildByTag(100);
		lbOrder->setString(order->getCString());
		if (idx == 0)
		{
			lbOrder->setString("My");
		}
		lbOrder->setPosition(ccp(20, m_sprCell->getContentSize().height/2));

		CCLabelTTF *lbName = (CCLabelTTF*)cell->getChildByTag(101);
		lbName->setString(name->getCString());
		lbName->setPosition(ccp(m_sprCell->getContentSize().width, m_sprCell->getContentSize().height/2));

		CCLabelBMFont *lbScore = (CCLabelBMFont*)cell->getChildByTag(102);
		lbScore->setString(score->getCString());
		lbScore->setPosition(ccp(m_sprCell->getContentSize().width/2, m_sprCell->getContentSize().height/2));
	}


	return cell;
}

unsigned int ScoreScene::numberOfCellsInTableView(CCTableView *table)
{
	return m_listSize;
}

//end new delegate


// Facebook //=========================================

void ScoreScene::fbCallback( CCObject* pSender )
{
	//CCLOG("Call to Log In/Out facebook");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (EziSocialObject::sharedObject()->isFacebookSessionActive()) //logged in
	{
		//CCLOG("Log Out");
		EziSocialObject::sharedObject()->perfromLogoutFromFacebook();
	}
	else
	{
		//CCLOG("Log In");
		bool needPublicPermission = false;
		EziSocialObject::sharedObject()->performLoginUsingFacebook(needPublicPermission); // Pass true if you need publish permission also
	}
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void ScoreScene::fbSessionCallback(int responseCode, const char *responseMessage)
{
	switch (responseCode)
	{
	case EziSocialWrapperNS::RESPONSE_CODE::ERROR_INTERNET_NOT_AVAILABLE:
		//CCLOG("ERROR_INTERNET_NOT_AVAILABLE");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::ERROR_READ_PERMISSION_ERROR:
		//CCLOG("ERROR_READ_PERMISSION_ERROR");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::ERROR_PUBLISH_PERMISSION_ERROR:
		//CCLOG("ERROR_PUBLISH_PERMISSION_ERROR");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_USER_PERMISSION_REVOKED:
		//CCLOG("FB_LOGIN_USER_PERMISSION_REVOKED");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_APP_NOT_ALLOWERD_TO_USE_FB:
		//CCLOG("FB_LOGIN_APP_NOT_ALLOWERD_TO_USE_FB");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_PERMISSION_DENIED:
		//CCLOG("FB_LOGIN_PERMISSION_DENIED");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_SUCCESSFUL: /////////////////// logged in state
		{
			//CCMessageBox("Log in success!", "Info");
			//CCLOG("FB_LOGIN_SUCCESSFUL");

			m_isLoggedIn = true;
			m_fbItem->setSelectedIndex(1); //log out button

			//get profile information
			//bool needsEmail = true;
//debug
			EziSocialObject::sharedObject()->fetchFBUserDetails(true); //need email = true

			break;
		}
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGIN_FAILED:
		//CCLOG("FB_LOGIN_FAILED");
		break;
	case EziSocialWrapperNS::RESPONSE_CODE::FB_LOGOUT_SUCCESSFUL:  /////////////////// logged out state
		{
			//CCLOG("FB_LOGOUT_SUCCESSFUL");
			//CCMessageBox("Log out success!", "Info");

			m_isLoggedIn = false;
			m_fbItem->setSelectedIndex(0); //log in button

			m_userSprite->removeAllChildren();
			m_lbName->setString(G_DEFAULT_NAME);
			
			DataManager::sharedDataManager()->SetName(G_DEFAULT_NAME);
			DataManager::sharedDataManager()->SetProfileID("NULL");
			DataManager::sharedDataManager()->SetFbUserName("NULL");

			break;
		}		
	}
}

void ScoreScene::fbUserPhotoCallback(const char *userPhotoPath, const char* fbID)
{
	//const char* currentUserId = EziSocialObject::sharedObject()->getCurrentFacebookUser()->getProfileID();

	////CCLOG("fbID = %s", fbID);
	////CCLOG("userPhotoPath = %s", userPhotoPath);
	////CCLOG("currentUserId = %s", currentUserId);

	if ((strcmp(userPhotoPath, "") != 0)) // && (strcmp(currentUserId, fbID) == 0)
	{
		CCSprite* userPhoto = CCSprite::create(userPhotoPath);
		userPhoto->setPosition(ccp(m_userSprite->getContentSize().width/2, m_userSprite->getContentSize().height/2));
		userPhoto->setScale(m_userSprite->getContentSize().width/userPhoto->getContentSize().width);
		m_userSprite->addChild(userPhoto);
	}
}

void ScoreScene::fbUserDetailCallback( int responseCode, const char* responseMessage, EziFacebookUser* fbUser )
{
	if (fbUser != NULL)
	{
		// Set the current user in EziSocialObject to use it in future. EziSocialObject is immune to CCScene lifecycle. It is singleton class and will live forever.
		EziSocialObject::sharedObject()->setCurrentFacebookUser(fbUser);

		//save data
		// You can call method associated with EziFacebookUser and use the user details. For example:-
		EziFacebookUser* currentUser = EziSocialObject::sharedObject()->getCurrentFacebookUser();
		std::string fullname = currentUser->getFullName();
		//std::string emailID = currentUser->getEmailID();
		//std::string gender = currentUser->getGender();
		std::string userName = currentUser->getUserName();
		std::string profileID = currentUser->getProfileID();
		//std::string accessToken = currentUser->getAccessToken();

		DataManager::sharedDataManager()->SetName(fullname.c_str());
		DataManager::sharedDataManager()->SetProfileID(profileID.c_str());
		DataManager::sharedDataManager()->SetFbUserName(userName.c_str());

		m_lbName->setString(fullname.c_str());

		//CCLOG("FullName: %s", fullname.c_str());
		//CCLOG("EmailID: %s", emailID.c_str());
		//CCLOG("Gender: %s", gender.c_str());
		//CCLOG("UserName: %s", userName.c_str());
		//CCLOG("ProfileID: %s", profileID.c_str());
		//CCLOG("AccessToken: %s", accessToken.c_str());

		//avartar
//debug
		EziSocialObject::sharedObject()->getProfilePicForID(this, currentUser->getProfileID(), // Profile ID of current user
			128.0f, 128.0f, // Size of the image
			false // force download it from server
		);

		//facebook id to parse
//debug
		uploadFacebookId(profileID);
		uploadName(fullname);

		//friends
		//get friends: ALL_FRIENDS, ONLY_INSTALLED, ONLY_NOT_INSTALLED, ONLY_MY_DEVICES
		//from 0 to NUM_REQUEST_FRIENDS
//debug 
		EziSocialObject::sharedObject()->getFriends(EziSocialWrapperNS::FB_FRIEND_SEARCH::ONLY_INSTALLED, 0, NUM_REQUEST_FRIENDS);

	}
}

void ScoreScene::fbFriendsCallback(int responseCode, const char *responseMessage, cocos2d::CCArray *friends)
{
	//CCMessageBox(responseMessage, "Friends Callback");
	//CCLOG("Friends callback >>>>>>>>>>>>>>>>>>>>> Friends callback");

	if (m_friendList != NULL)
	{
		m_friendList->release();
		m_friendList = NULL;
	}

	m_friendList = CCArray::createWithArray(friends);
	m_friendList->retain();

	getFbFriendsScore();
}


void ScoreScene::getFbFriendsScore()
{
	m_isLoaded = false;
	m_lbWaiting->setVisible(true);

	std::string textToDisplay = "{\"ids\":[";
	CCObject* it;
	CCARRAY_FOREACH(m_friendList, it)
	{
		EziFacebookFriend* fbFriend = dynamic_cast<EziFacebookFriend*>(it);
		if (NULL != fbFriend)
		{
			//std::string fbid = fbFriend->getFBID();
			textToDisplay.append("\"").append(fbFriend->getFBID()).append("\",");
		}
	}


	textToDisplay.append("\"NULL\"]}");
	//CCLOG("JSON: %s", textToDisplay.c_str());

	//////////////////////////////////////////////////////////////////////////
	
	//CCLOG("Call get facebook friends score");

	CCString* url = ParseClient::sharedParseClient()->getUrl("getFbFriendsScore");

	CCHttpRequest* request = new CCHttpRequest();
	request->setHeaders(ParseClient::sharedParseClient()->m_header);
	request->setUrl(url->getCString());
	request->setRequestType(CCHttpRequest::kHttpPost);
	request->setResponseCallback(this, httpresponse_selector(ScoreScene::onGetLeaderboardCompleted));
	request->setTag("getFbFriendsScore");
	request->setRequestData(textToDisplay.c_str(), strlen(textToDisplay.c_str()));
	CCHttpClient::getInstance()->send(request);
	request->release();
}


#endif

// Facebook //=========================================
