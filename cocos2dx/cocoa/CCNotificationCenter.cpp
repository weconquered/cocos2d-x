/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org
Copyright (c) 2011 Stephane Capo, Nicolas Blondel, Quentin Stere, Noel Leron

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCNotificationCenter.h"
#include "CCString.h"

using namespace cocos2d;

CCNotificationCenter* CCNotificationCenter::myDefaultCenter=0;

CCNotificationCenter::~CCNotificationCenter()
{
	myObserverMap.clear();
	myNotificationMap.clear();
}

CCNotificationCenter::CCNotificationCenter()
{
	myObserverMap.clear();
	myNotificationMap.clear();
	myRemoveObserverMap.clear();

	postLevel=0;
}


void CCNotificationCenter::addObserver(void* observer,SEL_CallFuncNT selector,CCString* notificationCCName,CCObject* sender)
{
	std::string notificationName=notificationCCName->m_sString;
		// first map : observers
	ObserverStruct newobstruct;
	newobstruct.mySelector=selector;
	newobstruct.mySender=sender;
	newobstruct.myIsRemoved = false;

	// first check if observer already exist
	if(myObserverMap.find(observer)!=myObserverMap.end())
	{
		NotifVectorStruct&	currentvector=myObserverMap[observer];

		bool alreadythere=false;
		// check in current list if observer is not already there 
		for(unsigned int i=0;i<currentvector.myVector.size();i++)
		{
			if(currentvector.myVector[i] == notificationName)
			{
				alreadythere=true;
				break;
			}
		}
		if(!alreadythere)
		{
			currentvector.myVector.push_back(notificationName);
		}
	}
	else // first use of this observer
	{
		NotifVectorStruct	toadd;
		toadd.myVector.push_back(notificationName);
		myObserverMap[observer]=toadd;
	}

	// second map : messages

	if(myNotificationMap.find(notificationName) != myNotificationMap.end())
	{
		std::vector<ObserverStructVector >& currentmap=myNotificationMap[notificationName];
		std::vector<ObserverStructVector >::iterator	itobs;

		bool	found=false;
		for(itobs = currentmap.begin();itobs != currentmap.end();itobs++)
		{
			if((*itobs).myObserver == observer)
			{
				bool alreadythere=false;
				int alreadythereindex=0;
				for(unsigned int i=0;i<(*itobs).myVector.size();i++)
				{
					if(((*itobs).myVector[i].mySelector == selector)&&(((*itobs).myVector[i].mySender == sender)|| (sender == 0)))
					{
						(*itobs).myVector[i].myIsRemoved=false;
						alreadythere=true;
						alreadythereindex=i;
						break;
					}
				}
				if(!alreadythere)
				{
					(*itobs).myVector.push_back(newobstruct);
				}
				else // already there, check if sender is different
				{
					if(sender == 0)
					{
						(*itobs).myVector[alreadythereindex].mySender=0;
					}
					else if(sender != (*itobs).myVector[alreadythereindex].mySender)
					{
						// what can I do ?
					}
				}
				found=true;
				break;
			}
		}

		if(!found)
		{
			ObserverStructVector	newvectortoadd;
			newvectortoadd.myVector.clear();
			newvectortoadd.myObserver=observer;
			newvectortoadd.myVector.push_back(newobstruct);
			currentmap.push_back(newvectortoadd);
		}
	}
	else
	{
		ObserverStructVector	newvectortoadd;
		newvectortoadd.myVector.clear();
		newvectortoadd.myObserver=observer;
		newvectortoadd.myVector.push_back(newobstruct);

		std::vector<ObserverStructVector > newmaptoadd;

		newmaptoadd.push_back(newvectortoadd);
		
		myNotificationMap[notificationName]=newmaptoadd;
	}
}


void CCNotificationCenter::removeObserver(void* observer,CCString* notificationName,CCObject* sender)
{

	std::string notifName="";
	if(notificationName)
		notifName=notificationName->m_sString;

	protectedSetRemoveState(observer,notifName,sender);

	if(myRemoveObserverMap.find(observer)!=myRemoveObserverMap.end())
	{
		std::vector<removeObserverStruct>&	currentvector=myRemoveObserverMap[observer];
		removeObserverStruct	toAdd;
		toAdd.notificationName=notifName;
		toAdd.sender=sender;
		currentvector.push_back(toAdd);		
	}
	else
	{
		std::vector<removeObserverStruct>	currentvector;
		
		removeObserverStruct	toAdd;
		toAdd.notificationName=notifName;
		toAdd.sender=sender;
		currentvector.push_back(toAdd);	
		myRemoveObserverMap[observer]=currentvector;
		
	}
}

void CCNotificationCenter::manageRemoveObserverList()
{
	std::map<void*,std::vector<removeObserverStruct> >::const_iterator it;
	for(it=myRemoveObserverMap.begin();it!=myRemoveObserverMap.end();++it)
	{
		void* obs=(*it).first;
		const std::vector<removeObserverStruct>&	currentvector=(*it).second;
		std::vector<removeObserverStruct>::const_iterator	itv;
		for(itv=currentvector.begin();itv!=currentvector.end();++itv)
		{
			protectedRemoveObserver(obs,(*itv).notificationName,(*itv).sender);
		}
	}

	myRemoveObserverMap.clear();

}

