package org.cocos2dx.iap;

public class TransactionInfo {
	
	public String mProductId;
	public String mTranscationId;
	public String mErrorDescription;
	
	public TransactionInfo(String productId)
	{
		mProductId = productId;
		mTranscationId = "";
		mErrorDescription = "";
	}
	
	public TransactionInfo(String productId, String transcactionId)
	{
		mProductId = productId;
		mTranscationId = transcactionId;
		mErrorDescription = "";
	}

	public TransactionInfo(String productId, String transcactionId, String errorDescription)
	{
		mProductId = productId;
		mTranscationId = transcactionId;
		mErrorDescription = errorDescription;
	}
}
