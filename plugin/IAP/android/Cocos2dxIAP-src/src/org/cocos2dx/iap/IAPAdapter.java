package org.cocos2dx.iap;

public interface IAPAdapter
{		
	public void loadProduct(String productId);
	public void purchaseProduct(String productId);
	public boolean isServiceValid();
	public void notifyIAPToExit();
	public String getAdapterName();
}