void CCNotificationCenter::protectedSetRemoveState(void* observer,const std::string&  notificationName,CCObject* sender)
{
	if((sender==0)&&(notificationName==""))
	{
		protectedSetRemoveState(observer);
		return;
	}
	bool canErase=false;
	std::map<void*,NotifVectorStruct >::iterator	it=myObserverMap.find(observer);
	if(it!=myObserverMap.end())
	{
		NotifVectorStruct&	currentvector=myObserverMap[observer];
		std::vector<std::string>::iterator	itvector;

		for(itvector=currentvector.myVector.begin();itvector!=currentvector.myVector.end();itvector++)
		{
			if(notificationName == (*itvector))
			{
				if(myNotificationMap.find((*itvector)) != myNotificationMap.end())
				{
					std::vector<ObserverStructVector>&		currentprotocolmap=myNotificationMap[(*itvector)];
					std::vector<ObserverStructVector>::iterator  vectorToRemoveit;

					for(vectorToRemoveit=currentprotocolmap.begin();vectorToRemoveit!=currentprotocolmap.end();vectorToRemoveit++)
					{
						if((*vectorToRemoveit).myObserver == observer)
						{	
							std::vector<ObserverStruct>::iterator removestructit;
							for(removestructit = (*vectorToRemoveit).myVector.begin();removestructit != (*vectorToRemoveit).myVector.end();removestructit++)
							{
								if( ((*removestructit).mySender == sender) || (sender == 0))
								{
									(*removestructit).myIsRemoved=true;
									break;
								}
							}
							break;
						}
					}
				}
				break;
			}
		}
	}
}

void CCNotificationCenter::protectedSetRemoveState(void* observer)
{
	// search all notifications for this observer
	std::map<void*,NotifVectorStruct >::iterator	it=myObserverMap.find(observer);
	if(it!=myObserverMap.end())
	{
		NotifVectorStruct&	currentvector=myObserverMap[observer];

		// search each notification name and set remove state

		unsigned int i;
		for(i=0;i<currentvector.myVector.size();i++)
		{
			std::string notifname=currentvector.myVector[i];
			if(myNotificationMap.find(notifname) != myNotificationMap.end())
			{
				std::vector<ObserverStructVector>&		currentprotocolmap=myNotificationMap[notifname];
				std::vector<ObserverStructVector>::iterator  vectorToRemoveit;

				for(vectorToRemoveit=currentprotocolmap.begin();vectorToRemoveit!=currentprotocolmap.end();vectorToRemoveit++)
				{
					if((*vectorToRemoveit).myObserver == observer)
					{	
						std::vector<ObserverStruct>::iterator	itFlag;
						for(itFlag=(*vectorToRemoveit).myVector.begin();itFlag!=(*vectorToRemoveit).myVector.end();itFlag++)
						{
							(*itFlag).myIsRemoved=true;
						}
						break;
					}
				}
			}
		}
	}
}

void CCNotificationCenter::protectedRemoveObserver(void* observer)
{
	// search all notifications for this observer

	std::map<void*,NotifVectorStruct >::iterator	it=myObserverMap.find(observer);
	if(it!=myObserverMap.end())
	{
		NotifVectorStruct&	currentvector=myObserverMap[observer];

		// search each notification name and remove observer form the list

		unsigned int i;
		for(i=0;i<currentvector.myVector.size();i++)
		{
			std::string notifname=currentvector.myVector[i];

			std::map<std::string,std::vector<ObserverStructVector> >::iterator	itfound=myNotificationMap.find(notifname);
			if(itfound != myNotificationMap.end())
			{
				std::vector<ObserverStructVector>&		currentprotocolmap=myNotificationMap[notifname];
				std::vector<ObserverStructVector>::iterator  vectorToRemoveit;

				for(vectorToRemoveit=currentprotocolmap.begin();vectorToRemoveit!=currentprotocolmap.end();vectorToRemoveit++)
				{
					if((*vectorToRemoveit).myObserver == observer)
					{	
						currentprotocolmap.erase(vectorToRemoveit);
						break;
					}
				}

				if(currentprotocolmap.size()==0)
				{
					myNotificationMap.erase(itfound);				
				}
			}
		}
		
		currentvector.myVector.clear();
		myObserverMap.erase(it);
	}

}



