//
// MainScene class
//
var MainScene = function(){};

// Create callback for timeline
MainScene.prototype.onCallback1 = function()
{	
    // Rotate the label when the button is pressed
    this.helloLabel.runAction(cc.RotateBy.create(1,360));
    this.helloLabel.setString("Callback 1");
};

MainScene.prototype.onCallback2 = function()
{	
    // Rotate the label when the button is pressed
    this.helloLabel.runAction(cc.RotateBy.create(1,-360));
    this.helloLabel.setString("Callback 2");
};

MainScene.prototype.onCallbackTest = function()
{
    cc.log("onCallbackTest");
}