void CCNotificationCenter::protectedRemoveObserver(void* observer,const std::string& notificationName,CCObject* sender)
{
	if((sender==0)&&(notificationName==""))
	{
		protectedRemoveObserver(observer);
		return;
	}
	bool canErase=false;
	std::map<void*,NotifVectorStruct >::iterator	it=myObserverMap.find(observer);
	if(it!=myObserverMap.end())
	{
		NotifVectorStruct&	currentvector=myObserverMap[observer];
		std::vector<std::string>::iterator	itvector;

		for(itvector=currentvector.myVector.begin();itvector!=currentvector.myVector.end();itvector++)
		{
			if(notificationName == (*itvector))
			{
				std::map<std::string,std::vector<ObserverStructVector> >::iterator	itfound=myNotificationMap.find((*itvector));

				if(itfound != myNotificationMap.end())
				{
					std::vector<ObserverStructVector>&		currentprotocolmap=myNotificationMap[(*itvector)];
					std::vector<ObserverStructVector>::iterator  vectorToRemoveit;

					for(vectorToRemoveit=currentprotocolmap.begin();vectorToRemoveit!=currentprotocolmap.end();vectorToRemoveit++)
					{
						if((*vectorToRemoveit).myObserver == observer)
						{	
							std::vector<ObserverStruct>::iterator removestructit;
							for(removestructit = (*vectorToRemoveit).myVector.begin();removestructit != (*vectorToRemoveit).myVector.end();removestructit++)
							{
								if((*removestructit).myIsRemoved)
								{	
									if( ((*removestructit).mySender == sender) || (sender == 0))
									{
										(*vectorToRemoveit).myVector.erase(removestructit);
										if((*vectorToRemoveit).myVector.size() == 0)
										{
											currentprotocolmap.erase(vectorToRemoveit);
										}
										canErase=true;
										break;
									}
								}
							}
							break;
						}
					}
					if(currentprotocolmap.size() == 0)
					{
						myNotificationMap.erase(itfound);
					}
				}

				if(canErase)
				{
					currentvector.myVector.erase(itvector);
					if(currentvector.myVector.size() == 0)
					{
						myObserverMap.erase(it);
					}
				}
				break;
			}
		}
	}
}

//#define PRINTF_NOTIFICATION

#ifdef PRINTF_NOTIFICATION
void	PrintPost(CCString* notificationName,CCObject* sender,CCObject* userInfo)
{
	std::string	printname=notificationName->m_sString;
	std::string	printsender="nil";
	if(sender)
	{
		printsender=sender->getClassType();
#ifdef _DEBUG
		if(!sender->m_LogEvent)
		{
			return ;
		}
#endif
	}
	std::string	printui="nil";
	if(userInfo)
	{
		printsender=userInfo->getClassType();
	}
#ifdef _DEBUG
	printf("post Notification Name %s (sender = %s - userInfo = %s)\n",printname.c_str(),printsender.c_str(),printui.c_str());
#endif
}
#endif

void CCNotificationCenter::postNotificationName(CCString* notificationName,CCObject* sender,CCObject* userInfo)
{
	if(postLevel == 0)
	{
		manageRemoveObserverList();
	}
	postLevel++;
#ifdef PRINTF_NOTIFICATION
	PrintPost(notificationName,sender,userInfo);
#endif

	const std::string& NM=notificationName->m_sString;
	// search if some observers exists for this msg

	if(myNotificationMap.find(NM) != myNotificationMap.end())
	{
		unsigned int j;

		// create notification
		CCNotification*	notification=new CCNotification(notificationName,sender,userInfo);

#ifdef _DEBUG
		bool EventToTrace=false;
		if(notificationName->m_sString == "EVENT_DISABLE_FORCE_IN_ACTION")
		{
#ifdef _DEBUG
			printf("start %s post\n",notificationName->m_sString.c_str());
#endif
			EventToTrace=true;
		}
#endif

		bool endj=false;
		j=0;

		while(!endj)
		{
			CCObject* currentobserver=(CCObject*)myNotificationMap[NM][j].myObserver;
			#ifdef _DEBUG
			if(EventToTrace)
			{
				printf("found current observer : %s \n",currentobserver->getClassType());
			}
			#endif
			bool endi=false;
			unsigned int i=0;
			while(!endi)
			{
				if(!myNotificationMap[NM][j].myVector[i].myIsRemoved)
				{		
					if((myNotificationMap[NM][j].myVector[i].mySender == 0) || (myNotificationMap[NM][j].myVector[i].mySender == sender))
					{
						(currentobserver->*myNotificationMap[NM][j].myVector[i].mySelector)(notification);
						#ifdef _DEBUG
						if(EventToTrace)
						{
							printf("current observer : %s callback called\n",currentobserver->getClassType());
						}
						#endif
					}
					#ifdef _DEBUG
					else if(EventToTrace)
					{
						printf("current observer : %s callback was not send because of not registered sender\n",currentobserver->getClassType());
					}
					#endif
				}
				#ifdef _DEBUG
				else if(EventToTrace)
				{
					printf("current observer : %s callback was removed\n",currentobserver->getClassType());
				}
				#endif

				i++;
				if(i>=myNotificationMap[NM][j].myVector.size())
				{
					endi=true;
				}
			}
			j++;
			if(j>=myNotificationMap[NM].size())
			{
				endj=true;
			}
		}
		#ifdef _DEBUG
		if(EventToTrace)
		{
			printf("end %s post\n",notificationName->m_sString.c_str());
		}
		#endif
		delete notification;
	}
	postLevel--;
}

void	CCNotificationCenter::update()
{
	manageRemoveObserverList();
}